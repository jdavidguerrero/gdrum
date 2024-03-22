#include "RPI_ads1115.h"
#include <iostream>
#include <unistd.h> 
#include "pigpio.h"


int main() {

    RPI_ADS1115 ads;
    const int16_t threshold0 = 6000; 
    const int16_t threshold1 = 6000;
    const int16_t threshold2 = 6000;


    if (!ads.begin()) {
        std::cerr << "Failed to initialize ADS." << std::endl;
        gpioTerminate(); // Clean gpio
        return 1;
    }

    while (true){
        int16_t adc0, adc1, adc2, adc3;
        float volts0, volts1, volts2, volts3;

        adc0 = ads.readADC_SingleEnded(0);
        adc1 = ads.readADC_SingleEnded(1);
        adc2 = ads.readADC_SingleEnded(2);
        //adc3 = ads.readADC_SingleEnded(3);

        volts0 = ads.computeVolts(adc0);
        volts1 = ads.computeVolts(adc1);
        volts2 = ads.computeVolts(adc2);
        //volts3 = ads.computeVolts(adc3);

        std::cout << "-----------------------------------------------------------" << std::endl;
        std::cout << "AIN0: " << adc0 << "  " << volts0 << "V" << std::endl;
        std::cout << "AIN1: " << adc1 << "  " << volts1 << "V" << std::endl;
        std::cout << "AIN2: " << adc2 << "  " << volts2 << "V" << std::endl;
        //std::cout << "AIN3: " << adc3 << "  " << volts3 << "V" << std::endl;

         if (adc0 > threshold0) {
        std::cout << "Umbral superado en AIN0" << std::endl;
        // Aquí colocarías la lógica para reproducir un sonido u otra acción
        }
        if (adc1 > threshold1) {
            std::cout << "Umbral superado en AIN1" << std::endl;
            // Acción para AIN1
        }
        if (adc2 > threshold2) {
            std::cout << "Umbral superado en AIN2" << std::endl;
            // Acción para AIN2
        }

        usleep(100000); //usec time
    }

    gpioTerminate(); 
    return 0;
}