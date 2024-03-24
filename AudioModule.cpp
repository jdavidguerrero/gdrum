// AudioModule.cpp
#include <sndfile.h> // Asegúrate de que libsndfile está instalada
#include <iostream>
#include "AudioModule.h"
#include <cstring>
#include <vector>
#include <thread>
#include <chrono>

AudioModule::AudioModule() {
    if (dac.getDeviceCount() < 1) {
        std::cerr << "No audio devices found!\n";
        exit(1);
    }
    setupAudioStream();
}

AudioModule::~AudioModule() {
    if (dac.isStreamOpen()) dac.closeStream();
}

void AudioSample::loadFromFile(const std::string& filename) {
    SF_INFO sfinfo;
    SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    if (!file) {
        std::cerr << "Can't open file: " << filename << std::endl;
        return;
    }

    size_t numSamples = sfinfo.frames * sfinfo.channels;
    data.resize(numSamples);

    sf_read_float(file, data.data(), numSamples);
    sf_close(file);
}

void AudioModule::setupAudioStream() {

 
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    std::cout << "DeviceId: "<< parameters.deviceId << std::endl;
    parameters.nChannels = 2; // stereo
    unsigned int sampleRate = 44100; 
    unsigned int bufferFrames = 512; 

    try {
    std::cout << "Conmfig audio stream..." << std::endl;
    dac.openStream(&parameters, nullptr, RTAUDIO_FLOAT32, sampleRate, &bufferFrames, &AudioModule::audioCallback, this);
    if (dac.isStreamOpen()) {
        std::cout << "Audio Stream Opened." << std::endl;
    } else {
        std::cerr << "Error opening AudioStream." << std::endl;
    }
} catch (RtAudioError &e) {
    std::cerr << "ERROR open stream: " << e.getMessage() << std::endl;
    exit(1);
} catch (std::exception &e) {
    // Captura excepciones estándar
    std::cerr << "Error standard: " << e.what() << std::endl;
    exit(1);
} catch (...) {
    // Captura cualquier otro tipo de error no especificado
    std::cerr << "Uknown error" << std::endl;
    exit(1);
}
}

void AudioModule::loadSampleForChannel(const std::string& filename, size_t channel) {
    std::cout << "Loading sample for channel " << channel << " from file: " << filename << std::endl;

    if (channel >= samples.size()) {
        samples.resize(channel + 1);
    }

    // Intenta cargar la muestra desde el archivo
    samples[channel].loadFromFile(filename);
    samples[channel].channel = channel;

    if (samples[channel].data.empty()) {
        std::cerr << "Error: Failed to load sample for channel " << channel << " from file: " << filename << std::endl;
    } else {
        std::cout << "Sample loaded successfully for channel " << channel << std::endl;
    }
}


void AudioModule::playSample(size_t channel) {
    if (channel < samples.size()) {
        samples[channel].reset(); // Restablecer el contador de frames
        activeChannel = channel;
        if (!dac.isStreamRunning()) {
            try {
                dac.startStream();
            } catch (RtAudioError &error) {
                error.printMessage();
            }
        }
    }
}   
int AudioModule::audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                               double streamTime, RtAudioStreamStatus status, void *userData) {
    AudioModule* audioModule = static_cast<AudioModule*>(userData);
    float *out = static_cast<float *>(outputBuffer);
    if (!audioModule || audioModule->activeChannel >= audioModule->samples.size()) {
        std::fill_n(out, nBufferFrames * 2, 0); // Llenar de ceros si no hay datos válidos
        return 0;
    }

    AudioSample& sample = audioModule->samples[audioModule->activeChannel];
    if (sample.frameCounter >= sample.data.size()) {
        // Si ya se reprodujo toda la muestra, llenar de ceros y considerar detener el stream
        std::fill_n(out, nBufferFrames * 2, 0);
        return 0;
    }

    for (unsigned int i = 0; i < nBufferFrames; ++i) {
        if (sample.frameCounter < sample.data.size()) {
            *out++ = sample.data[sample.frameCounter++]; // Canal izquierdo
            *out++ = sample.data[sample.frameCounter < sample.data.size() ? sample.frameCounter++ : sample.frameCounter]; // Canal derecho
        } else {
            *out++ = 0; // Relleno si se alcanza el final del vector
            *out++ = 0;
        }
    }

    return 0;
}
