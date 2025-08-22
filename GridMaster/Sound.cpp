#include "Sound.h"

bool sound_new(struct Sound **sound)
{
    *sound = new Sound;
    if (!*sound)
    {
        std::cerr << "Error in initializing new Sound.\n";
        return false;
    }
    struct Sound *s = *sound;
    s->audioEngine = new ma_engine;
    if (ma_engine_init(NULL, s->audioEngine) != MA_SUCCESS)
    {
        std::cerr << "Failed to initialize audio engine.\n";
        return false;
    }
    s->explosionSound = new ma_sound;
    if (ma_sound_init_from_file(s->audioEngine, "Assets/explosion.mp3", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC,
                                NULL, NULL, s->explosionSound) != MA_SUCCESS)
    {
        std::cerr << "Failed to load explosion sound.\n";
        return false;
    }
    s->radiationSound = new ma_sound;
    if (ma_sound_init_from_file(s->audioEngine, "Assets/radiation.mp3", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC,
                                NULL, NULL, s->radiationSound) != MA_SUCCESS)
    {
        std::cerr << "Failed to load radiation sound.\n";
        return false;
    }
    s->beepSound = new ma_sound;
    if (ma_sound_init_from_file(s->audioEngine, "Assets/beep.mp3", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL,
                                NULL, s->beepSound) != MA_SUCCESS)
    {
        std::cerr << "Failed to load beep sound.\n";
        return false;
    }
    s->winSound = new ma_sound;
    if (ma_sound_init_from_file(s->audioEngine, "Assets/win.mp3", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL,
                                NULL, s->winSound) != MA_SUCCESS)
    {
        std::cerr << "Failed to load win sound.\n";
        return false;
    }
    s->winGridMasterSound = new ma_sound;
    if (ma_sound_init_from_file(s->audioEngine, "Assets/winGridMaster.mp3", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC,
                                NULL, NULL, s->winGridMasterSound) != MA_SUCCESS)
    {
        std::cerr << "Failed to load win Grid Master sound.\n";
        return false;
    }

    s->clickSound = new ma_sound;
    if (ma_sound_init_from_file(s->audioEngine, "Assets/click.mp3", MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL,
                                NULL, s->clickSound) != MA_SUCCESS)
    {
        std::cerr << "Failed to load win Grid Master sound.\n";
        return false;
    }

    return true;
}
void sound_free(struct Sound **sound)
{
    if (*sound)
    {
        struct Sound *s = *sound;
        if (s->explosionSound)
        {
            ma_sound_uninit(s->explosionSound);
            delete s->explosionSound;
            s->explosionSound = nullptr;
        }
        if (s->radiationSound)
        {
            ma_sound_uninit(s->radiationSound);
            delete s->radiationSound;
            s->radiationSound = nullptr;
        }
        if (s->beepSound)
        {
            ma_sound_uninit(s->beepSound);
            delete s->beepSound;
            s->beepSound = nullptr;
        }
        if (s->winSound)
        {
            ma_sound_uninit(s->winSound);
            delete s->winSound;
            s->winSound = nullptr;
        }
        if (s->winGridMasterSound)
        {
            ma_sound_uninit(s->winGridMasterSound);
            delete s->winGridMasterSound;
            s->winGridMasterSound = nullptr;
        }
        if (s->clickSound)
        {
            ma_sound_uninit(s->clickSound);
            delete s->clickSound;
            s->clickSound = nullptr;
        }
        if (s->audioEngine)
        {
            ma_engine_uninit(s->audioEngine);
            delete s->audioEngine;
            s->audioEngine = nullptr;
        }

        delete s;
        *sound = nullptr;
        std::cout << "Free Sound.\n";
    }
}

// Play seletced sound, looping sounds keep getting called every frame but it doesn't restart until is finished
void sound_play(struct Sound *s, SoundType type)
{
    if (s)
    {
        switch (type)
        {
        case SOUND_EXPLOSION:
            if (s->explosionSound)
                ma_sound_start(s->explosionSound);
            break;
        case SOUND_RADIATION:
            if (s->radiationSound)
                ma_sound_start(s->radiationSound);
            break;
        case SOUND_BEEP:
            if (s->beepSound)
                ma_sound_start(s->beepSound);
            break;
        case SOUND_WIN:
            if (s->winSound)
                ma_sound_start(s->winSound);
            break;
        case SOUND_WIN_GRID_MASTER:
            if (s->winGridMasterSound)
                ma_sound_start(s->winGridMasterSound);
            break;
        case SOUND_CLICK:
            if (s->clickSound)
                ma_sound_start(s->clickSound);
            break;
        default:
            break;
        }
    }
}

// Stop the sound and reset its position to the start
void sound_stop(struct Sound *s, SoundType type)
{
    if (s)
    {
        switch (type)
        {
        case SOUND_EXPLOSION:
            if (s->explosionSound)
            {
                ma_sound_stop(s->explosionSound);
                ma_sound_seek_to_pcm_frame(s->explosionSound, 0);
            }

            break;
        case SOUND_RADIATION:
            if (s->radiationSound)
            {
                ma_sound_stop(s->radiationSound);
                ma_sound_seek_to_pcm_frame(s->radiationSound, 0);
            }
            break;
        case SOUND_BEEP:
            if (s->beepSound)
            {
                ma_sound_stop(s->beepSound);
                ma_sound_seek_to_pcm_frame(s->beepSound, 0);
            }
            break;
        case SOUND_WIN:
            if (s->winSound)
            {
                ma_sound_stop(s->winSound);
                ma_sound_seek_to_pcm_frame(s->winSound, 0);
            }
            break;
        case SOUND_WIN_GRID_MASTER:
            if (s->winGridMasterSound)
            {
                ma_sound_stop(s->winGridMasterSound);
                ma_sound_seek_to_pcm_frame(s->winGridMasterSound, 0);
            }
            break;
        case SOUND_CLICK:
            if (s->clickSound)
            {
                ma_sound_stop(s->clickSound);
                ma_sound_seek_to_pcm_frame(s->clickSound, 0);
            }
            break;
        default:
            break;
        }
    }
}
