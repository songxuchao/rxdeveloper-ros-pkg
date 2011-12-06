

#include "machineItem.h"
#include <QDebug>
#include "../rxdev.h"
#include "machineEdit.h"
#include "groupItem.h"

MachineItem::MachineItem(QGraphicsRectItem *parent, QGraphicsScene *scene):
    QGraphicsWidget(parent),
    //QGraphicsItem(parent, scene),
    _location(0,0),
    _width(200),
    _gridSpace(10),
    _outterborderColor(Qt::cyan),
    _outterborderPen(),
    _dragStart(0,0),
    _height(68)
{
    this->setHandlesChildEvents(true);
    _outterborderPen.setWidth(1);
    _outterborderPen.setColor(_outterborderColor);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);

    _title.setPos(-4,_height);
    _title.setPlainText("MACHINE");
    _title.setParentItem(this);
    _title.setRotation(-90);
    _title.setDefaultTextColor(Qt::black);

    _name.setPos(10,0);
    _name.setTextWidth(_width);
    _name.setParentItem(this);
    _name.setDefaultTextColor(Qt::red);

    _address.setPos(10,15);
    _address.setTextWidth(_width);
    _address.setParentItem(this);
    _address.setDefaultTextColor(Qt::green);

    _default.setPos(10,30);
    _default.setTextWidth(_width);
    _default.setParentItem(this);
    _default.setDefaultTextColor(Qt::green);

    _timeout.setPos(10,45);
    _timeout.setTextWidth(_width);
    _timeout.setParentItem(this);
    _timeout.setDefaultTextColor(Qt::green);

    updateMachineItem();

}
MachineItem::~MachineItem(){
            removeEnvItems();
        }


void MachineItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setColor(Qt::red);
    getMachineData();
    setColor(Qt::cyan);
    event->setAccepted(true);
}

bool MachineItem::getMachineData()
{
    MachineEdit machineEdit(this);
    machineEdit.setWindowTitle("Machine: "+getName());
    bool accept = machineEdit.exec();
    if (accept){
        setName(machineEdit.getName());
        setAddress(machineEdit.getAddress());
        setRos_root(machineEdit.getRos_root());
        setRos_package_path(machineEdit.getRos_package_path());
        setDefault(machineEdit.getDefault());
        setUser(machineEdit.getUser());
        setPassword(machineEdit.getPassword());
        setTimeout(machineEdit.getTimeout());
        setIf(machineEdit.getIf());
        setUnless(machineEdit.getUnless());
        updateMachineItem();
        return true;
    } else
        return false;
}

void MachineItem::updateMachineItem()
{
    _name.setHtml("<font size=\"-2\" color=\"black\">name: <font size=\"-2\" color=\"blue\">"+getName());
    _address.setHtml("<font size=\"-2\" color=\"black\">address: <font size=\"-2\" color=\"blue\">"+getAddress());
    _default.setHtml("<font size=\"-2\" color=\"black\">default: <font size=\"-2\" color=\"green\">"+getDefault());
    _timeout.setHtml("<font size=\"-2\" color=\"black\">timeout: <font size=\"-2\" color=\"green\">"+getTimeout());
}

QString MachineItem::getName()
{
    return _nameString;
}
void MachineItem::setName(QString newName)
{
    _nameString = newName;
}
QString MachineItem::getAddress()
{
    return _addressString;
}

void MachineItem::setAddress(QString newAddress)
{
    _addressString = newAddress;
}
QString MachineItem::getRos_root()
{
    return _ros_rootString;
}

void MachineItem::setRos_root(QString newRos_root)
{
    _ros_rootString = newRos_root;
}

QString MachineItem::getRos_package_path()
{
        return _ros_package_pathString;
}

void MachineItem::setRos_package_path(QString newRos_package_path)
{
    _ros_package_pathString = newRos_package_path;
}

QString MachineItem::getDefault()
{
        return _defaultString;
}

void MachineItem::setDefault(QString newDefault)
{
    _defaultString = newDefault;
}

QString MachineItem::getUser()
{
        return _userString;
}

void MachineItem::setUser(QString newUser)
{
    _userString = newUser;
}

QString MachineItem::getPassword()
{
        return _passwordString;
}

void MachineItem::setPassword(QString newPassword)
{
    _passwordString = newPassword;
}

QString MachineItem::getTimeout()
{
        return _timeoutString;
}

void MachineItem::setTimeout(QString newTimeout)
{
    _timeoutString = newTimeout;
}

void MachineItem::setLocation(QPointF point)
{
    _location=point;
}


void MachineItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
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

        event->setAccepted(true);// tell the base class we are handling this event

    }
}

void MachineItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    // allow the user to drag the box, capture the starting position on mouse-down

    if (event->button() == Qt::LeftButton) {
        setColor(Qt::red);
        event->setAccepted(true);

    } else
        event->setAccepted(false);


}


void MachineItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    // user have moved the mouse, move the location of the box

    QPointF newPos = event->pos() ;
    _location += (newPos - _dragStart);

    this->setPos(_location);
}

void MachineItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(100);
}
void MachineItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(0);
}



void MachineItem::setColor(QColor color)
{
    _outterborderColor = color;
    this->update(0,0,_width,_height);
}

QRectF MachineItem::boundingRect() const
{
   return QRectF(-6,0,_width+15,_height);
}

void MachineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
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
    QBrush background (Qt::red);
    background.setStyle(Qt::Dense3Pattern);
    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->setBrush( background);

    painter->drawRect(0, 0, _width,_height);

}

void MachineItem::addEnvItem(EnvItem *env)
{
    envItems.append(env);

}

void MachineItem::removeEnvItems()
{
    foreach (EnvItem *env, envItems) {
        removeEnvItem(env);
        delete env;
    }

}

void MachineItem::removeEnvItem(EnvItem *env)
{
    int index = envItems.indexOf(env);

    if (index != -1)
        envItems.removeAt(index);
}
QString MachineItem::getIf()
{
    return _ifString;
}

void MachineItem::setIf(QString newIf)
{
    _ifString=newIf;
}

QString MachineItem::getUnless()
{
    return _unlessString;
}

void MachineItem::setUnless(QString newUnless)
{
    _unlessString = newUnless;
}
