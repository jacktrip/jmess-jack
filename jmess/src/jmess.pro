#******************************
# Created by Juan-Pablo Caceres
#******************************

CONFIG += qt thread debug_and_release build_all
CONFIG(debug, debug|release) {
  TARGET = jmess_debug
  } else {
  TARGET = jmess
  }
QT -= gui
QT += xml

!win32 {
  INCLUDEPATH+=/usr/local/include
  LIBS += -L/usr/local/lib -ljack -lm
}

macx {
  message(MAC OS X)
  CONFIG -= app_bundle
  CONFIG += x86
  DEFINES += __MAC_OSX__
  }
linux-g++ {
  message(Linux)
  LIBS += -lasound
  QMAKE_CXXFLAGS += -D__LINUX_ALSA__ #-D__LINUX_OSS__ #RtAudio Flags
  QMAKE_CXXFLAGS += -g -O2
  DEFINES += __LINUX__
  }
linux-g++-64 {
  message(Linux 64bit)
  LIBS += -lasound
  QMAKE_CXXFLAGS += -fPIC -D__LINUX_ALSA__ #-D__LINUX_OSS__ #RtAudio Flags
  QMAKE_CXXFLAGS += -g -O2
  DEFINES += __LINUX__
  }

DESTDIR = .
QMAKE_CLEAN += -r ./jmess ./jmess_debug ./release ./debug
target.path = /usr/bin
INSTALLS += target


# Input
HEADERS += JMess.h
SOURCES += JMess.cpp \
           jmess_main.cpp

