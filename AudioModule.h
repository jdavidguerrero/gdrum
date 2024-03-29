// AudioModule.h
#ifndef AUDIOMODULE_H
#define AUDIOMODULE_H

#include <chrono>
#include <vector>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


class AudioSample {
public:
    std::string sampleRoute;
};
class AudioModule {
public:
    AudioModule();
    ~AudioModule();
    void loadSampleForChannel(const std::string& filename, size_t channel);
    void playSample(size_t channel, int volume);
private:
    std::vector<AudioSample> samples;
    AudioModule(const AudioModule&) = delete;
    AudioModule& operator=(const AudioModule&) = delete;
};

#endif // AUDIOMODULE_H
