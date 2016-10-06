/*
	This file is part of the UrgBenri application.

	Copyright (c) 2016 Mehrez Kristou.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Please contact kristou@hokuyo-aut.jp for more details.

*/

#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QTime>
#include <QTranslator>

#include "ViewerPluginInterface.h"

namespace Ui {
class TableViewerPlugin;
}

class TableViewerPlugin : public ViewerPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(ViewerPluginInterface)
    Q_PLUGIN_METADATA(IID "org.kristou.UrgBenri.TableViewerPlugin")
    PLUGIN_FACTORY(TableViewerPlugin)
public:
    explicit TableViewerPlugin(QWidget *parent = 0);
    virtual ~TableViewerPlugin();


    void setRefreshRate(int refreshRate_) {  refreshRate = refreshRate_;}

    QString pluginName() const { return tr("Table"); }
    QIcon pluginIcon() const{ return QIcon(":/TableViewerPlugin/tabIcon"); }
    QString pluginDescription() const {return tr("Show measurement data on a table");}
    PluginVersion pluginVersion() const {return PluginVersion(1, 0, 0);}
    QString pluginAuthorName() const {return "Mehrez Kristou";}
    QString pluginAuthorContact() const {return "mehrez@kristou.com";}
    int pluginLoadOrder() const {return 1;}
    bool pluginIsExperimental() const { return false; }

    QString pluginLicense() const { return "GPL"; }
    QString pluginLicenseDescription() const { return "GPL"; }

    void saveState(QSettings &settings);
    void restoreState(QSettings &settings);

    void loadTranslator(const QString &locale);

protected:
    void changeEvent(QEvent* e);

public slots:
    void addMeasurementData(const QString &id
                            , const PluginDataStructure &data);
    void refresh();
    void setSelectedStep(int step);
    void clear();

    void onLoad(PluginManagerInterface *manager);
    void onUnload();

private:
    Ui::TableViewerPlugin *ui;

    void setupConnections();
    void setupShortcuts();

    PluginDataStructure localData;

    int selectedStep;
    long currentScan;
    QTranslator m_translator;

    int refreshRate;
    QTime refreshTimer;
};

#endif // TABLEWIDGET_H

