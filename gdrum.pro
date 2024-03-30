QT += gui core quick quickcontrols2
CONFIG += c++11
TARGET = gdrum
DEFINES += QT_DEPRECATED_WARNINGS

# Configuración de los directorios de salida
DESTDIR = output
OBJECTS_DIR = output/obj
MOC_DIR = output/moc
RCC_DIR = output/rcc
UI_DIR = output/ui

# Configuración de los archivos fuente y de cabecera
SOURCES += \
    src/main.cpp \
    src/sensors/RPI_ads1115.cpp \
    src/audio/AudioModule.cpp

HEADERS += \
    include/sensors/RPI_ads1115.h \
    include/audio/AudioModule.h

# Configuración de los recursos
RESOURCES += \
    qml.qrc

# Configuración de bibliotecas adicionales
LIBS += -lwiringPi -lrt -lrtaudio -lsndfile

# Configuración de rutas de inclusión
INCLUDEPATH += $$PWD/include

# Rutas de importación adicionales para QML
QML_IMPORT_PATH =

# Rutas de importación para el Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =
