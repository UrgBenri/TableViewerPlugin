!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    QT += core gui

    DEPENDPATH += $$PWD
    INCLUDEPATH += $$PWD

    SOURCES += \
            $$PWD/UrgTableDrawWidget.cpp \
            $$PWD/UrgTableModel.cpp

    HEADERS  += \
            $$PWD/UrgTableDrawWidget.h \
            $$PWD/UrgTableModel.h
}
