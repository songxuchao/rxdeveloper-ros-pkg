
#include "envItem.h"
#include <QDebug>
#include "../rxdev.h"
#include "envEdit.h"
#include "machineItem.h"
#include "includeFileItem.h"
#include "nodeItem.h"
#include "groupItem.h"

EnvItem::EnvItem(QGraphicsRectItem *parent, QGraphicsScene *scene):
    QGraphicsWidget(parent),
    //QGraphicsItem(parent, scene),
    _location(0,0),
    _width(200),
    _gridSpace(10),
    _outterborderColor(Qt::cyan),
    _outterborderPen(),
    _dragStart(0,0),
    _height(35)
{
    this->setHandlesChildEvents(true);
    _outterborderPen.setWidth(1);
    _outterborderPen.setColor(_outterborderColor);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);

    _title.setPos(-4,_height);
    _title.setPlainText("ENV");
    _title.setParentItem(this);
    _title.setRotation(-90);
    _title.setDefaultTextColor(Qt::black);


    _name.setPos(10,0);
    _name.setTextWidth(_width);
    _name.setParentItem(this);
    _name.setDefaultTextColor(Qt::green);

    _value.setPos(10,15);
    _value.setTextWidth(_width);
    _value.setParentItem(this);
    _value.setDefaultTextColor(Qt::red);


    updateEnvItem();

}


void EnvItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setColor(Qt::red);
    getEnvData();
    setColor(Qt::cyan);
    event->setAccepted(true);
}

bool EnvItem::getEnvData()
{
    EnvEdit envEdit(this);
    envEdit.setWindowTitle("Env: "+getName());
    bool accept = envEdit.exec();
    if ((accept)){
        setName(envEdit.getName());
        setValue(envEdit.getValue());
        setIf(envEdit.getIf());
        setUnless(envEdit.getUnless());
        updateEnvItem();
        return true;
    } else
        return false;
}

void EnvItem::updateEnvItem()
{
    _name.setHtml("<font size=\"-2\" color=\"black\">name: <font size=\"-2\" color=\"blue\">"+getName());
    _value.setHtml("<font size=\"-2\" color=\"black\">value: <font size=\"-2\" color=\"red\">"+getValue());
}

QString EnvItem::getName()
{
    return _nameString;
}

void EnvItem::setName(QString newName)
{
    _nameString = newName;
}
QString EnvItem::getValue()
{
    return _valueString;
}
void EnvItem::setValue(QString newValue)
{
    _valueString = newValue;
}


void EnvItem::setLocation(QPointF point)
{
    _location=point;
}


void EnvItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
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
            case NodeItem::Type:                                                         //Node or Test
                NodeItem *node;
                node=qgraphicsitem_cast<NodeItem *>(scene()->itemAt(_location));
                node->addEnvItem(this);
                scene()->removeItem(this);
                break;
            case MachineItem::Type:                                                         //Machine
                MachineItem *machine;
                machine=qgraphicsitem_cast<MachineItem *>(scene()->itemAt(_location));
                machine->addEnvItem(this);
                scene()->removeItem(this);
                break;
            case IncludeFileItem::Type:                                                         //IncludeFile
                IncludeFileItem *includeFile;
                includeFile=qgraphicsitem_cast<IncludeFileItem *>(scene()->itemAt(_location));
                includeFile->addEnvItem(this);
                scene()->removeItem(this);
                break;
            case 8:                                                             //TextItem -> check for parent
                if (scene()->itemAt(_location)->parentItem()->type() ==NodeItem::Type){  //Node or Test
                    NodeItem *node;
                    node=qgraphicsitem_cast<NodeItem *>(scene()->itemAt(_location)->parentItem());
                    node->addEnvItem(this);
                    scene()->removeItem(this);
                    break;
                }else if (scene()->itemAt(_location)->parentItem()->type() ==MachineItem::Type){  //Machine
                    MachineItem *machine;
                    machine=qgraphicsitem_cast<MachineItem *>(scene()->itemAt(_location)->parentItem());
                    machine->addEnvItem(this);
                    scene()->removeItem(this);
                }else if (scene()->itemAt(_location)->parentItem()->type() ==IncludeFileItem::Type){  //includeFile
                    IncludeFileItem *includeFile;
                    includeFile=qgraphicsitem_cast<IncludeFileItem *>(scene()->itemAt(_location)->parentItem());
                    includeFile->addEnvItem(this);
                    scene()->removeItem(this);
                }
                break;

            }
        }

            event->setAccepted(true);// tell the base class we are handling this event

    }
}

void EnvItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    // allow the user to drag the box, capture the starting position on mouse-down

    if (event->button() == Qt::LeftButton) {
        setColor(Qt::red);
        event->setAccepted(true);

    } else
        event->setAccepted(false);


}


void EnvItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    // user have moved the mouse, move the location of the box

    QPointF newPos = event->pos() ;
    _location += (newPos - _dragStart);
    this->setPos(_location);
}

void EnvItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(100);
}
void EnvItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(0);
}



void EnvItem::setColor(QColor color)
{
    _outterborderColor = color;
    this->update(0,0,_width,_height);
}

QRectF EnvItem::boundingRect() const
{
   return QRectF(-6,0,_width+15,_height);
}

void EnvItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
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
    QBrush background (Qt::gray);
    background.setStyle(Qt::Dense6Pattern);
    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->setBrush( background);

    painter->drawRect(0, 0, _width,_height);

}
QString EnvItem::getIf()
{
    return _ifString;
}

void EnvItem::setIf(QString newIf)
{
    _ifString=newIf;
}

QString EnvItem::getUnless()
{
    return _unlessString;
}

void EnvItem::setUnless(QString newUnless)
{
    _unlessString = newUnless;
}


