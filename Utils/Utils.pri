INCLUDEPATH += $$PWD

win32 {
    msvc {
        LIBS += Advapi32.lib
    } else {
        LIBS += -lAdvapi32
    }
}

HEADERS += \
    $$PWD/utils.h

SOURCES += \
    $$PWD/utils.cpp
