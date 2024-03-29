#include "RPI_ads1115.h"
#include "AudioModule.h"


int main() {
    RPI_ADS1115 ads;
    AudioModule audioModule;
    bool running = true;
    SDL_Event event;

    audioModule.loadSampleForChannel("resources/audio/slap.wav", 0);
    audioModule.loadSampleForChannel("resources/audio/slap.wav", 1);
    audioModule.loadSampleForChannel("resources/audio/slap.wav", 2);
    const int16_t THRESHOLD = 12000;    
    const int debounceDelay = 150;
    int NUM_CHANNELS = 3;
    std::vector<std::chrono::steady_clock::time_point> lastTriggerTimes(NUM_CHANNELS, std::chrono::steady_clock::now());

   if (!ads.begin()) {
     std::cerr << "Failed to initialize ADS." << std::endl;// Clean gpio
    return 1;
  }


   while (running){   
       
		while (SDL_PollEvent(&event) != 0) {
       if (event.type == SDL_QUIT) {
          running = false;
           SDL_Quit();
            exit(1);
       }
        } 

     for (int channel = 0; channel < NUM_CHANNELS; ++channel) { 
       int value = ads.readADC_SingleEnded(channel);
       auto now = std::chrono::steady_clock::now();
      int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTriggerTimes[channel]).count();
     if (value > THRESHOLD && elapsed > debounceDelay) {

        int volume = 0;
        if (value > 20000) {
            volume = MIX_MAX_VOLUME; // Establece el volumen al máximo si el valor es mayor que 20000
        } else if (value > THRESHOLD) {
            // Escala el valor entre THRESHOLD y 20000 a un rango entre MIX_MAX_VOLUME/4 y MIX_MAX_VOLUME
            volume = 80 + (value - THRESHOLD) * (MIX_MAX_VOLUME - 80)  / (20000 - THRESHOLD);
        }
          std::cout << "Value: " << value << "  " << "Channel:" << channel <<"  Volume: " << volume << std::endl;
          audioModule.playSample(channel , volume);
          lastTriggerTimes[channel] = now;
      }
     } 
    

   }

    audioModule.~AudioModule();
    return 0;
}