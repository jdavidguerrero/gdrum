#include "sensors/ChannelRead.h"
#include "sensors/Worker.h"
#include <QThread>
#include <iostream>
#include <chrono>
#include <vector>


ChannelRead::ChannelRead(QObject *parent) : QObject(parent), m_ads(nullptr), m_audioModule(nullptr) {}

void ChannelRead::setup(RPI_ADS1115 *ads, AudioModule *audioModule) {
    m_ads = ads;
    m_audioModule = audioModule;
    std::cout << "Init Sample Mode" << std::endl;
    if (!m_ads || !m_audioModule) {
        std::cerr << "ADS or AudioModule not set." << std::endl;
        return;
    }
}


/*
void Read::readChannel(int deviceIndex, int channel) {
    RPI_ADS1115* ads = (deviceIndex == 0) ? m_ads1 : m_ads2;
    
    while (m_running) {
        int value = ads->readADC_SingleEnded(channel);

        if (value > threshold) { // Define un valor adecuado para 'threshold'
            emit thresholdExceeded(channel); // Opcional: emite una señal para la UI
            m_audioModule->playSample(channel);
        }
        
        QThread::msleep(10); // Evita sobrecargar el CPU
    }
}

void Read::startReading() {
    m_running = true;
    int totalChannels = 6; // 6 canales en total, ajusta según necesites

    for (int i = 0; i < totalChannels; ++i) {
        QThread* thread = new QThread();
        int deviceIndex = (i < 4) ? 0 : 1; // Los primeros 4 canales en el dispositivo 0, los siguientes en el dispositivo 1
        int channel = (i < 4) ? i : i - 4; // Ajusta el número de canal para el segundo dispositivo

        // Crea un trabajador y muévelo al thread
        Worker* worker = new Worker(); // Worker necesita ser implementado para manejar la lectura de un canal
        worker->moveToThread(thread);
        
        connect(thread, &QThread::started, [worker, deviceIndex, channel]() {
            worker->readChannel(deviceIndex, channel);
        });
        connect(this, &Read::stopReadingSignal, worker, &Worker::stopReading, Qt::DirectConnection); // Asegura una parada segura
        connect(thread, &QThread::finished, worker, &QObject::deleteLater);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);

        m_threads.append(thread);
        thread->start();
    }
}

*/


void ChannelRead::startReading() {

    m_running = true;
    int totalChannels = 3;
    for (int i = 0; i < totalChannels; ++i) {
        QThread* thread = new QThread();
        int channel = i; // Define el canal aquí para que pueda ser capturado por la lambda
        
        // Asume que m_ads es una instancia válida de RPI_ADS1115 y está adecuadamente inicializada
        Worker* worker = new Worker(m_ads);
        worker->setChannel(channel);
        worker->setThreshold(12000);
        worker->setDebounceDelay(150);
        worker->moveToThread(thread);
        
        connect(thread, &QThread::started, worker, &Worker::process);
        connect(this, &ChannelRead::stopReadingSignal, worker, &Worker::stop, Qt::DirectConnection);
        connect(worker, &Worker::finished, thread, &QThread::quit);
        connect(worker, &Worker::finished, worker, &QObject::deleteLater);
        connect(worker, &Worker::thresholdExceeded, this, &ChannelRead::handleThresholdExceeded);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);
        


        m_threads.append(thread);
        thread->start();

    }

}

void ChannelRead::handleThresholdExceeded(int channel) {
    m_audioModule->playSample(channel); // Aquí decides qué sample reproducir basado en el canal.
    emit thresholdExceeded(channel);
}

void ChannelRead::setVolume(int channel, int volumeLevel) {
    if (m_audioModule) {
        m_audioModule->setVolumeChannel(channel, volumeLevel);
    }
}


void ChannelRead::stopReading() {
    emit stopReadingSignal(); // Detiene todos los workers
    m_running = false;
    for (auto thread : qAsConst(m_threads)) {
        thread->quit();
        thread->wait();
    }
    m_threads.clear();
}

