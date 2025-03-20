// #pragma once

// #include <soloud.h>
// #include <soloud_wav.h>
// #include <vector>
// #include <list>
// #include <unordered_map>

// struct AudioHandle {
//     std::string name;
//     float volume;

//     SoLoud::handle handle;
// };


// class Audio {
// public:
//     Audio();
//     ~Audio();

//     static Audio instance; //! free at the end of the program

//     AudioHandle play(std::string name);
//     AudioHandle play(std::string name, float volume);
//     void stop(AudioHandle handle);
//     void stopAll();

//     void update();

//     void load(std::string name, std::string path);
// private:
//     float volume;

//     SoLoud::Soloud soloud;

//     std::unordered_map<std::string, SoLoud::Wav> sounds;
//     std::list<AudioHandle> handles;
// };

#include <engine/audio.h>

Audio Audio::instance = Audio();

Audio::Audio(){
    volume = 1.0f;

    soloud.init();
}

Audio::~Audio(){
    soloud.deinit();
}

AudioHandle* Audio::play(std::string name){
    return play(name, 1.0f);
}

AudioHandle* Audio::play(std::string name, float volume){
    AudioHandle* handle = new AudioHandle();
    handle->name = name;
    handle->volume = volume;

    SoLoud::Wav& sound = sounds[name];
    handle->handle = soloud.play(sound);
    soloud.setVolume(handle->handle, volume * this->volume);

    handles.push_back(handle);

    return handle;
}

bool Audio::isPlaying(AudioHandle* handle){
    return soloud.isValidVoiceHandle(handle->handle);
}

void Audio::stop(AudioHandle* handle){
    soloud.stop(handle->handle);
}

void Audio::stopAll(){
    for(AudioHandle* handle : handles){
        soloud.stop(handle->handle);
    }
}

void Audio::setVolume(float volume){
    this->volume = volume;
    for(AudioHandle* handle : handles){
        soloud.setVolume(handle->handle, handle->volume * this->volume);
    }
}

float Audio::getVolume(){
    return volume;
}

void Audio::update(){
    for (auto it = handles.begin(); it != handles.end(); ) {
        AudioHandle* handle = *it;
        if (!soloud.isValidVoiceHandle(handle->handle)) {
            it = handles.erase(it); // Erase returns the next iterator
            delete handle;
        } else {
            ++it; // Only increment if no deletion
        }
    }
}

void Audio::load(std::string name, std::string path){
    SoLoud::Wav& sound = sounds[name];
    sound.load(path.c_str());
}