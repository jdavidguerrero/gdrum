#include "RPI_ads1115.h"
#include <iostream>
#include <unistd.h> 
#include "pigpio.h"


int main() {

    RPI_ADS1115 ads;


    if (!ads.begin()) {
        std::cerr << "Failed to initialize ADS." << std::endl;
        return 1;
    }

    while (true){
        int16_t adc0, adc1, adc2, adc3;
        float volts0, volts1, volts2, volts3;

        adc0 = ads.readADC_SingleEnded(0);
        adc1 = ads.readADC_SingleEnded(1);
        adc2 = ads.readADC_SingleEnded(2);
        adc3 = ads.readADC_SingleEnded(3);

        volts0 = ads.computeVolts(adc0);
        volts1 = ads.computeVolts(adc1);
        volts2 = ads.computeVolts(adc2);
        volts3 = ads.computeVolts(adc3);

        std::cout << "-----------------------------------------------------------" << std::endl;
        std::cout << "AIN0: " << adc0 << "  " << volts0 << "V" << std::endl;
        std::cout << "AIN1: " << adc1 << "  " << volts1 << "V" << std::endl;
        std::cout << "AIN2: " << adc2 << "  " << volts2 << "V" << std::endl;
        std::cout << "AIN3: " << adc3 << "  " << volts3 << "V" << std::endl;

        usleep(100000); //usec time
    }

    return 0;
}