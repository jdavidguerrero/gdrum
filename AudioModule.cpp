// AudioModule.cpp
#include <sndfile.h> // Asegúrate de que libsndfile está instalada
#include <iostream>
#include "AudioModule.h"
#include <cstring>

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
        std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
        return;
    }

    // Calcula el número total de muestras a leer (frames * canales)
    size_t numSamples = sfinfo.frames * sfinfo.channels;
    data.resize(numSamples);

    // Lee las muestras del archivo en el vector de datos
    sf_read_float(file, data.data(), numSamples);
    sf_close(file);

    // Inicializa el estado de reproducción
    play = false;
    currentSampleIndex = 0;
}

void AudioModule::setupAudioStream() {
    RtAudio::StreamParameters parameters;
    parameters.deviceId = 0;
    parameters.nChannels = 2; 
    parameters.firstChannel = 0;
    unsigned int sampleRate = 44100; 
    unsigned int bufferFrames = 512;
    try {
        dac.openStream(&parameters, nullptr, RTAUDIO_FLOAT32,
                       sampleRate, &bufferFrames, &AudioModule::audioCallback, this);
    } catch (RtAudioError &e) {
        e.printMessage();
        exit(1);
    }
}

void AudioModule::loadSampleForChannel(const std::string& filename, size_t channel) {
     if (channel >= samples.size()) {
        samples.resize(channel + 1);
    }
    samples[channel].loadFromFile(filename);
}

void AudioModule::playSample(size_t channel) {
    if (channel < samples.size()) {
        samples[channel].play = true;
        samples[channel].currentSampleIndex = 0; 
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
    auto* audioModule = static_cast<AudioModule*>(userData);
    float *buffer = static_cast<float*>(outputBuffer);
    std::memset(outputBuffer, 0, sizeof(float) * nBufferFrames * 2); // Limpia el buffer
    
    for (auto& sample : audioModule->samples) {
        if (sample.play && sample.currentSampleIndex < sample.data.size()) {
            for (unsigned int i = 0; i < nBufferFrames; ++i) {
                // Asume estéreo y suma los samples al buffer de salida
                buffer[i * 2] += sample.data[sample.currentSampleIndex]; // Canal izquierdo
                buffer[i * 2 + 1] += sample.data[sample.currentSampleIndex]; // Canal derecho
                sample.currentSampleIndex++; 
                if (sample.currentSampleIndex >= sample.data.size()) {
                sample.play = false; 
                sample.currentSampleIndex = 0; 
                break; 
                }
            }
            }
        }
    return 0; 
    }