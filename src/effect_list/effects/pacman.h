#pragma once

#include "effect_list/effect.h"

#define PACMAN_W 10
#define PACMAN_H 10

static const uint32_t pacman1[PACMAN_W*PACMAN_H] PROGMEM = {
    0x000000, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0xffff00, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0xffff00, 0xffff00, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
};

static const uint32_t pacman2[PACMAN_W*PACMAN_H] PROGMEM = {
    0x000000, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0xffff00, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0xffff00, 0xffff00, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000,
    0x000000, 0x000000, 0x000000, 0xffff00, 0xffff00, 0xffff00, 0xffff00, 0x000000, 0x000000, 0x000000,
};

class Pacman : public Effect
{
public:
    Pacman() {}

    void on_init()
    {
        phase = 0;
        set_fps(5);
    }

    void on_update()
    {
        fader(255);
        CRGB c(255, 255, 255);
        for (int i = PACMAN_W - phase ; i < WIDTH ; i += 4)
        {
            setPixCl(PACMAN_H / 2 - 1, i, c);
            setPixCl(PACMAN_H / 2 - 1, i + 1, c);
            setPixCl(PACMAN_H / 2, i, c);
            setPixCl(PACMAN_H / 2, i + 1, c);
        }
        drawSprite(0, 0, PACMAN_W, PACMAN_H, phase / 2 ? pacman1 : pacman2);
        phase = (phase + 1) % 4;
    }
private:
    static void setPixCl(int16_t x, int16_t y, CRGB cl) {
        if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH) 
            getPix(x, y) = cl;
    }

    static void drawSprite(int x, int y, int w, int h, const uint32_t *spr)
    {
        for (int i = 0 ; i < w ; ++i)
        {
            for (int j = 0 ; j < h ; ++j)
            {
                int v = pgm_read_dword(spr + i + j * w);
                if (v)
                    setPixCl(y + j, x + i, CRGB(v));
            }
        }
    }

private:
    int phase;
};