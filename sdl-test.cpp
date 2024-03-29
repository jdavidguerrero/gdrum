#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL no pudo inicializarse. SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer no pudo inicializarse. Mix_Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    Mix_Chunk* sound = Mix_LoadWAV("resources/audio/bass.wav");
    if (!sound) {
        std::cout << "No se pudo cargar el sonido. Mix_Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    Mix_PlayChannel(0, sound, 0);
    SDL_Delay(300);
    Mix_FreeChunk(sound);
  
    SDL_Quit();

    return 0;
}
