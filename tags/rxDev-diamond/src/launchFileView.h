/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef LAUNCHFILEVIEW_H
#define LAUNCHFILEVIEW_H

#include <QResizeEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QScrollBar>
#include "math.h"
#include "tagItems/nodeItem.h"
#include <QLineEdit>
#include <QStandardItemModel>
#include <QComboBox>
#include "tagItems/parameterItem.h"


QT_BEGIN_NAMESPACE
class QMenu;
class QPointF;
class QGraphicsLineItem;

QT_END_NAMESPACE


class LaunchFileView : public QGraphicsView {
public:
    enum Mode { DragItem, InsertLine, DeleteItem };

    LaunchFileView(QWidget *parent = 0);
    QLineEdit *lineEdit_namespace;
    QLineEdit *lineEdit_name;
    NodeItem * selectedBox;
    ParameterItem * selectedParam;
    QComboBox *comboBox_sceneScale;
    QString selectedNodeName;
    QString selectedNodePackage;
    QStringList  selectedNodeSubscriptions;
    QStringList selectedNodePublications;
    QStringList selectedNodeServices;
    QStringList selectedNodeParameters;
    bool getDeprecated();
    QString getMessage();
    void setDeprecated(bool depri);
    void setMessage(QString new_message);
protected:


    void wheelEvent(QWheelEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void itemInserted(NodeItem *item);



public slots:
    void setMode(Mode mode);

private:
    Mode myMode;


    bool _handScrolling;
    QPoint _handScrollingOrigin;
    QCursor _savedCursor;
    int _handScrollingValueX;
    int _handScrollingValueY;

    QPointF startPoint;
    QGraphicsLineItem *line;

    int _width;
    int _height;
    int _space;
    bool deprecated;
    QString message;

};



#endif
