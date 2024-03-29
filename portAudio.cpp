#include <iostream>
#include <vector>
#include <sndfile.h>
#include "portaudio.h"

// Función para cargar el archivo de audio utilizando libsndfile
std::vector<float> loadAudioFile(const std::string& filename) {
    SF_INFO sfinfo;
    SNDFILE* infile = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    if (!infile) {
        std::cerr << "Can't open file: " << filename << std::endl;
        return {};
    }

    std::vector<float> buffer(sfinfo.frames * sfinfo.channels);
    sf_count_t readcount = sf_read_float(infile, buffer.data(), buffer.size());

    sf_close(infile);

    if (readcount < static_cast<sf_count_t>(buffer.size())) {
        std::cerr << "File read incomplete" << std::endl;
        return {};
    }

    return buffer;
}

// Callback de PortAudio
static int paCallback(const void* inputBuffer, void* outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void* userData) {
    auto* out = static_cast<float*>(outputBuffer);
    std::cout << "Callback "<< std::endl;

    auto* data = static_cast<std::vector<float>*>(userData);
    static size_t frameCounter = 0;

    for (unsigned int i = 0; i < framesPerBuffer; ++i) {
        if (frameCounter < data->size()) {
            *out++ = (*data)[frameCounter++]; // Left channel
            if (frameCounter < data->size()) *out++ = (*data)[frameCounter++]; // Right channel
        } else {
            *out++ = 0;
            *out++ = 0;
        }
    }

    return paContinue;
}

int main() {
    std::vector<float> audioData = loadAudioFile("resources/audio/slap.wav");

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    PaStream* stream;
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cerr << "No default output device" << std::endl;
        return 1;
    }
    outputParameters.channelCount = 2; // Estéreo
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;

    err = Pa_OpenStream(&stream, nullptr, &outputParameters, 44100, 256, paClipOff, paCallback, &audioData);
    if (err != paNoError) {
        std::cerr << "PortAudio error: open stream: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: start stream: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    std::cout << "Presiona Enter para salir..." << std::endl;
    std::cin.get();

    if (stream) {
        Pa_CloseStream(stream);
    }
    Pa_Terminate();
}