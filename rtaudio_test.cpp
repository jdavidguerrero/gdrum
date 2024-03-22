#include <sndfile.h>
#include <vector>
#include <iostream>
#include "RtAudio.h"


std::vector<float> loadAudioFile(const std::string& filename) {
    SF_INFO sfinfo;
    SNDFILE *infile = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    if (!infile) {
        std::cerr << "Can't open file: " << filename << std::endl;
        return {};
    }

    std::vector<float> buffer(sfinfo.frames * sfinfo.channels);
    long long readcount = sf_read_float(infile, buffer.data(), buffer.size());

    sf_close(infile);

    if (readcount < (long long)buffer.size()) {
        std::cerr << "File read incompleted" << std::endl;
        return {};
    }

    return buffer;
}

int playback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
             double streamTime, RtAudioStreamStatus status, void *userData) {
    auto *out = static_cast<float *>(outputBuffer);
    auto *data = static_cast<std::vector<float> *>(userData);
    static size_t frameCounter = 0;

    for (unsigned int i = 0; i < nBufferFrames; ++i) {
        if (frameCounter < data->size()) {
            *out++ = (*data)[frameCounter++]; // Left channel
            if (frameCounter < data->size()) *out++ = (*data)[frameCounter++]; // Right channel
        } else {
            *out++ = 0;
            *out++ = 0;
        }
    }
    return 0;
}

int main() {
    std::vector<float> audioData = loadAudioFile("resources/audio/slap.wav");

    RtAudio dac;
    if (dac.getDeviceCount() < 1) {
        std::cerr << "No audio devices found!\n";
        exit(1);
    }

    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 2; // Estéreo
    unsigned int sampleRate = 44100; // Ajusta a la tasa de muestreo de tu archivo
    unsigned int bufferFrames = 512; // Tamaño del buffer

    try {
        dac.openStream(&parameters, nullptr, RTAUDIO_FLOAT32,
                       sampleRate, &bufferFrames, &playback, &audioData);
        dac.startStream();
    } catch (RtAudioError &e) {
        e.printMessage();
        exit(1);
    }

    std::cout << "Presiona Enter para salir..." << std::endl;
    std::cin.get();

    if (dac.isStreamOpen()) dac.closeStream();
}







