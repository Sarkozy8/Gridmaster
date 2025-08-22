#pragma once
#include "miniaudio.h"
#include "Game.h"

// Control all sound systems and play audio

enum SoundType
{
    SOUND_EXPLOSION,
    SOUND_RADIATION,
    SOUND_BEEP,
    SOUND_WIN,
    SOUND_WIN_GRID_MASTER,
    SOUND_CLICK
};

struct Sound
{
    ma_engine *audioEngine = nullptr;
    ma_sound *explosionSound = nullptr;
    ma_sound *radiationSound = nullptr;
    ma_sound *beepSound = nullptr;
    ma_sound *winSound = nullptr;
    ma_sound *winGridMasterSound = nullptr;
    ma_sound *clickSound = nullptr;
};

bool sound_new(struct Sound **sound);
void sound_free(struct Sound **sound);
void sound_play(struct Sound *s, SoundType type);
void sound_stop(struct Sound *s, SoundType type);
