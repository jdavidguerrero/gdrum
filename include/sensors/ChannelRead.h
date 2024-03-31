#ifndef CHANNELREAD_H
#define CHANNELREAD_H

#include <QObject>
#include <QMap>
#include "audio/AudioModule.h"
#include "sensors/RPI_ads1115.h"


class ChannelRead : public QObject {
    Q_OBJECT
public:
    explicit ChannelRead(QObject *parent = nullptr);
    void setup(RPI_ADS1115 *ads, AudioModule *audioModule);
    //void setup(RPI_ADS1115 *ads1, RPI_ADS1115 *ads2, AudioModule *audioModule);
    Q_INVOKABLE void startReading();
    Q_INVOKABLE void stopReading();
    void setThresholdForChannel(int channel, int threshold);


public slots:
    void handleThresholdExceeded(int channel);
     void setVolume(int channel, int volumeLevel);
    
signals:
    void stopReadingSignal();
    void thresholdExceeded(int channel);


   



private:
    RPI_ADS1115 *m_ads;
    //RPI_ADS1115 *m_ads2;
    AudioModule *m_audioModule;
    QList<QThread*> m_threads;
    std::atomic<bool> m_running{false};
    QMap<int, int> m_thresholds;
};

#endif // CHANNELREAD_H
