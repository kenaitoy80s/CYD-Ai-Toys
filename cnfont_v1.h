#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>

/*
 * ============================================================
 * CYD YoRadio V1 - Chinese Font Test
 * ============================================================
 *
 * 16x16 Chinese bitmap
 *
 * V1 first test characters:
 *
 * 中 文 电 台
 * 时 间
 * 网 络
 * 音 量
 *
 * Do NOT replace glcdfont.c.
 * English / numbers / YoRadio icons continue using
 * the original Adafruit_GFX font system.
 *
 * ============================================================
 */

struct CN_GLYPH_V1 {
  uint32_t code;
  const uint16_t *bitmap;
};


/*
 * ------------------------------------------------------------
 * 中文字模
 * ------------------------------------------------------------
 *
 * 每个汉字 16 x 16
 * 每一行 16 bit
 *
 * 1 = 前景色
 * 0 = 背景色
 *
 * 注意：
 * 下面先使用测试字模。
 * 第一炮的目标不是漂亮，而是先确认：
 *
 * UTF-8 → 中文字符 → CYD
 *
 * 这条链能跑通。
 * ------------------------------------------------------------
 */

static const uint16_t CN_ZHONG[] PROGMEM = {
  0x0180,
  0x0180,
  0x0180,
  0xFFFF,
  0x0180,
  0x0180,
  0x0180,
  0x0180,
  0x0180,
  0x0180,
  0x0180,
  0xFFFF,
  0x0180,
  0x0180,
  0x0180,
  0x0180
};


/*
 * ------------------------------------------------------------
 * 临时测试字库
 *
 * 第一版先让“中”活起来。
 * 其他字等第一炮成功后再加入正式字模。
 * ------------------------------------------------------------
 */

static const CN_GLYPH_V1 CN_GLYPHS_V1[] = {
  { 0x4E2D, CN_ZHONG }   // 中
};


static const uint8_t CN_GLYPH_COUNT_V1 =
    sizeof(CN_GLYPHS_V1) / sizeof(CN_GLYPHS_V1[0]);


/*
 * ------------------------------------------------------------
 * 查找中文字模
 * ------------------------------------------------------------
 */

static inline const uint16_t *
cnFindGlyphV1(uint32_t code)
{
  for (uint8_t i = 0; i < CN_GLYPH_COUNT_V1; i++) {

    if (CN_GLYPHS_V1[i].code == code) {
      return CN_GLYPHS_V1[i].bitmap;
    }
  }

  return nullptr;
}


/*
 * ------------------------------------------------------------
 * UTF-8 解码
 *
 * YoRadio / ESP32 字符串通常是 UTF-8。
 *
 * 中文：
 *
 * 中 = E4 B8 AD
 *
 * 这里把 3 个 byte 转成：
 *
 * 0x4E2D
 * ------------------------------------------------------------
 */

static inline uint32_t
cnUtf8DecodeV1(const char *&p)
{
  uint8_t c = (uint8_t)*p++;

  if (c < 0x80) {
    return c;
  }

  if ((c & 0xE0) == 0xC0) {

    uint8_t c2 = (uint8_t)*p++;

    return ((uint32_t)(c & 0x1F) << 6) |
           (c2 & 0x3F);
  }

  if ((c & 0xF0) == 0xE0) {

    uint8_t c2 = (uint8_t)*p++;
    uint8_t c3 = (uint8_t)*p++;

    return ((uint32_t)(c & 0x0F) << 12) |
           ((uint32_t)(c2 & 0x3F) << 6) |
           (c3 & 0x3F);
  }

  if ((c & 0xF8) == 0xF0) {

    uint8_t c2 = (uint8_t)*p++;
    uint8_t c3 = (uint8_t)*p++;
    uint8_t c4 = (uint8_t)*p++;

    return ((uint32_t)(c & 0x07) << 18) |
           ((uint32_t)(c2 & 0x3F) << 12) |
           ((uint32_t)(c3 & 0x3F) << 6) |
           (c4 & 0x3F);
  }

  return 0xFFFD;
}


/*
 * ------------------------------------------------------------
 * 画一个 16x16 中文字符
 * ------------------------------------------------------------
 */

template <class Display>
static inline bool
cnDrawGlyphV1(
    Display &display,
    int16_t x,
    int16_t y,
    uint32_t code,
    uint16_t foreground,
    uint16_t background)
{
  const uint16_t *glyph = cnFindGlyphV1(code);

  if (!glyph) {
    return false;
  }

  for (uint8_t row = 0; row < 16; row++) {

    uint16_t bits =
        pgm_read_word(&glyph[row]);

    for (uint8_t col = 0; col < 16; col++) {

      if (bits & (0x8000 >> col)) {

        display.drawPixel(
            x + col,
            y + row,
            foreground
        );
      }
      else {

        display.drawPixel(
            x + col,
            y + row,
            background
        );
      }
    }
  }

  return true;
}


/*
 * ------------------------------------------------------------
 * V1 中文测试
 *
 * 在屏幕指定位置显示：
 *
 * 中
 *
 * ------------------------------------------------------------
 */

template <class Display>
static inline void
cnTestV1(
    Display &display,
    int16_t x,
    int16_t y,
    uint16_t foreground,
    uint16_t background)
{
  cnDrawGlyphV1(
      display,
      x,
      y,
      0x4E2D,
      foreground,
      background
  );
}
