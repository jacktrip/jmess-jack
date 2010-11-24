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
  LIBS += -ljack -lm
}

macx {
  message(MAC OS X)
  QMAKE_CXXFLAGS += -D__MACOSX_CORE__ #-D__UNIX_JACK__ #RtAudio Flags
  CONFIG -= app_bundle
  CONFIG += x86
  #LIBS += -framework CoreAudio -framework CoreFoundation
  LIBS += -framework CoreFoundation
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
HEADERS += JMess.h \
           anyoption.h
SOURCES += JMess.cpp \
           anyoption.cpp \
           jmess_main.cpp

