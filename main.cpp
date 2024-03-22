#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "ads1115rpi.h"
#include <unistd.h>
#include <csignal>
#include <cstdlib> 
#include <pigpio.h>
#include <chrono>
#include <thread>
#include <signal.h>
#include <atomic>

// Variable global para mantener el estado de ejecución
std::atomic<bool> keepRunning(true);

// Manejador de señal para detener la ejecución con Ctrl+C
void signalHandler(int signum) {
    keepRunning.store(false);
}


class ADS1115Reader : public ADS1115rpi {
public:
    // Implementación de hasSample para imprimir el valor del ADC
    virtual void hasSample(float voltage,float read, ADS1115settings::Input channel) {
        std::cout << "Valor ADC en canal " << channel << ": " << voltage << " V" "---"  << read << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
};

int main(int argc, char *argv[]) {

    signal(SIGINT, signalHandler);

    ADS1115settings settings;
    settings.samplingRate = ADS1115settings::FS64HZ;
    settings.pgaGain = ADS1115settings::FSR4_096;
    settings.channel = ADS1115settings::AIN0;
    ADS1115Reader adc;
    
     try {
        adc.start(settings);
        std::cout << "Iniciando lecturas. Presiona Ctrl+C para detener..." << std::endl;

        while (keepRunning) {

            //adc.readAllChannels();
            for (int channel = ADS1115settings::AIN0; channel <= ADS1115settings::AIN3; ++channel) {
            if (!keepRunning) break; // Comprueba si se ha solicitado detener el bucle.

            adc.setChannel(static_cast<ADS1115settings::Input>(channel)); // Cambia al canal actual.
            std::this_thread::sleep_for(std::chrono::milliseconds(15)); // Espera un poco para la estabilización y conversión.
            
            // En este punto, dataReady debería ser llamado automáticamente en respuesta a la señal DRDY,
            // lo que a su vez debería invocar hasSample para procesar los datos.
            // Asegúrate de que la lógica en dataReady y hasSample esté correctamente implementada para manejar esto.
        }

         

        }
  
        adc.stop(); 
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        adc.stop(); // Asegúrate de detener y limpiar adecuadamente en caso de error
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
