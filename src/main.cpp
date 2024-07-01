#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <iostream>
#include "audio/AudioModule.h"
#include "sensors/RPI_ads1115.h"
#include "sensors/ChannelRead.h" // Asegúrate de que el nombre del archivo sea correcto

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    auto *ads1 = new RPI_ADS1115(); // Primer dispositivo ADS1115
    // Si estás usando dos dispositivos ADS1115, inicializa el segundo aquí también
    // auto *ads2 = new RPI_ADS1115();
    // Configura las direcciones I2C si es necesario, por ejemplo, ads2->begin(0x49);
    
    if (!ads1->begin(0x48)) { // Asume 0x48 como dirección predeterminada
        std::cerr << "Failed to initialize ADS1115." << std::endl;
    }
    
    auto *audioModule = new AudioModule();

    // Carga los samples para los canales
    audioModule->loadSampleForChannel("resources/audio/slap.wav", 0);
    audioModule->loadSampleForChannel("resources/audio/tone.wav", 1);
    audioModule->loadSampleForChannel("resources/audio/slap.wav", 2);

   

    // Si Read necesita manejar múltiples ADS1115, asegúrate de que su método setup() los acepte
    ChannelRead *readSensors = new ChannelRead();
    readSensors->setup(ads1, /* ads2, */ audioModule);
    readSensors->startReading();

    engine.rootContext()->setContextProperty("readSensors", readSensors);
    

    engine.load(QUrl(QStringLiteral("qrc:/src/gui/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

        // Conecta la señal aboutToQuit de QCoreApplication para limpiar adecuadamente antes de salir.
    QObject::connect(&app, &QCoreApplication::aboutToQuit, readSensors, &ChannelRead::stopReading);

    // Limpieza al cerrar la aplicación.
    QObject::connect(&app, &QGuiApplication::aboutToQuit, [&]() {
        delete ads1;
        // delete ads2; // Si estás usando un segundo ADS1115
        delete audioModule;
        delete readSensors;
    });


    return app.exec();
}
