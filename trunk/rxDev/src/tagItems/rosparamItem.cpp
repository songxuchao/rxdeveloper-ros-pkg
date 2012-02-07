
#include "rosparamItem.h"
#include <QDebug>
#include "../rxdev.h"
#include "rosparamEdit.h"
#include "groupItem.h"

RosparamItem::RosparamItem(QGraphicsRectItem *parent, QGraphicsScene *scene):
    QGraphicsWidget(parent),
    //QGraphicsItem(parent, scene),
    _location(0,0),
    _width(200),
    _gridSpace(10),
    _outterborderColor(Qt::darkCyan),
    _outterborderPen(),
    _dragStart(0,0),
    _height(75)
{
    this->setHandlesChildEvents(true);
    _outterborderPen.setWidth(1);
    _outterborderPen.setColor(_outterborderColor);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);

    _rosparamTitle.setPos(-4,_height+2);
    _rosparamTitle.setPlainText("ROSPARAM");
    _rosparamTitle.setParentItem(this);
    _rosparamTitle.setRotation(-90);
    _rosparamTitle.setDefaultTextColor(Qt::black);

    _name.setPos(10,-2);
    _name.setTextWidth(_width);
    _name.setParentItem(this);
    _name.setDefaultTextColor(Qt::green);

    _value.setPos(10,10);
    _value.setTextWidth(_width);
    _value.setParentItem(this);
    _value.setDefaultTextColor(Qt::red);

    _type.setPos(10,46);
    _type.setParentItem(this);
    _type.setTextWidth(_width);
    _type.setDefaultTextColor(Qt::blue);


    _namespace.setPos(10,56);
    _namespace.setParentItem(this);
    _namespace.setTextWidth(_width);
    _namespace.setDefaultTextColor(Qt::blue);

    updateRosparamItem();

}



void RosparamItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setColor(Qt::red);
    getRosparamData();
    setColor(Qt::darkCyan);
    event->setAccepted(true);
}

bool RosparamItem::getRosparamData()
{
    RosparamEdit param(this);
    param.setWindowTitle("Rosparam: "+getName());
    bool accept = param.exec();
    if (accept){
        setName(param.getName());
        setValue(param.getValue());
        setType(param.getType());
        setText(param.getText());
        setNamespace(param.getNamespace());
        updateRosparamItem();
        setIf(param.getIf());
        setUnless(param.getUnless());
        return true;
    } else
        return false;
}

void RosparamItem::updateRosparamItem()
{
    _name.setHtml("<font size=\"-2\" color=\"black\">param: <font size=\"-2\" color=\"blue\">"+getName());
    _value.setHtml("<font size=\"-2\" color=\"black\">value: <font size=\"-2\" color=\"blue\">"+getValue());
    _type.setHtml("<font size=\"-2\" color=\"black\">type: <font size=\"-2\" color=\"green\">"+getType());
    _namespace.setHtml("<font size=\"-2\" color=\"black\">ns: <font size=\"-2\" color=\"red\">"+getNamespace());
}

QString RosparamItem::getName()
{
    return _nameString;
}

void RosparamItem::setName(QString newName)
{
    _nameString = newName;
}
QString RosparamItem::getValue()
{
    return _valueString;
}
void RosparamItem::setValue(QString newValue)
{
    _valueString = newValue;
}

QString RosparamItem::getType()
{
    return _typeString;
}
void RosparamItem::setType(QString newType)
{
    _typeString = newType;
}

QString RosparamItem::getNamespace()
{
    return _namespaceString;
}
void RosparamItem::setNamespace(QString newNamespace)
{
    _namespaceString = newNamespace;
}


void RosparamItem::setLocation(QPointF point)
{
    _location=point;
}


/*

void NodeItem::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
}


void RosparamItem::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{

    event->setAccepted(false);

}
*/
void RosparamItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{

    if (event->button() == Qt::LeftButton) {
        setColor(Qt::darkCyan);
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

        foreach (QGraphicsItem *item, collidingItems()) {

                if (item->type() ==GroupItem::Type){

                    this->setPos(mapToItem(item,0,0));
                        setParentItem(item);

                } else {
                    this->setPos(mapToScene(0,0));
                    this->setParentItem(0);
                }
            }

        if (scene()->itemAt(_location)){

            switch (scene()->itemAt(_location)->type()){
            case GroupItem::Type:                                                         //Group
                setParentItem(scene()->itemAt(_location));
                this->setPos(_location-(this->parentItem()->pos()));
                break;
            case NodeItem::Type:                                                         //Node or Test
                NodeItem *node;
                node=qgraphicsitem_cast<NodeItem *>(scene()->itemAt(_location));
                node->addRosparamItem(this);
                scene()->removeItem(this);
                break;
            case 8:                                                             //TextItem -> check for parent
                if (scene()->itemAt(_location)->parentItem()->type() ==NodeItem::Type){  //Node or Test
                    NodeItem *node;
                    node=qgraphicsitem_cast<NodeItem *>(scene()->itemAt(_location)->parentItem());
                    node->addRosparamItem(this);
                    scene()->removeItem(this);
                }
                break;
            }
        }
        event->setAccepted(true);// tell the base class we are handling this event

    }
}

void RosparamItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    // allow the user to drag the box, capture the starting position on mouse-down

    if (event->button() == Qt::LeftButton) {
        setColor(Qt::red);
        event->setAccepted(true);
    } else
        event->setAccepted(false);
}


void RosparamItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    // user have moved the mouse, move the location of the box

    QPointF newPos = event->pos() ;
    _location += (newPos - _dragStart);

    this->setPos(_location);
}

void RosparamItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(100);
}
void RosparamItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(0);
}



void RosparamItem::setColor(QColor color)
{
    _outterborderColor = color;
    this->update(0,0,_width,_height);
}

QRectF RosparamItem::boundingRect() const
{
    return QRectF(-6,0,_width+15,_height);
}

void RosparamItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
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

    QBrush background (Qt::yellow);
    background.setColor(Qt::gray);
    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->setBrush( background);
    painter->drawRect(0, 0, _width,_height);

}

QString RosparamItem::getIf()
{
    return _ifString;
}

void RosparamItem::setIf(QString newIf)
{
    _ifString=newIf;
}

QString RosparamItem::getUnless()
{
    return _unlessString;
}

void RosparamItem::setUnless(QString newUnless)
{
    _unlessString = newUnless;
}

QString RosparamItem::getText()
{
    return _rosparamText;
}

void RosparamItem::setText(QString newText)
{
    _rosparamText = newText;
}
