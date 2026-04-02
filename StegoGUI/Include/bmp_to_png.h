#pragma once

#include <string>

/**
 * @brief Результат конвертации
 */
enum class ConvertResult {
    OK = 0,
    ERROR_BMP_OPEN,          // Не удалось открыть BMP файл
    ERROR_BMP_INVALID,       // Некорректный формат BMP
    ERROR_BMP_UNSUPPORTED,   // Неподдерживаемая глубина цвета (не 8 или 24 бит)
    ERROR_PNG_OPEN,          // Не удалось открыть выходной PNG файл
    ERROR_PNG_WRITE,         // Ошибка записи PNG
    ERROR_MEMORY,            // Ошибка выделения памяти
};

/**
 * @brief Конвертирует BMP файл (8-бит или 24-бит) в PNG файл.
 *
 * @param bmpFile   Путь к входному BMP файлу.
 * @param pngFile   Путь к выходному PNG файлу (будет создан/перезаписан).
 * @param verbose   Если true — выводит диагностику в stderr.
 * @return          ConvertResult::OK при успехе, иначе код ошибки.
 *
 * Поддерживаемые форматы BMP:
 *   - 8-бит с палитрой (indexed colour, DIB BITMAPINFOHEADER)
 *   - 24-бит RGB (без сжатия, BI_RGB)
 *
 * Генерируемый PNG:
 *   - Тип цвета: RGB (color type 2), 8 бит на канал
 *   - Фильтр: None (быстро и достаточно для большинства изображений)
 *   - Сжатие: zlib deflate (уровень 6) через стандартную библиотеку <zlib.h>
 */
ConvertResult bmpToPng(const std::string& bmpFile,
    const std::string& pngFile,
    bool verbose = false);

/**
 * @brief Возвращает человекочитаемое описание кода ошибки.
 */
const char* convertResultMessage(ConvertResult result);