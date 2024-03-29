// AudioModule.cpp
#include "AudioModule.h"


AudioModule::AudioModule() {

    samples.resize(8);
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }
    int numAudioDevices = SDL_GetNumAudioDevices(0); // 0 para dispositivos de salida
    for (int i = 0; i < numAudioDevices; ++i) {
        const char* deviceName = SDL_GetAudioDeviceName(i, 0); // 0 para dispositivos de salida
        std::cout << "Audio device " << i << ": " << deviceName << std::endl;
    }
    if (Mix_AllocateChannels(16) < 0) {
        std::cout << "Cant allocate channels. Mix_Error: " << Mix_GetError() << std::endl;
    }
    std::cout<<"SDL Initialized"<< std::endl;
}

AudioModule::~AudioModule() {
    Mix_CloseAudio();
    SDL_Quit();
}




void AudioModule::loadSampleForChannel( const std::string& filename, size_t channel) {
    samples[channel].sampleRoute = filename;
    std::cout<< "Assing file: " << filename << std::endl;;
}

void AudioModule::playSample(size_t channel, int volume) {
        Mix_Chunk* sound = Mix_LoadWAV(samples[channel].sampleRoute.c_str());
            if (!sound) {
                std::cout << "No se pudo cargar el sonido. Mix_Error: " << Mix_GetError() << std::endl;
            }
        Mix_Volume(channel,volume);
        Mix_PlayChannel(channel, sound, 0);
        SDL_Delay(100);
        Mix_FreeChunk(sound);
}

