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

#include "UrgTableDrawWidget.h"

//#include "stringUtils.h"
#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QMenu>
#include <QMutexLocker>
#include <iostream>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>

UrgTableDrawWidget::UrgTableDrawWidget(QWidget* parent) :
    QTableView(parent)
{
    setModel(&m_model);

    QAction* copyAction = new QAction(tr("Copy"), this);
    connect(copyAction, &QAction::triggered, this, &UrgTableDrawWidget::copy);
//    QAction* pasteAction = new QAction(tr("Paste"), this);
//    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
    setContextMenuPolicy(Qt::ActionsContextMenu);
    addAction(copyAction);
    //    addAction( pasteAction );
}

UrgTableDrawWidget::~UrgTableDrawWidget()
{
}

int UrgTableDrawWidget::columnCount()
{
    return m_model.columnCount();
}


void UrgTableDrawWidget::setMeasurementData(const PluginDataStructure &data)
{
        m_model.setTableData(data);
}

void UrgTableDrawWidget::clear()
{
    m_model.setTableData(PluginDataStructure());
}

void UrgTableDrawWidget::setSelectedStep(int step)
{
    m_model.setSelectedStep(step);
    scrollTo(m_model.step2index(step));
}

void UrgTableDrawWidget::copy()
{
    QItemSelectionModel* selection = selectionModel();
    QModelIndexList indexes = selection->selectedIndexes();

    if (indexes.size() < 1) {
        return;
    }

    // QModelIndex::operator < sorts first by row, then by column.
    // this is what we need
    std::sort(indexes.begin(), indexes.end());

    // You need a pair of indexes to find the row changes
    QModelIndex previous = indexes.first();
    indexes.removeFirst();
    QString selected_text;
    QModelIndex current = previous;
    Q_FOREACH(current, indexes) {
        QVariant data = model()->data(previous);
        QString text = data.toString();
        // At this point `text` contains the text in one cell
        selected_text.append(text);
        // If you are at the start of the row the row number of the previous index
        // isn't the same.  Text is followed by a row separator, which is a newline.
        if (current.row() != previous.row()) {
            selected_text.append(QLatin1Char('\n'));
        }
        // Otherwise it's the same row, so append a column separator, which is a tab.
        else {
            selected_text.append(QLatin1Char('\t'));
        }
        previous = current;
    }

    // add last element
    selected_text.append(model()->data(current).toString());
    selected_text.append(QLatin1Char('\n'));
    qApp->clipboard()->setText(selected_text);

    QMessageBox::information(this, QApplication::applicationName(),
                         tr("Selected values were copied to your system clipboard.\n"
                            "You can paste them in any other application."));
}

//void UrgTableDrawWidget::paste()
//{
//    QString selected_text = qApp->clipboard()->text();
//    QStringList cells = selected_text.split(QRegExp(QLatin1String("\\n|\\t")));
//    while (!cells.isEmpty() && cells.back().size() == 0) {
//        cells.pop_back(); // strip empty trailing tokens
//    }
//    int rows = selected_text.count(QLatin1Char('\n'));
//    int cols = cells.size() / rows;
//    if (cells.size() % rows != 0) {
//        // error, uneven number of columns, probably bad data
//        emit error(QApplication::applicationName(),
//                             tr("Invalid clipboard data, unable to perform paste operation."));
//        return;
//    }

//    if (cols != columnCount()) {
//        // error, clipboard does not match current number of columns
//        emit error(QApplication::applicationName(),
//                             tr("Invalid clipboard data, incorrect number of columns."));
//        return;
//    }

//    // don't clear the grid, we want to keep any existing headers
//    setRowCount(rows);
//    // setColumnCount(cols);
//    int cell = 0;
//    for (int row = 0; row < rows; ++row) {
//        for (int col = 0; col < cols; ++col, ++cell) {
//            QTableWidgetItem* newItem = new QTableWidgetItem(cells[cell]);
//            setItem(row, col, newItem);
//        }
//    }
//}

void UrgTableDrawWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->matches(QKeySequence::Copy)) {
        copy();
    }
    //  else if(event->matches(QKeySequence::Paste) )
    //  {
    //    paste();
    //  }
    else {
        QTableView::keyPressEvent(event);
    }
}

void UrgTableDrawWidget::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        break;
    default:
        break;
    }
}

void UrgTableDrawWidget::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    QTableView::currentChanged(current, previous);
    int step = m_model.row2step(current.row());
    m_model.setSelectedStep(step);
    emit selectedStepChanged(step);
}

