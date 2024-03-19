#include <audio.hpp>

// <Audio Manager>
std::map<std::string, SoundVectorPtr> AudioManager::sound_map;
float AudioManager::timer = 0.0f;
float AudioManager::tick  = 210.0f;

float AudioManager::sfx_volume    = 1.f;
float AudioManager::music_volume  = 1.f;

// Load a sound and put it's smart pointer into the sound map (it's path is the key)
void AudioManager::load(std::string name) {
    std::vector<Sound> sound_arr {LoadSound((SFX_DIR name).c_str())};

    sound_map[name] = std::make_shared<std::vector<Sound>>(sound_arr);
}

// Returns a sound smart pointer and loads the sound if required
SoundVectorPtr AudioManager::get(std::string name) {
    if (sound_map.find(name) != sound_map.end())
        return sound_map[name];

    load(name);
    return sound_map[name];
}

// Unloads a sound
void AudioManager::unload(std::string name) {

    std::vector<Sound>& arr = *sound_map[name].get();

    for (auto& sound: arr)
        UnloadSound(sound);

    sound_map.erase(name);
}

// Unloads all sounds that aren't being referanced 
void AudioManager::unload_unused() {
    for (auto& sound_pair: sound_map) {

        if (sound_pair.second.use_count() == 1) {
            unload(sound_pair.first);
        }
    }
}
// Ticks down a timer which calls "unload_unused()" when it hits 0 every "tick" seconds
void AudioManager::unload_check() {
    timer -= GetFrameTime();

    if (timer < .0) {
        timer = tick;

        unload_unused();
    }
}

// Reloads all sounds
void AudioManager::reload() {
    for (auto& sound_pair: sound_map) {
        
        std::vector<Sound>& arr = *sound_pair.second.get();

        for (auto& sound: arr)
            UnloadSound(sound);
        
        arr.clear();
        arr.push_back(LoadSound((SFX_DIR sound_pair.first).c_str()));
    }
}

// Unloads all sounds
void AudioManager::unload_all() {
    for (auto& texture_pair: sound_map) {

        unload(texture_pair.first);
    }
}

// Plays a sound with randomized pitch and volume
void AudioManager::play_sound(std::string path, float volume, float pitch, float volume_rand, float pitch_rand) {
    SoundVectorPtr sound_arr_ptr = get(path);

    Sound sound;

    bool found = false;    
    for (auto& potential_sound: *sound_arr_ptr.get()) {

        if (!IsSoundPlaying(potential_sound)) {
            found = true;
            sound = potential_sound;
        }
    }

    if (!found) {
        sound = LoadSound((SFX_DIR path).c_str());
        sound_arr_ptr.get()->push_back(sound);
    }

    float rand_pitch = pitch + RandF2() * pitch_rand * .5,
            rand_vol = volume + RandF2() * volume_rand * .5;
    
    SetSoundPitch(sound, rand_pitch);
    SetSoundVolume(sound, rand_vol * sfx_volume);

    PlaySound(sound);
}