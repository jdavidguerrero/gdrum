#include <QThread>
#include "sensors/Worker.h"
#include "sensors/RPI_ads1115.h" // Incluye el archivo de encabezado de tu clase RPI_ADS1115
#include <iostream>

Worker::Worker(RPI_ADS1115* ads, QObject *parent) : QObject(parent), m_ads(ads) {}
    

void Worker::setChannel(int channel) {
    m_channel = channel;
}

void Worker::setThreshold(int threshold) {
    m_threshold = threshold;
}

void Worker::setDebounceDelay(int delayMs) {
    m_debounceDelay = delayMs;
}

void Worker::process() {
    if (!m_ads) {
        emit error("ADS1115 not initialized");
        return;
    }

    m_running = true;
    while (m_running) {
        int value = m_ads->readADC_SingleEnded(m_channel);
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTriggerTime).count();
        if (value > m_threshold && elapsed > m_debounceDelay) {
            std::cout << "Channel: " << m_channel << " Value: " << value << std::endl;
            emit thresholdExceeded(m_channel); // Opcionalmente emitir si se va a manejar fuera
            lastTriggerTime = now;
        }
        QThread::msleep(1); // Evita la sobrecarga del CPU
    }

    emit finished();
}


void Worker::stop() {
    m_running = false;
}
