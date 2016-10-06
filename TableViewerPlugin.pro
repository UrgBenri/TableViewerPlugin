TEMPLATE    = lib
CONFIG      += plugin
DESTDIR     = $$PWD/../bin/plugins
CONFIG      += c++11
TARGET      = TableViewerPlugin

!include(TableViewerPlugin.pri) {
        error("Unable to include Table Viewer Plugin.")
}

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
