#include "../Include/bmp_to_png.h"

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <zlib.h>

// ─────────────────────────────────────────────────────────────────
//  Внутренние структуры BMP
// ─────────────────────────────────────────────────────────────────

#pragma pack(push, 1)
struct BmpFileHeader {
    uint16_t signature;   // 'BM'
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;  // Смещение до пиксельных данных
};

struct BmpInfoHeader {
    uint32_t headerSize;
    int32_t  width;
    int32_t  height;      // Положительный — нижний ряд первый; отрицательный — верхний ряд первый
    uint16_t colorPlanes;
    uint16_t bitsPerPixel;
    uint32_t compression; // 0 = BI_RGB
    uint32_t imageSize;
    int32_t  xPixelsPerMeter;
    int32_t  yPixelsPerMeter;
    uint32_t colorsInTable;
    uint32_t importantColors;
};
#pragma pack(pop)

// ─────────────────────────────────────────────────────────────────
//  Вспомогательные функции для PNG
// ─────────────────────────────────────────────────────────────────

static uint32_t crc32Table[256];
static bool     crcTableReady = false;

static void buildCrcTable() {
    for (uint32_t n = 0; n < 256; ++n) {
        uint32_t c = n;
        for (int k = 0; k < 8; ++k)
            c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
        crc32Table[n] = c;
    }
    crcTableReady = true;
}

// Записать 4 байта big-endian
static void writeBE32(std::ofstream& out, uint32_t v) {
    uint8_t buf[4] = {
        uint8_t(v >> 24), uint8_t(v >> 16),
        uint8_t(v >> 8),  uint8_t(v)
    };
    out.write(reinterpret_cast<char*>(buf), 4);
}

// Записать PNG chunk: длина (4) + тип (4) + данные + CRC (4)
static void writePngChunkV2(std::ofstream& out,
    const char     type[5],
    const uint8_t* data,
    uint32_t       len) {
    writeBE32(out, len);
    out.write(type, 4);
    if (len > 0) out.write(reinterpret_cast<const char*>(data), len);

    if (!crcTableReady) buildCrcTable();
    uint32_t crc = 0xFFFFFFFFu;
    for (int i = 0; i < 4; ++i)
        crc = crc32Table[(crc ^ uint8_t(type[i])) & 0xFF] ^ (crc >> 8);
    for (uint32_t i = 0; i < len; ++i)
        crc = crc32Table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    crc ^= 0xFFFFFFFFu;
    writeBE32(out, crc);
}

// ─────────────────────────────────────────────────────────────────
//  Основная функция
// ─────────────────────────────────────────────────────────────────

ConvertResult bmpToPng(const std::string& bmpFile,
    const std::string& pngFile,
    bool verbose) {

    // ── 1. Открыть и прочитать BMP ──────────────────────────────
    std::ifstream bmp(bmpFile, std::ios::binary);
    if (!bmp.is_open()) return ConvertResult::ERROR_BMP_OPEN;

    BmpFileHeader fh{};
    BmpInfoHeader ih{};

    bmp.read(reinterpret_cast<char*>(&fh), sizeof(fh));
    bmp.read(reinterpret_cast<char*>(&ih), sizeof(ih));

    if (!bmp || fh.signature != 0x4D42) return ConvertResult::ERROR_BMP_INVALID;
    if (ih.headerSize < 40)              return ConvertResult::ERROR_BMP_INVALID;
    if (ih.compression != 0)             return ConvertResult::ERROR_BMP_UNSUPPORTED;
    if (ih.bitsPerPixel != 8 && ih.bitsPerPixel != 24)
        return ConvertResult::ERROR_BMP_UNSUPPORTED;

    const int32_t  W = ih.width;
    const int32_t  absH = (ih.height < 0) ? -ih.height : ih.height;
    const bool     topDown = (ih.height < 0);
    const uint16_t bpp = ih.bitsPerPixel;

    if (W <= 0 || absH <= 0) return ConvertResult::ERROR_BMP_INVALID;

    if (verbose)
        std::cerr << "[bmpToPng] " << W << "x" << absH
        << " " << bpp << "bpp topDown=" << topDown << "\n";

    // ── 2. Прочитать палитру (только для 8 бит) ─────────────────
    // Палитра: до 256 записей по 4 байта (B, G, R, reserved)
    std::vector<uint8_t> palette;
    if (bpp == 8) {
        // Пропустить возможные дополнительные байты заголовка
        uint32_t paletteOffset = 14 + ih.headerSize; // после BmpFileHeader + BmpInfoHeader
        bmp.seekg(paletteOffset, std::ios::beg);

        uint32_t numColors = ih.colorsInTable ? ih.colorsInTable : 256u;
        palette.resize(numColors * 4);
        bmp.read(reinterpret_cast<char*>(palette.data()), palette.size());
        if (!bmp) return ConvertResult::ERROR_BMP_INVALID;
    }

    // ── 3. Прочитать пиксельные данные ──────────────────────────
    bmp.seekg(fh.dataOffset, std::ios::beg);

    // Строка BMP выравнивается до 4 байт
    uint32_t rowBytes = bpp == 8
        ? (uint32_t(W) + 3) & ~3u
        : (uint32_t(W) * 3 + 3) & ~3u;

    std::vector<uint8_t> bmpPixels(size_t(rowBytes) * absH);
    bmp.read(reinterpret_cast<char*>(bmpPixels.data()), bmpPixels.size());
    if (!bmp) return ConvertResult::ERROR_BMP_INVALID;
    bmp.close();

    // ── 4. Преобразовать в RGB ───────────────────────────────────
    // rawRgb[y][x] = {R, G, B}, y=0 — верхняя строка
    std::vector<uint8_t> rawRgb(size_t(W) * absH * 3);

    for (int32_t y = 0; y < absH; ++y) {
        // BMP без topDown хранит строки снизу вверх
        int32_t srcRow = topDown ? y : (absH - 1 - y);
        const uint8_t* src = bmpPixels.data() + size_t(srcRow) * rowBytes;
        uint8_t* dst = rawRgb.data() + size_t(y) * W * 3;

        if (bpp == 24) {
            for (int32_t x = 0; x < W; ++x) {
                dst[x * 3 + 0] = src[x * 3 + 2]; // R
                dst[x * 3 + 1] = src[x * 3 + 1]; // G
                dst[x * 3 + 2] = src[x * 3 + 0]; // B
            }
        }
        else { // 8 bit
            for (int32_t x = 0; x < W; ++x) {
                uint8_t idx = src[x];
                if (size_t(idx) * 4 + 2 < palette.size()) {
                    dst[x * 3 + 0] = palette[idx * 4 + 2]; // R
                    dst[x * 3 + 1] = palette[idx * 4 + 1]; // G
                    dst[x * 3 + 2] = palette[idx * 4 + 0]; // B
                }
                else {
                    dst[x * 3 + 0] = dst[x * 3 + 1] = dst[x * 3 + 2] = 0;
                }
            }
        }
    }

    // ── 5. Применить фильтр None и сжать zlib ───────────────────
    // PNG требует перед каждой строкой байт фильтра (0 = None)
    size_t filteredSize = size_t(absH) * (1 + size_t(W) * 3);
    std::vector<uint8_t> filtered(filteredSize);
    for (int32_t y = 0; y < absH; ++y) {
        filtered[size_t(y) * (1 + W * 3)] = 0; // filter type = None
        std::memcpy(&filtered[size_t(y) * (1 + W * 3) + 1],
            &rawRgb[size_t(y) * W * 3],
            size_t(W) * 3);
    }

    // Сжать deflate
    uLongf compBound = compressBound(filteredSize);
    std::vector<uint8_t> compressed(compBound);
    int zret = compress2(compressed.data(), &compBound,
        filtered.data(), filteredSize,
        6 /* уровень сжатия */);
    if (zret != Z_OK) return ConvertResult::ERROR_MEMORY;
    compressed.resize(compBound);

    if (verbose)
        std::cerr << "[bmpToPng] compressed " << filteredSize
        << " -> " << compBound << " bytes\n";

    // ── 6. Записать PNG файл ─────────────────────────────────────
    std::ofstream png(pngFile, std::ios::binary);
    if (!png.is_open()) return ConvertResult::ERROR_PNG_OPEN;

    // PNG сигнатура
    const uint8_t pngSig[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
    png.write(reinterpret_cast<const char*>(pngSig), 8);

    // IHDR chunk (13 байт)
    uint8_t ihdr[13];
    auto be32 = [](uint8_t* p, uint32_t v) {
        p[0] = v >> 24; p[1] = v >> 16; p[2] = v >> 8; p[3] = v;
        };
    be32(ihdr + 0, uint32_t(W));
    be32(ihdr + 4, uint32_t(absH));
    ihdr[8] = 8;  // bit depth
    ihdr[9] = 2;  // color type: RGB
    ihdr[10] = 0;  // compression method: deflate
    ihdr[11] = 0;  // filter method: adaptive
    ihdr[12] = 0;  // interlace method: none
    writePngChunkV2(png, "IHDR", ihdr, 13);

    // IDAT chunk
    writePngChunkV2(png, "IDAT", compressed.data(), uint32_t(compressed.size()));

    // IEND chunk
    writePngChunkV2(png, "IEND", nullptr, 0);

    if (!png) return ConvertResult::ERROR_PNG_WRITE;
    png.close();

    if (verbose)
        std::cerr << "[bmpToPng] Done -> " << pngFile << "\n";

    return ConvertResult::OK;
}

// ─────────────────────────────────────────────────────────────────
const char* convertResultMessage(ConvertResult result) {
    switch (result) {
    case ConvertResult::OK:                  return "Успех";
    case ConvertResult::ERROR_BMP_OPEN:      return "Не удалось открыть BMP файл";
    case ConvertResult::ERROR_BMP_INVALID:   return "Некорректный формат BMP";
    case ConvertResult::ERROR_BMP_UNSUPPORTED: return "Неподдерживаемая глубина цвета (нужно 8 или 24 бит)";
    case ConvertResult::ERROR_PNG_OPEN:      return "Не удалось создать PNG файл";
    case ConvertResult::ERROR_PNG_WRITE:     return "Ошибка записи PNG";
    case ConvertResult::ERROR_MEMORY:        return "Ошибка выделения памяти / сжатия";
    }
    return "Неизвестная ошибка";
}