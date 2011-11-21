/****************************************************************************
**
** Original Code:
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

#include <QtGui>

#include "remapArrow.h"
#include "remapArrowEdit.h"
#include <math.h>
#include "groupItem.h"

const qreal Pi = 3.14;
const qreal nodeDimension = 100; // The Size of the NodeItems

//! [0]
RemapArrow::RemapArrow(NodeItem *startItem, NodeItem *endItem,
                       QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene),
      offset(0)
{
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setZValue(-1);
    title.setHtml("<center><font size=\"-2\" color=\"blue\">TEXT</center>");
    title.setParentItem(this);
    title.setDefaultTextColor(Qt::blue);
    incl.setHtml("<center><font size=\"-2\" color=\"red\">incl</center>");
    incl.setParentItem(this);

}
//! [0]

//! [1]
QRectF RemapArrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;
    //qDebug()<<"Arrow"<<line();
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath RemapArrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}
//! [2]

//! [3]
void RemapArrow::updatePosition()
{
    switch ( myStartItem->arrows.indexOf(this) )        //creates a position offset if there is more than one arrow for a node
    {
    case 0:
        offset=0;
        break;
    case 1:
        offset=15;
        break;
    case 2:
        offset=-15;
        break;
    case 3:
        offset=30;
        break;
    case 4:
        offset=-30;
        break;
    case 5:
        offset=45;
        break;
    case 6:
        offset=-45;
        break;
    default:
        offset=0;
    }
    QPoint inclPos;
    QPoint startParent(0,0);
    QPoint endParent(0,0);
    if (myStartItem->parentItem()){
        startParent.setX(myStartItem->parentItem()->pos().x());
        startParent.setY(myStartItem->parentItem()->pos().y());
    }
    if (myEndItem->parentItem()){
        endParent.setX(myEndItem->parentItem()->pos().x());
        endParent.setY(myEndItem->parentItem()->pos().y());
    }

    QLineF line(startParent+myStartItem->pos()+QPoint(nodeDimension/2+offset, nodeDimension/2+offset),
                (endParent+myEndItem->pos()+QPoint(nodeDimension/2+offset, nodeDimension/2+offset)));
    setLine(line);
    if (line.p2().x()-line.p1().x()>0){
        textPos.setX(line.p1().x()+nodeDimension/2+5-offset);
        inclPos.setX(line.p1().x()+nodeDimension/2-5-offset);
    }else{
        textPos.setX(line.p2().x()+nodeDimension/2+55-offset);
        inclPos.setX(line.p1().x()-nodeDimension/2-15-offset);
    }

    if (line.p2().y()-line.p1().y()>0){
        textPos.setY(line.p2().y()+(line.p1().y()-line.p2().y())/2);
        inclPos.setY(line.p1().y()+nodeDimension/2-5-offset);
    }else{
        textPos.setY(line.p1().y()+(line.p2().y()-line.p1().y())/2);
        inclPos.setY(line.p1().y()-nodeDimension/2-15-offset);
    }
    incl.setPos(inclPos);
    title.setPos(textPos);
    title.setHtml("<center><font size=\"-2\" color=\"blue\">"+getTo()+"</center>");
    //incl.setPos((centerLine.p2().x()-centerLine.p1().x()).length(),centerLine.p1().y()-20);

}
//! [3]

//! [4]
void RemapArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                       QWidget *)
{
    if (myStartItem->collidesWithItem(myEndItem))
        return;

//    qDebug()<<offset;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 10;
    painter->setPen(myPen);
    painter->setBrush(myColor);
    //! [4] //! [5]
    QPoint startParent(0,0);
    QPoint endParent(0,0);
    if (myStartItem->parentItem()){
        startParent.setX(myStartItem->parentItem()->pos().x());
        startParent.setY(myStartItem->parentItem()->pos().y());
    }
    if (myEndItem->parentItem()){
        endParent.setX(myEndItem->parentItem()->pos().x());
        endParent.setY(myEndItem->parentItem()->pos().y());
    }
    QLineF centerLine(startParent+myStartItem->pos()+QPoint(nodeDimension/2+offset,nodeDimension/2+offset),
                      endParent+myEndItem->pos()+QPoint(nodeDimension/2+offset,nodeDimension/2+offset));

    QPolygonF endPolygon;
    QPointF p1, p2, intersectPoint;
    QLineF polyLine;
    if (_subToPub){
        endPolygon = myEndItem->polygon();
        p1 = endPolygon.first() + endParent+myEndItem->pos();
        for (int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i) + endParent+myEndItem->pos();
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =
                    polyLine.intersect(centerLine, &intersectPoint);
            if (intersectType == QLineF::BoundedIntersection)
                break;
            p1 = p2;
        }
        setLine(QLineF(intersectPoint, startParent+myStartItem->pos()+QPoint(nodeDimension/2+offset,nodeDimension/2+offset)));
        double angle = ::acos(line().dx() / line().length());
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                cos(angle + Pi - Pi / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;

    } else{
        endPolygon = myStartItem->polygon();
        p1 = endPolygon.first() + startParent+myStartItem->pos();
        for (int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i) + startParent+myStartItem->pos();
            polyLine = QLineF(p2, p1);
            QLineF::IntersectType intersectType =
                    polyLine.intersect(centerLine, &intersectPoint);
            if (intersectType == QLineF::BoundedIntersection)
                break;
            p1 = p2;
        }
        setLine(QLineF(intersectPoint, endParent+myEndItem->pos()+QPoint(nodeDimension/2+offset,nodeDimension/2+offset)));
        double angle = ::acos(line().dx() / line().length());
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                cos(angle + Pi - Pi / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;

    }

    //! [6] //! [7]
    painter->drawLine(line());
    painter->drawPolygon(arrowHead);
    if (isSelected()) {
        painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);


    }
}
//! [7]

bool RemapArrow::getArrowData()
{
    RemapArrowEdit remapEdit(this);
    bool accept = remapEdit.exec();
    if ((accept)){
//        QStringList temp_name = remapEdit.getFrom().split("{");
        _subToPub=remapEdit.getSubToPub();
    setFrom(remapEdit.getFrom());
    setTo(remapEdit.getTo());
        //        setFrom(myStartItem->getName()+"/"+temp_name.at(0));
//        setFrom(temp_name.at(0));
//        temp_name.clear();
//        temp_name = remapEdit.getTo().split(QRegExp("[{/]"));
//        QString target =temp_name.at(0);
//        //qDebug()<<"name"<<temp_name.count();
        //target = endItem()->getName().append("/").append(target);
//        if (endItem()->getNamespace() != ""){
//            target = endItem()->getNamespace().append("/").append(target);
//        }
//        if (endItem()->parentItem()){
//            GroupItem *group = qgraphicsitem_cast<GroupItem *>(endItem()->parentItem());
//            if (group->getNamespace() !="")
//                target = group->getNamespace().append("/").append(target);

//        }
//        setTo(target);
        setIf(remapEdit.getIf());
        setUnless(remapEdit.getUnless());

        RemapItem *remap=new RemapItem;
        remap->setFrom(this->getFrom());
        remap->setTo(this->getTo());
        remap->setIf(this->getIf());
        remap->setUnless(this->getUnless());
        return true;
    } else
        return false;
}

void RemapArrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    getArrowData();
    this->updatePosition();

    event->setAccepted(true);

}

void RemapArrow::setFrom(QString newFrom)
{
    _fromString=newFrom;
}

void RemapArrow::setTo(QString newTo)
{
    _toString=newTo;
}

QString RemapArrow::getFrom()
{
    return _fromString;
}

QString RemapArrow::getTo()
{
    return _toString;
}

QString RemapArrow::getIf()
{
    return _ifString;
}

void RemapArrow::setIf(QString newIf)
{
    _ifString=newIf;
}

QString RemapArrow::getUnless()
{
    return _unlessString;
}

void RemapArrow::setUnless(QString newUnless)
{
    _unlessString = newUnless;
}

bool RemapArrow::getSubToPub()
{
    return _subToPub;
}
