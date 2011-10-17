
#include "remapItem.h"
#include <QDebug>
#include "../rxdev.h"
#include "remapEdit.h"
#include "nodeItem.h"
#include "groupItem.h"

RemapItem::RemapItem(QGraphicsRectItem *parent, QGraphicsScene *scene):
    QGraphicsWidget(parent),
    //QGraphicsItem(parent, scene),
    _location(0,0),
    _width(200),
    _gridSpace(10),
    _outterborderColor(Qt::cyan),
    _outterborderPen(),
    _dragStart(0,0),
    _height(45)
{
    this->setHandlesChildEvents(true);
    _outterborderPen.setWidth(1);
    _outterborderPen.setColor(_outterborderColor);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);

    _title.setPos(-4,_height+2);
    _title.setPlainText("REMAP");
    _title.setParentItem(this);
    _title.setRotation(-90);
    _title.setDefaultTextColor(Qt::black);


    _from.setPos(10,0);
    _from.setTextWidth(_width);
    _from.setParentItem(this);
    _from.setDefaultTextColor(Qt::green);

    _to.setPos(10,15);
    _to.setTextWidth(_width);
    _to.setParentItem(this);
    _to.setDefaultTextColor(Qt::red);


    updateRemapItem();

}


void RemapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setColor(Qt::red);
    getRemapData();
    setColor(Qt::cyan);
    event->setAccepted(true);
}

bool RemapItem::getRemapData()
{
    RemapEdit remapEdit(this);

    bool accept = remapEdit.exec();
    if ((accept)){
        setFrom(remapEdit.getFrom());
        setTo(remapEdit.getTo());
        setIf(remapEdit.getIf());
        setUnless(remapEdit.getUnless());
        updateRemapItem();
        return true;
    } else
        return false;
}

void RemapItem::updateRemapItem()
{
    _from.setHtml("<font size=\"-2\" color=\"black\">from: <font size=\"-2\" color=\"blue\">"+getFrom());
    _to.setHtml("<font size=\"-2\" color=\"black\">to: <font size=\"-2\" color=\"red\">"+getTo());
}

QString RemapItem::getFrom()
{
    return _fromString;
}

void RemapItem::setFrom(QString newFrom)
{
    _fromString = newFrom;
}
QString RemapItem::getTo()
{
    return _toString;
}
void RemapItem::setTo(QString newTo)
{
    _toString = newTo;
}


void RemapItem::setLocation(QPointF point)
{
    _location=point;
}


void RemapItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{

    if (event->button() == Qt::LeftButton) {
        setColor(Qt::cyan);
        // force my box to snap to grid, just truncate the pixel number and
        // snap to the next lowest grid value

        if (fmod(_location.x(),_gridSpace) >_gridSpace/2)
            _location.setX( ( static_cast<int>(_location.x()+_gridSpace) / _gridSpace) * _gridSpace );
        else
            _location.setX( ( static_cast<int>(_location.x()) / _gridSpace) * _gridSpace );
        if (fmod(_location.y(),_gridSpace) >_gridSpace/2)
            _location.setY( ( static_cast<int>(_location.y()+_gridSpace) / _gridSpace) * _gridSpace );
        else
            _location.setY( ( static_cast<int>(_location.y()) / _gridSpace) * _gridSpace );

        if (scene()->itemAt(_location-QPoint(1,1))){
            switch (scene()->itemAt(_location-QPoint(1,1))->type()){
            case GroupItem::Type:                                                         //Group
                setParentItem(scene()->itemAt(_location-QPoint(1,1)));
                this->setPos(_location-(this->parentItem()->pos()));
                break;
            case NodeItem::Type:                                                         //Node or Test
                NodeItem *node;
                node=qgraphicsitem_cast<NodeItem *>(scene()->itemAt(_location-QPoint(1,1)));
                node->addRemapItem(this);
                //Todo: create a remapArrow
                scene()->removeItem(this);
                break;
            case 8:                                                             //TextItem -> check for parent
                if (scene()->itemAt(_location-QPoint(1,1))->parentItem()->type() ==NodeItem::Type){  //Node or Test
                    NodeItem *node;
                    node=qgraphicsitem_cast<NodeItem *>(scene()->itemAt(_location-QPoint(1,1))->parentItem());
                    node->addRemapItem(this);
                    //Todo: create a remapArrow
                    scene()->removeItem(this);
                }
                break;

            }
        }else{
            if (this->parentItem()){
                this->setPos(_location+(this->parentItem()->pos()));
                this->setParentItem(0);
            }else
                this->setPos(mapToScene(0,0));

        }

            event->setAccepted(true);// tell the base class we are handling this event

    }
}

void RemapItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    // allow the user to drag the box, capture the starting position on mouse-down

    if (event->button() == Qt::LeftButton) {
        setColor(Qt::red);
        event->setAccepted(true);

    } else
        event->setAccepted(false);


}


void RemapItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    // user have moved the mouse, move the location of the box

    QPointF newPos = event->pos() ;
    _location += (newPos - _dragStart);

    this->setPos(_location);
}

void RemapItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(100);
}
void RemapItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(0);
}



void RemapItem::setColor(QColor color)
{
    _outterborderColor = color;
    this->update(0,0,_width,_height);
}

QRectF RemapItem::boundingRect() const
{
   return QRectF(-6,0,_width+15,_height);
}

void RemapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget)

{
    _outterborderPen.setColor( _outterborderColor );
    _outterborderPen.setStyle(Qt::SolidLine);
    painter->setPen(_outterborderPen);
    if (isSelected()) {
        painter->setPen(QPen(Qt::red, 2, Qt::DashLine));
    }
    if (parentItem()) {
        painter->drawLine(0,2,-5,2);
        painter->drawLine(0,4,-5,4);
        painter->drawLine(0,_height-2,-5,_height-2);
        painter->drawLine(0,_height-4,-5,_height-4);
        painter->drawLine(_width,2,_width+5,2);
        painter->drawLine(_width,4,_width+5,4);
        painter->drawLine(_width,_height-2,_width+5,_height-2);
        painter->drawLine(_width,_height-4,_width+5,_height-4);

    }
    QBrush background (Qt::green);
    background.setStyle(Qt::Dense6Pattern);
    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->setBrush( background);

    painter->drawRect(0, 0, _width,_height);

}


