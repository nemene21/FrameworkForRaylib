#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <memory>
#include <map>
#include <vector>

#include <misc.hpp>

#define SFX_DIR  (std::string)("assets/sfx/") +

typedef std::shared_ptr<std::vector<Sound>> SoundVectorPtr;

class AudioManager {
public:
    static std::map<
        std::string,
        SoundVectorPtr
    > sound_map;

    static float timer;
    static float tick;
    static float sfx_volume;    
    static float music_volume;    

    static void load_sfx(std::string path);
    static void unload_sfx(std::string path);
    static SoundVectorPtr get_sfx(std::string path);

    static void play_sfx(std::string path, float volume=1.0, float pitch=1.0, float volume_rand=0.0, float pitch_rand=0.0);

    static void unload_check();
    static void unload_unused();
    static void unload_all();
    static void reload();
};

#endif