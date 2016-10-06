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

#include "TableViewerPlugin.h"
#include "ui_TableViewerPlugin.h"

TableViewerPlugin::TableViewerPlugin(QWidget *parent)
    : ViewerPluginInterface(parent)
    , ui(new Ui::TableViewerPlugin)
    , currentScan(0)
    , refreshRate(100)
{
    ui->setupUi(this);
    ui->tableDrawWidget->clear();

    setupConnections();
    setupShortcuts();

    selectedStep = -1;

    refreshTimer.start();

    ui->tableRefreshAction->setVisible(false);
}

TableViewerPlugin::~TableViewerPlugin()
{
    delete ui;
}

void TableViewerPlugin::changeEvent(QEvent* e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        if(ui) ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TableViewerPlugin::refresh()
{
    ui->tableDrawWidget->setMeasurementData(localData);

    QString stepInfo = tr("Time: <b>%1</b>")
                       .arg(QString::number(localData.timestamp), 8, '0');
    ui->stepInfo->setText(stepInfo);
}

void TableViewerPlugin::setSelectedStep(int step)
{
    ui->tableDrawWidget->setSelectedStep(step);
}

void TableViewerPlugin::setupConnections()
{
    connect(ui->tableClear, &QAbstractButton::clicked,
            ui->tableDrawWidget, &UrgTableDrawWidget::clear);

    connect(ui->tableCopy, &QAbstractButton::clicked,
            ui->tableDrawWidget, &UrgTableDrawWidget::copy);

    connect(ui->tableDrawWidget, &UrgTableDrawWidget::selectedStepChanged,
            this, &ViewerPluginInterface::selectedStepChanged);
}

void TableViewerPlugin::setupShortcuts()
{
//    (void) new QShortcut(QKeySequence(Qt::ALT + Qt::Key_O), this, SLOT(showOrthoView()));
//    (void) new QShortcut(QKeySequence(Qt::ALT + Qt::Key_T), this, SLOT(showTableView()));
//    (void) new QShortcut(QKeySequence(Qt::ALT + Qt::Key_G), this, SLOT(toggleGrid()));
}

void TableViewerPlugin::clear()
{
    localData.ranges.clear();
    localData.levels.clear();
    ui->tableDrawWidget->clear();
    ui->stepInfo->setText("");
}

void TableViewerPlugin::onLoad(PluginManagerInterface *manager)
{

}

void TableViewerPlugin::onUnload()
{

}

void TableViewerPlugin::addMeasurementData(const QString &id, const PluginDataStructure &data)
{
    //TODO: investigate this, memory leak!!!!
    localData = data;

    if (refreshTimer.elapsed() > refreshRate) {
        refreshTimer.restart();
        refresh();
    }
}

void TableViewerPlugin::saveState(QSettings &settings)
{

}

void TableViewerPlugin::restoreState(QSettings &settings)
{

}

void TableViewerPlugin::loadTranslator(const QString &locale)
{
    qApp->removeTranslator(&m_translator);
    if (m_translator.load(QString("plugin.%1").arg(locale), ":/TableViewerPlugin")) {
        qApp->installTranslator(&m_translator);
    }
}
