#include <audio.hpp>

// <Audio Manager>
std::map<std::string, SoundVectorPtr> AudioManager::sound_map;
std::map<std::string, MusicPtr>       AudioManager::music_map;
float AudioManager::timer = 0.0f;
float AudioManager::tick  = 210.0f;

float AudioManager::sfx_volume    = 1.f;
float AudioManager::music_volume  = 1.f;

float AudioManager::music_fade = 1.f;
float AudioManager::fade_speed = 1.f;

MusicPtr AudioManager::track_playing = nullptr;
MusicPtr AudioManager::track_fading  = nullptr;

void AudioManager::process_track_fade(float delta) {
    music_fade += delta * fade_speed;
    music_fade = std::min(music_fade, 1.f);

    if (track_playing.get() != nullptr) {
        SetMusicVolume(*track_playing.get(), music_volume * music_fade);

        if (!IsMusicStreamPlaying(*track_playing.get())) {
            PlayMusicStream(*track_playing.get());
        }
        UpdateMusicStream(*track_playing.get());
    }

    if (track_fading.get() != nullptr) {
        SetMusicVolume(*track_fading.get(),  music_volume * (1.f - music_fade));

        if (!IsMusicStreamPlaying(*track_fading.get()) && music_fade != 1.f) {
            PlayMusicStream(*track_fading.get());
        }

        UpdateMusicStream(*track_fading.get());
    }
}

void AudioManager::play_track(std::string path, float fade = 0.f) {
    music_fade = 1.f - music_fade;
    fade_speed = 1 / (fade + 0.001f);

    if (track_playing != nullptr) 
        track_fading = std::make_shared<Music>(*track_playing.get());
    track_playing = get_track(path);

    PlayMusicStream(*track_playing.get());
}

// Load a track and put it's smart pointer into the track map (it's path is the key)
void AudioManager::load_track(std::string name) {

    music_map[name] = std::make_shared<Music>(LoadMusicStream((MUSIC_DIR name).c_str()));
}

// Returns a track smart pointer and loads the sound if required
MusicPtr AudioManager::get_track(std::string name) {
    if (music_map.find(name) != music_map.end())
        return music_map[name];

    load_track(name);
    return music_map[name];
}

// Unloads a track
void AudioManager::unload_track(std::string name) {

    Music &track = *music_map[name].get();
    UnloadMusicStream(track);

    music_map.erase(name);
}

// Load a sound and put it's smart pointer into the sound map (it's path is the key)
void AudioManager::load_sfx(std::string name) {
    std::vector<Sound> sound_arr {LoadSound((SFX_DIR name).c_str())};

    sound_map[name] = std::make_shared<std::vector<Sound>>(sound_arr);
}

// Returns a sound smart pointer and loads the sound if required
SoundVectorPtr AudioManager::get_sfx(std::string name) {
    if (sound_map.find(name) != sound_map.end())
        return sound_map[name];

    load_sfx(name);
    return sound_map[name];
}

// Unloads a sound
void AudioManager::unload_sfx(std::string name) {

    std::vector<Sound>& arr = *sound_map[name].get();

    for (auto& sound: arr)
        UnloadSound(sound);

    sound_map.erase(name);
}

// Unloads all sounds and tracks that aren't being referanced 
void AudioManager::unload_unused() {
    for (auto &sound_pair: sound_map) {

        if (sound_pair.second.use_count() == 1) {
            unload_sfx(sound_pair.first);
        }
    }

    for (auto &music_pair: music_map) {

        if (music_pair.second.use_count() == 1) {
            unload_track(music_pair.first);
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

// Reloads all sounds and tracks
void AudioManager::reload() {
    for (auto &sound_pair: sound_map) {
        
        std::vector<Sound>& arr = *sound_pair.second.get();

        for (auto& sound: arr)
            UnloadSound(sound);
        
        arr.clear();
        arr.push_back(LoadSound((SFX_DIR sound_pair.first).c_str()));
    }

    for (auto &music_pair: music_map) {
        
        UnloadMusicStream(*music_pair.second.get());
        *music_pair.second.get() = LoadMusicStream((MUSIC_DIR music_pair.first).c_str());
    }
}

// Unloads all sounds and tracks
void AudioManager::unload_all() {

    for (auto& texture_pair: sound_map) {
        unload_sfx(texture_pair.first);
    }
    for (auto& music_pair: music_map) {
        unload_track(music_pair.first);
    }
}

// Plays a sound with randomized pitch and volume
void AudioManager::play_sfx(std::string path, float volume, float pitch, float volume_rand, float pitch_rand) {
    SoundVectorPtr sound_arr_ptr = get_sfx(path);

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