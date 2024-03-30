// AudioModule.h
#ifndef AUDIOMODULE_H
#define AUDIOMODULE_H

#include "RtAudio.h"
#include <chrono>
#include <vector>
#include <string>

class AudioSample {
public:
    std::vector<float> data;
    int channel = 0;
    size_t frameCounter = 0;
    float volume = 0.5f; 
    void setVolume(int volumeLevel) {volume = static_cast<float>(volumeLevel) / 100.0f;}
    void loadFromFile(const std::string& filename);
    void reset() { frameCounter = 0; } 
};
class AudioModule {
public:
    AudioModule();
    ~AudioModule();
    size_t activeChannel = -1;
    void loadSampleForChannel(const std::string& filename, size_t channel);
    void playSample(size_t channel);
    static int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                             double streamTime, RtAudioStreamStatus status, void *userData);

private:
    std::vector<AudioSample> samples;
    RtAudio dac;
    void setupAudioStream();
};

#endif // AUDIOMODULE_H