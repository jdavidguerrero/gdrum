// AudioModule.h
#ifndef AUDIOMODULE_H
#define AUDIOMODULE_H

#include <vector>
#include <string>
#include "RtAudio.h"

class AudioSample {
public:
    std::vector<float> data;
    bool play = false;
    size_t currentSampleIndex = 0;

    void loadFromFile(const std::string& filename);
    void reset() { 
        currentSampleIndex = 0;
        play = false;
    }
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
    void setupAudioStream();
};

#endif // AUDIOMODULE_H