// AudioModule.h
#ifndef AUDIOMODULE_H
#define AUDIOMODULE_H

#include <chrono>
#include <vector>
#include <string>
#include "RtAudio.h"


class AudioSample {
public:
    std::vector<float> data;
    int channel = 0;
    size_t frameCounter = 0;
    void loadFromFile(const std::string& filename);
    void reset() { frameCounter = 0; } 
};
class AudioModule {
public:
    AudioModule();
    ~AudioModule();
    void loadSampleForChannel(const std::string& filename, size_t channel);
    void playSample(size_t channel);
    static int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                             double streamTime, RtAudioStreamStatus status, void *userData);

private:
    std::vector<AudioSample> samples;
    RtAudio dac;
    size_t activeChannel = -1;
    void setupAudioStream();
};

#endif // AUDIOMODULE_H