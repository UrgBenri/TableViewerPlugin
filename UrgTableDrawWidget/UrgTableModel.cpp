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

#include "UrgTableModel.h"

#include <QDebug>
#include <QColor>
#include <QBrush>

#define COLUMNS_COUNT 11

UrgTableModel::UrgTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_selectedStep(-1)
{
}

UrgTableModel::~UrgTableModel()
{

}

int UrgTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_localData.ranges.size();
}

int UrgTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return COLUMNS_COUNT;
}

QVariant UrgTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    switch(role){
    case Qt::DisplayRole:{
        if(col == 0){
            if(m_localData.converter){
                return m_localData.converter.data()->index2Step(row);
            }
            else{
                return row;
            }
        }
        else if((col % 2) == 1){
            int index = (col -1)/ 2;
            if(row < m_localData.ranges.size() && index < m_localData.ranges[row].size()){
                return (qlonglong)m_localData.ranges[row][index];
            }
            else{
                return QString("");
            }
        }else{
            int index = (col -2)/ 2;
            if(row < m_localData.levels.size() && index < m_localData.levels[row].size()){
                return (qlonglong)m_localData.levels[row][index];
            }
            else{
                return QString("");
            }
        }
    }break;
    case Qt::BackgroundRole:{
        if (row == m_selectedStep){
            QBrush cellBackground(QColor(Qt::magenta).lighter(180));
            return cellBackground;
        }
    }break;
    }
    return QVariant();
}

QVariant UrgTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {

            if(section == 0){
                return tr("Step");
            }
            else if((section % 2) == 1){
                int index = (section -1)/ 2;
                return tr("Range %1").arg(index);
            }else{
                int index = (section -2)/ 2;
                return tr("Level %1").arg(index);
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags UrgTableModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

void UrgTableModel::setTableData(const PluginDataStructure &data)
{
    bool reset = m_localData.ranges.size() != data.ranges.size();
    if(reset){
        beginResetModel();
    }
    m_localData = data;
    if(reset) {
        endResetModel();
    }
    else{
        emit dataChanged(createIndex(0, 0)
                         , createIndex(rowCount(), COLUMNS_COUNT -1)
                         , QVector<int>() <<Qt::DisplayRole);
    }
}

void UrgTableModel::setSelectedStep(int step)
{
    if(m_localData.converter){
        m_selectedStep = m_localData.converter.data()->step2Index(step);
        emit dataChanged(createIndex(0, 0)
                         , createIndex(rowCount(), COLUMNS_COUNT -1)
                         , QVector<int>() <<Qt::BackgroundRole);
    }
}

QModelIndex UrgTableModel::step2index(int step)
{
    if(m_localData.converter){
        return index(m_localData.converter.data()->step2Index(step), 0);
    }
    else {
        return QModelIndex();
    }
}

int UrgTableModel::row2step(int row)
{
    if(m_localData.converter){
        return m_localData.converter.data()->index2Step(row);
    }
    else{
        return -1;
    }
}

