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
    const int16_t threshold = 6000;    

    if (!ads.begin()) {
        std::cerr << "Failed to initialize ADS." << std::endl;
        gpioTerminate(); // Clean gpio
        return 1;
    }

    while (true){
        int NUM_CHANNELS = 3;
        for (int channel = 0; channel < NUM_CHANNELS; ++channel) { 
        int value = ads.readADC_SingleEnded(channel);
        //std::cout << "-----------------------------------------------------------" << std::endl;
        //std::cout << "Value: " << value << "  " << "Channel:" << channel << std::endl;
        if (value > threshold) {
            std::cout << "Value: " << value << "  " << "Channel:" << channel << std::endl;
            audioModule.playSample(channel);
        }
        }

    }

    gpioTerminate(); 
    return 0;
}