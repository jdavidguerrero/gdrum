#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMap>
#include "sensors/RPI_ads1115.h"
#include <chrono>

class Worker : public QObject {
    Q_OBJECT
public:
    explicit Worker(RPI_ADS1115* ads = nullptr, QObject *parent = nullptr);
    void setChannel(int channel);
    void setThreshold(int threshold);
    void setDebounceDelay(int delayMs);

public slots:
    void process(); // Método para iniciar el procesamiento (lectura del sensor)
    void stop(); // Método para detener el procesamiento

signals:
    void finished(); // Señal para indicar que el trabajador ha terminado
    void error(QString err); // Señal para indicar un error
    void thresholdExceeded(int channel); // Señal para cuando se excede el umbral

private:
    RPI_ADS1115* m_ads;
    int m_channel;
    int m_threshold;
    bool m_running; 
    std::chrono::steady_clock::time_point lastTriggerTime;
    int m_debounceDelay; 
};

#endif // WORKER_H
