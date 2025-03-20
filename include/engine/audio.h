#pragma once

#include <soloud.h>
#include <soloud_wav.h>
#include <vector>
#include <list>
#include <unordered_map>

struct AudioHandle {
    std::string name;
    float volume;

    SoLoud::handle handle;
};


class Audio {
public:
    Audio();
    ~Audio();

    static Audio instance; //! free at the end of the program

    AudioHandle* play(std::string name);
    AudioHandle* play(std::string name, float volume);

    bool isPlaying(AudioHandle* handle);    

    void stop(AudioHandle* handle);
    void stopAll();

    void setVolume(float volume);
    float getVolume();

    void update();

    void load(std::string name, std::string path);
private:
    float volume;

    SoLoud::Soloud soloud;

    std::unordered_map<std::string, SoLoud::Wav> sounds;
    std::list<AudioHandle*> handles;
};