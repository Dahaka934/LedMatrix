#pragma once

#include "effect_list/effect.h"
#include "libs/fonts.h"

class TextMode : public Effect {
  uint8_t *current_text;
  uint32_t speed = 5;
  uint32_t tick;
  const uint8_t font[SYM_AMNT][SYM_SIZE] = DEFAULT_FONTS;

  uint8_t convert_char_utf8_to_cp1251(const char *str, uint32_t &pos) {
    if (str[pos] < 0x80) {
      return str[pos];
    } else if (str[pos] == 0xd0) {
      pos++;

      if (str[pos] >= 0x90 && str[pos] <= 0xbf) {
        return (uint16_t)str[pos] - 0xcfd0;
      } else if (str[pos] == 0x81) {
        return (uint16_t)str[pos] - 0xcfd9;
      }
    } else if (str[pos] == 0xd1) {
      pos++;

      if (str[pos] >= 0x80 && str[pos] <= 0x8f) {
        return (uint16_t)str[pos] - 0xd090;
      } else if (str[pos] == 0x91) {
        return (uint16_t)str[pos] - 0xd0d9;
      }
    }

    if (str[pos] == '\0')
      pos--;  // фикс чтоб внешний цикл не ушёл в бесконечность
    return 0;
  }

  uint8_t *convert_utf8_to_cp1251(const char *str) {
    uint32_t pos = 0;
    uint32_t size = 0;

    if (str == nullptr) return nullptr;
    while (str[pos] != '\0') {
      if (convert_char_utf8_to_cp1251(str, pos)) {
        size++;
      } else {
        out("Unknown symbol: pos %d: '%c' %d\n", pos, str[pos], str[pos]);
        return nullptr;
      }
      pos++;
    }

    uint8_t *ret_str = new uint8_t[size + 1];
    pos = 0;
    uint32_t npos = 0;

    while (str[pos] != '\0') {
      ret_str[npos] = convert_char_utf8_to_cp1251(str, pos);

      if (ret_str[npos]) {
        npos++;
      }
      pos++;
    }

    ret_str[npos] = '\0';

    return ret_str;
  }

  uint8_t get_char_num_in_font(uint8_t ch) {
    if (ch >= 32 && ch <= 126) {
      return ch - 32;
    } else if (ch >= 192) {
      return ch - 97;
    }
    out("Undefined symbols: %c\n", ch);

    return 0;
  }

  void draw_symbol(uint8_t ch, int16_t x_offset, int16_t y_offset) {
    ch = get_char_num_in_font(ch);

    uint8_t byte_num = 0;
    uint8_t cur_byte = 0;

    // нивротебенная хуйня для хуёвых шрифтов
    for (uint8_t i = 0; i < LET_WIDTH; ++i) {
      for (uint8_t j = 0; j < LET_HEIGHT; ++j) {
        uint8_t cur_bit = (i * LET_HEIGHT + j) % 8;
        if (cur_bit == 0) {
          cur_byte = font[ch][byte_num];
          byte_num++;
        }

        if (cur_byte & (1 << cur_bit)) {
          LedMatrix.at(x_offset + i, y_offset + j) = 0xff00ff;
        }
      }
    }
  }

  void draw_text() {
    if (current_text == nullptr) return;

    uint32_t pos = 0;
    int32_t sym_pos = 0;

    while (current_text[pos] != '\0') {
      sym_pos = LEDS_WIDTH + pos * (LET_WIDTH + FONT_SPACE) - tick / speed;

      if (sym_pos > -LET_WIDTH && sym_pos < LEDS_WIDTH) {
        draw_symbol(current_text[pos], sym_pos, (LEDS_HEIGHT - LET_HEIGHT) / 2);
      }

      pos++;
    }

    if (sym_pos <= -LET_WIDTH) {
      tick = 0;
    }
  }

 public:
  TextMode() {}
  ~TextMode() {
    if (current_text) {
      delete current_text;
    }
  }

  void set_text(const char *text) {
    if (current_text) {
      delete current_text;
      current_text = nullptr;
    }

    current_text = convert_utf8_to_cp1251(text);
  }

  void on_init() {
    current_text = nullptr;
    set_text("Улыбнись этому прекрасному дню))");
    set_fps(60);
  }

  void on_update() {
    on_clear();
    draw_text();
    tick++;
  }
};
