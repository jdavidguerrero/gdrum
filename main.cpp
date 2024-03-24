#include "RPI_ads1115.h"
#include "AudioModule.h"
#include <iostream>
#include <unistd.h> 
#include "pigpio.h"


int main() {

    RPI_ADS1115 ads;
    AudioModule audioModule;
    

    audioModule.loadSampleForChannel("resources/audio/bass.wav", 0);
    audioModule.loadSampleForChannel("resources/audio/tone.wav", 1);
    audioModule.loadSampleForChannel("resources/audio/slap.wav", 2);
    const int16_t threshold = 10000;    
    const int debounceDelay = 750;
    int NUM_CHANNELS = 3; // ms, ajustar según sea necesario
    std::vector<std::chrono::steady_clock::time_point> lastTriggerTimes(NUM_CHANNELS, std::chrono::steady_clock::now());

    if (!ads.begin()) {
        std::cerr << "Failed to initialize ADS." << std::endl;
        gpioTerminate(); // Clean gpio
        return 1;
    }

    while (true){
        for (int channel = 0; channel < NUM_CHANNELS; ++channel) { 
        int value = ads.readADC_SingleEnded(channel);
        //std::cout << "-----------------------------------------------------------" << std::endl;
        //std::cout << "Value: " << value << "  " << "Channel:" << channel << std::endl;
        auto now = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTriggerTimes[channel]).count();

        if (value > threshold && elapsed > debounceDelay) {
            std::cout << "Value: " << value << "  " << "Channel:" << channel << std::endl;
            audioModule.playSample(channel);
            lastTriggerTimes[channel] = now;
        }
        }
    

    }
    gpioTerminate(); 
    return 0;
}