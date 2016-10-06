!contains( included_modules, $$PWD ) {
    included_modules += $$PWD
    QT += core gui

    !include($$PWD/PluginInterface/UrgBenriPluginInterface.pri) {
            error("Unable to include Viewer Plugin Interface.")
    }

    !include($$PWD/UrgTableDrawWidget/UrgTableDrawWidget.pri) {
            error("Unable to include Table Draw Widget.")
    }

    DEPENDPATH += $$PWD
    INCLUDEPATH += $$PWD

    HEADERS += \
        $$PWD/TableViewerPlugin.h

    SOURCES += \
        $$PWD/TableViewerPlugin.cpp

    FORMS += \
        $$PWD/TableViewerPlugin.ui

    RESOURCES += \
        $$PWD/TableViewerPlugin.qrc

    TRANSLATIONS = $$PWD/i18n/plugin_fr.ts \
            $$PWD/i18n/plugin_en.ts \
            $$PWD/i18n/plugin_ja.ts

}

