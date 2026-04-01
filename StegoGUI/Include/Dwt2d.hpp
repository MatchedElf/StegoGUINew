#pragma once
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <cmath>

/**
 * Дискретное вейвлет-преобразование (DWT) для 2D изображений
 * Формат: 8-bit grayscale (один uint8_t на пиксель)
 *
 * Поддерживаемые вейвлеты:
 *   - Хаар (HAAR)        — простой, быстрый, целочисленный
 *   - Добеши-4 (DB4)     — сглаживающий, хорошее качество
 *
 * Результат прямого преобразования хранится в виде субполос:
 *
 *   ┌─────────┬─────────┐
 *   │   LL    │   HL    │  ← строки [0..H/2), столбцы [0..W/2) и [W/2..W)
 *   ├─────────┼─────────┤
 *   │   LH    │   HH    │  ← строки [H/2..H), столбцы [0..W/2) и [W/2..W)
 *   └─────────┴─────────┘
 *
 *   LL — аппроксимация
 *   HL — горизонтальные детали
 *   LH — вертикальные детали
 *   HH — диагональные детали
 *
 * Значения result[] могут выходить за [0, 255], поэтому хранятся как float.
 * Для записи обратно в uint8_t используйте вспомогательные функции ниже.
 */

namespace dwt {

    // ─── Перечисление вейвлетов ──────────────────────────────────────────────────

    enum class Wavelet { HAAR, DB4 };

    // ─── Вспомогательные типы ────────────────────────────────────────────────────

    // «Мягкий» результат: float-матрица того же размера width × height
    using FloatMatrix = std::vector<std::vector<float>>;

    // ─── Внутренние реализации (не использовать напрямую) ────────────────────────

    namespace detail {

        // Фильтры Хаара
        // Lo: [1, 1] / sqrt(2),  Hi: [-1, 1] / sqrt(2)
        inline void haar_forward_1d(float* data, int n)
        {
            // Нормированный Хаар: lifting-scheme
            std::vector<float> tmp(n);
            int half = n / 2;
            for (int i = 0; i < half; ++i) {
                float a = data[2 * i];
                float b = data[2 * i + 1];
                tmp[i] = (a + b) * 0.5f;   // приближение
                tmp[half + i] = (a - b) * 0.5f;   // детали
            }
            std::memcpy(data, tmp.data(), n * sizeof(float));
        }

        inline void haar_inverse_1d(float* data, int n)
        {
            std::vector<float> tmp(n);
            int half = n / 2;
            for (int i = 0; i < half; ++i) {
                float s = data[i];
                float d = data[half + i];
                tmp[2 * i] = s + d;
                tmp[2 * i + 1] = s - d;
            }
            std::memcpy(data, tmp.data(), n * sizeof(float));
        }

        // Фильтры Добеши-4 (4 коэффициента)
        // Нормированные коэффициенты масштабирующей функции
        static const float DB4_LO[4] = {
             0.48296291314469f,
             0.83651630373781f,
             0.22414386804186f,
            -0.12940952255092f
        };
        static const float DB4_HI[4] = {
             0.12940952255092f,
             0.22414386804186f,
            -0.83651630373781f,
             0.48296291314469f
        };
        // Синтезирующие фильтры (инверсия)
        static const float DB4_LO_R[4] = {
            -0.12940952255092f,
             0.22414386804186f,
             0.83651630373781f,
             0.48296291314469f
        };
        static const float DB4_HI_R[4] = {
            -0.48296291314469f,
             0.83651630373781f,
            -0.22414386804186f,
            -0.12940952255092f
        };

        inline void db4_forward_1d(float* data, int n)
        {
            std::vector<float> tmp(n, 0.f);
            int half = n / 2;
            for (int i = 0; i < half; ++i) {
                float lo = 0.f, hi = 0.f;
                for (int k = 0; k < 4; ++k) {
                    int idx = (2 * i + k) % n;   // периодическое продление
                    lo += DB4_LO[k] * data[idx];
                    hi += DB4_HI[k] * data[idx];
                }
                tmp[i] = lo;
                tmp[half + i] = hi;
            }
            std::memcpy(data, tmp.data(), n * sizeof(float));
        }

        inline void db4_inverse_1d(float* data, int n)
        {
            std::vector<float> tmp(n, 0.f);
            int half = n / 2;
            for (int i = 0; i < half; ++i) {
                for (int k = 0; k < 4; ++k) {
                    int out_idx = (2 * i + k) % n;
                    tmp[out_idx] += DB4_LO_R[k] * data[i]
                        + DB4_HI_R[k] * data[half + i];
                }
            }
            std::memcpy(data, tmp.data(), n * sizeof(float));
        }

        // Применить 1D-преобразование по строкам матрицы
        inline void apply_rows(FloatMatrix& mat, int rows, int cols, bool forward, Wavelet wv)
        {
            std::vector<float> row(cols);
            for (int r = 0; r < rows; ++r) {
                std::memcpy(row.data(), mat[r].data(), cols * sizeof(float));
                if (forward) {
                    if (wv == Wavelet::HAAR) haar_forward_1d(row.data(), cols);
                    else                     db4_forward_1d(row.data(), cols);
                }
                else {
                    if (wv == Wavelet::HAAR) haar_inverse_1d(row.data(), cols);
                    else                     db4_inverse_1d(row.data(), cols);
                }
                std::memcpy(mat[r].data(), row.data(), cols * sizeof(float));
            }
        }

        // Применить 1D-преобразование по столбцам матрицы
        inline void apply_cols(FloatMatrix& mat, int rows, int cols, bool forward, Wavelet wv)
        {
            std::vector<float> col(rows);
            for (int c = 0; c < cols; ++c) {
                for (int r = 0; r < rows; ++r) col[r] = mat[r][c];
                if (forward) {
                    if (wv == Wavelet::HAAR) haar_forward_1d(col.data(), rows);
                    else                     db4_forward_1d(col.data(), rows);
                }
                else {
                    if (wv == Wavelet::HAAR) haar_inverse_1d(col.data(), rows);
                    else                     db4_inverse_1d(col.data(), rows);
                }
                for (int r = 0; r < rows; ++r) mat[r][c] = col[r];
            }
        }

    } // namespace detail

    // ─── Публичный API ───────────────────────────────────────────────────────────

    /**
     * Прямое 2D DWT (один уровень разложения).
     *
     * @param pixels  Входное изображение: pixels[row][col], uint8_t
     * @param result  Выходная матрица: result[row][col], uint8_t
     *                Субполосы записываются как uint8_t через нормировку [0..255].
     *                Для точного float-результата используйте dwt2d_float().
     * @param width   Ширина изображения (должна быть чётной)
     * @param height  Высота изображения (должна быть чётной)
     * @param wv      Тип вейвлета (по умолчанию: HAAR)
     *
     * Примечание: result должен быть заранее выделен (height строк по width uint8_t).
     */
    inline void dwt2d(uint8_t** pixels,
        uint8_t** result,
        int       width,
        int       height,
        Wavelet   wv = Wavelet::HAAR)
    {
        if (width < 2 || width % 2 != 0) throw std::invalid_argument("width must be even and >= 2");
        if (height < 2 || height % 2 != 0) throw std::invalid_argument("height must be even and >= 2");

        // Перенести во float-матрицу
        FloatMatrix mat(height, std::vector<float>(width));
        for (int r = 0; r < height; ++r)
            for (int c = 0; c < width; ++c)
                mat[r][c] = static_cast<float>(pixels[r][c]);

        // Прямое преобразование: сначала строки, потом столбцы
        detail::apply_rows(mat, height, width, true, wv);
        detail::apply_cols(mat, height, width, true, wv);

        // Нормировать в [0, 255] для uint8_t вывода
        // (субполосы могут иметь значения вне [0,255], поэтому применяем min-max)
        float mn = mat[0][0], mx = mat[0][0];
        for (int r = 0; r < height; ++r)
            for (int c = 0; c < width; ++c) {
                if (mat[r][c] < mn) mn = mat[r][c];
                if (mat[r][c] > mx) mx = mat[r][c];
            }

        float range = (mx - mn) > 1e-6f ? (mx - mn) : 1.f;
        for (int r = 0; r < height; ++r)
            for (int c = 0; c < width; ++c)
                result[r][c] = static_cast<uint8_t>(
                    std::round((mat[r][c] - mn) / range * 255.f));
    }

    /**
     * Прямое 2D DWT с точным float-результатом.
     * Возвращает FloatMatrix — не теряет информацию об отрицательных деталях.
     */
    inline FloatMatrix dwt2d_float(uint8_t** pixels,
        int       width,
        int       height,
        Wavelet   wv = Wavelet::HAAR)
    {
        if (width < 2 || width % 2 != 0) throw std::invalid_argument("width must be even and >= 2");
        if (height < 2 || height % 2 != 0) throw std::invalid_argument("height must be even and >= 2");

        FloatMatrix mat(height, std::vector<float>(width));
        for (int r = 0; r < height; ++r)
            for (int c = 0; c < width; ++c)
                mat[r][c] = static_cast<float>(pixels[r][c]);

        detail::apply_rows(mat, height, width, true, wv);
        detail::apply_cols(mat, height, width, true, wv);
        return mat;
    }

    /**
     * Обратное 2D DWT (реконструкция изображения).
     *
     * @param coeffs  Float-матрица коэффициентов (результат dwt2d_float)
     * @param result  Выходное изображение result[row][col], uint8_t
     * @param width   Ширина
     * @param height  Высота
     * @param wv      Тип вейвлета (должен совпадать с прямым преобразованием)
     */
    inline void idwt2d(const FloatMatrix& coeffs,
        uint8_t** result,
        int                width,
        int                height,
        Wavelet            wv = Wavelet::HAAR)
    {
        FloatMatrix mat = coeffs;   // рабочая копия

        // Обратное: сначала столбцы, потом строки
        detail::apply_cols(mat, height, width, false, wv);
        detail::apply_rows(mat, height, width, false, wv);

        for (int r = 0; r < height; ++r)
            for (int c = 0; c < width; ++c) {
                float v = mat[r][c];
                if (v < 0.f)   v = 0.f;
                if (v > 255.f) v = 255.f;
                result[r][c] = static_cast<uint8_t>(std::round(v));
            }
    }

    /**
     * Многоуровневое прямое 2D DWT.
     * Рекурсивно разлагает субполосу LL заданное число раз.
     *
     * @param pixels  Входное изображение
     * @param result  Результат (uint8_t, нормированный)
     * @param width   Ширина
     * @param height  Высота
     * @param levels  Число уровней (рекомендуется 1..4)
     * @param wv      Тип вейвлета
     */
    inline void dwt2d_multilevel(uint8_t** pixels,
        uint8_t** result,
        int       width,
        int       height,
        int       levels = 3,
        Wavelet   wv = Wavelet::HAAR)
    {
        // Начальная копия как float
        FloatMatrix mat(height, std::vector<float>(width));
        for (int r = 0; r < height; ++r)
            for (int c = 0; c < width; ++c)
                mat[r][c] = static_cast<float>(pixels[r][c]);

        int cur_w = width, cur_h = height;
        for (int lv = 0; lv < levels; ++lv) {
            if (cur_w < 2 || cur_h < 2) break;

            // Создаём временную подматрицу для текущего уровня
            FloatMatrix sub(cur_h, std::vector<float>(cur_w));
            for (int r = 0; r < cur_h; ++r)
                for (int c = 0; c < cur_w; ++c)
                    sub[r][c] = mat[r][c];

            detail::apply_rows(sub, cur_h, cur_w, true, wv);
            detail::apply_cols(sub, cur_h, cur_w, true, wv);

            // Запись обратно
            for (int r = 0; r < cur_h; ++r)
                for (int c = 0; c < cur_w; ++c)
                    mat[r][c] = sub[r][c];

            cur_w /= 2;
            cur_h /= 2;
        }

        // Нормировка в [0..255]
        float mn = mat[0][0], mx = mat[0][0];
        for (int r = 0; r < height; ++r)
            for (int c = 0; c < width; ++c) {
                if (mat[r][c] < mn) mn = mat[r][c];
                if (mat[r][c] > mx) mx = mat[r][c];
            }
        float range = (mx - mn) > 1e-6f ? (mx - mn) : 1.f;
        for (int r = 0; r < height; ++r)
            for (int c = 0; c < width; ++c)
                result[r][c] = static_cast<uint8_t>(
                    std::round((mat[r][c] - mn) / range * 255.f));
    }

} // namespace dwt