#include "RtAudio.h"
#include <iostream>

int main() {
    RtAudio dac;
    unsigned int devices = dac.getDeviceCount();
    RtAudio::DeviceInfo info;

    for (unsigned int i=0; i<devices; i++) {
        info = dac.getDeviceInfo(i);
        std::cout << "Device #" << i << ": " << info.name << '\n';
        
        // Imprimir más información sobre el dispositivo si lo deseas
        if (info.probed) { // Verifica si la información del dispositivo fue obtenida correctamente
            std::cout << "Output Channels: " << info.outputChannels << '\n';
            std::cout << "Input Channels: " << info.inputChannels << '\n';
            std::cout << "Duplex Channels: " << info.duplexChannels << '\n';
            std::cout << "Is Default Output: " << (info.isDefaultOutput ? "Yes" : "No") << '\n';
            std::cout << "Is Default Input: " << (info.isDefaultInput ? "Yes" : "No") << '\n';
            std::cout << "-----------------------------------\n";
        }
    }
    return 0;
}
