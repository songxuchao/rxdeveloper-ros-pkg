/*
  * Original Code Copyright 2010 by David W. Drell
  * Taken from ...
  *
  */

#include "nodeItem.h"
#include <QDebug>
#include <QBrush>
#include <QLinearGradient>
#include "../rxdev.h"
#include "./ui_rxdev.h"
#include "remapArrow.h"
#include "nodeEdit.h"
#include "math.h"
#include "groupItem.h"

NodeItem::NodeItem(QString node,QString package, QStringList subscriptions,QStringList publications,QStringList services,QStringList parameters, QGraphicsPolygonItem *parent, QGraphicsScene *scene):
    QGraphicsPolygonItem(parent, scene),
    _title(),
    _name(),
    _namespace(),
    _outterborderColor(Qt::black),
    _outterborderPen(),
    _location(0,0),
    _dragStart(0,0),
    _gridSpace(10),
    _width(100),
    _height(100),
    _node_or_testInt(0),
    _requiredInt(0),
    _respawnInt(0),
    _clear_paramsInt(0),
    _outputInt(0),
    _cwdInt(0),
    _blank(false)

{
    this->setParentItem(0);
    this->setHandlesChildEvents(true);
    _outterborderPen.setWidth(2);
    _outterborderPen.setColor(_outterborderColor);

    _title.setPos(0,-2);
    _title.setParentItem(this);
    _title.setTextWidth(_width);
    _title.setDefaultTextColor(Qt::blue);


    _name.setPos(0,22);
    _name.setTextWidth(_width);
    _name.setParentItem(this);
    _name.setDefaultTextColor(Qt::green);

    _namespace.setPos(0,54);
    _namespace.setTextWidth(_width);
    _namespace.setParentItem(this);
    _namespace.setDefaultTextColor(Qt::red);

    _contains.setPos(0,70);
    _contains.setTextWidth(_width);
    _contains.setParentItem(this);
    _contains.setDefaultTextColor(Qt::black);

    _pkgString=package;
    _typeString=node;
    _subscriptionsStringList=subscriptions;
    _publicationsStringList=publications;
    _servicesStringList=services;
    _parametersStringList=parameters;
    updateNodeItem();


    //QPainterPath path;
    myPolygon << QPointF(0, 0) << QPointF(0, _height)
              << QPointF(_width,_height) << QPointF(_width, 0)
              << QPointF(0, 0);

    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    this->setAcceptHoverEvents(true);


}

NodeItem::~NodeItem()
{

}

void NodeItem::setLocation(QPointF point)
{
    _location=point;
}


/* re-implement the mouse event handlers from the base QGraphicsItem class
  there are two versions provided in the base class, two that
  take QGraphicsSceneDragDropEvent - which I will ignore - and two
  that take QGraphicsSceneMouseEvent - which I will use
  */

void NodeItem::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
}

QString NodeItem::getNamespace()
{
    return _namespaceString;
}

void NodeItem::setNamespace(QString newNamespace)
{
    _namespaceString = newNamespace;
}

QString NodeItem::getName()
{
    return _nameString;
}
void NodeItem::setName(QString newName)
{
    _nameString = newName;
}


void NodeItem::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{

    event->setAccepted(false);

}

void NodeItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{

    if (event->button() == Qt::LeftButton) {



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

        setColor(Qt::blue);
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

void NodeItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    // allow the user to drag the box, capture the starting position on mouse-down
    if (event->button() == Qt::LeftButton) {

        setColor(Qt::red);

        _dragStart = event->pos();
    //    qDebug()<< getNamespace();

        event->setAccepted(true);

    } else
        event->setAccepted(false);


}


void NodeItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    // user have moved the mouse, move the location of the box

    QPointF newPos = event->pos() ;
    _location += (newPos - _dragStart);

    foreach (RemapArrow *arrow, arrows)
        arrow->updatePosition();

    this->setPos(_location);

    event->setAccepted(true);


}
void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton){

        setColor(Qt::yellow);
        getNodeData();

    }
    event->setAccepted(true);

}

void NodeItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    // return the box color to black when the mouse is no longer hovering

}

void NodeItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    // draw the box in blue if the mouse is hovering over it


}
void NodeItem::setColor(QColor color)
{
    _outterborderColor = color;
    this->update(0,0,_width,_height);
}

// boundingRect must be re-implemented from the base class to provide the scene with
// size info about this custom GraphicsItem

QRectF NodeItem::boundingRect() const
{
    return QRectF(-6,0,_width+15,_height);
}

void NodeItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{



    // draw the outter box

    _outterborderPen.setColor( _outterborderColor );
    _outterborderPen.setStyle(Qt::SolidLine);

    painter->setPen(_outterborderPen);
    QBrush background (QColor::fromRgb(218,218,255,255), Qt::SolidPattern);
    if (_node_or_testInt==1)
        background.setStyle(Qt::DiagCrossPattern);
    if (parentItem()) {
        painter->drawLine(0,10,-5,10);
        painter->drawLine(0,6,-5,6);
        painter->drawLine(0,_height-10,-5,_height-10);
        painter->drawLine(0,_height-6,-5,_height-6);
        painter->drawLine(_width,10,_width+5,10);
        painter->drawLine(_width,6,_width+5,6);
        painter->drawLine(_width,_height-10,_width+5,_height-10);
        painter->drawLine(_width,_height-6,_width+5,_height-6);

    }
    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->setBrush( background);

    //draw upper left corner

    QPointF topLeft (0, 0);
    QPointF bottomRight ( _width, _height );

    QRectF rect (topLeft, bottomRight);
    painter->drawRoundRect(rect,25,25);
    painter->drawLine(0,26,_width,26);


}
void NodeItem::addRemapItem(RemapItem *remap)
{
    remapItems.append(remap);
    updateContains();
}

void NodeItem::removeRemapItems()
{
    foreach (RemapItem *remap, remapItems) {
        removeRemapItem(remap);
        delete remap;
    }

}

void NodeItem::removeRemapItem(RemapItem *remap)
{
    int index = remapItems.indexOf(remap);

    if (index != -1)
        remapItems.removeAt(index);
    updateContains();
}


bool NodeItem::getNodeData()
{
    if (this->getType()=="_blank_node"){
        _blank=true;
    }
    NodeEdit nodeEdit(this, _blank);
    if (this->getType()=="nodelet")
        nodeEdit.setWindowTitle("Nodelet: "+getName());
    else
        nodeEdit.setWindowTitle("Node: "+getName());

    bool accept = nodeEdit.exec();
    if ((accept)){
        setType(nodeEdit.getType());
        setName(nodeEdit.getName());
        setArgs(nodeEdit.getArgs());
        setPkg(nodeEdit.getPackage());
        setNamespace(nodeEdit.getNamespace());
        setMachine(nodeEdit.getMachine());
        setTime_limit(nodeEdit.getTime_limit());
        setRetry(nodeEdit.getRetry());
        setLaunch_prefix(nodeEdit.getLaunch_prefix());
        setRequired(nodeEdit.getRequired());
        setRespawn(nodeEdit.getRespawn());
        setClear_params(nodeEdit.getClear_params());
        setOutput(nodeEdit.getOutput());
        setCwd(nodeEdit.getCwd());
        setNode_or_test(nodeEdit.node_or_test);
        setIf(nodeEdit.getIf());
        setUnless(nodeEdit.getUnless());
        //Todo: update all arrowtitles if the ns or the group-ns is changed
        updateNodeItem();
        return true;
    } else
        return false;
}
void NodeItem::updateNodeItem()
{
    _title.setHtml("<font size=\"-2\" color=\"black\"><center>::<font size=\"-2\" color=\"blue\">"+getType()+"<font size=\"-2\" color=\"black\">::</center>");
    _name.setHtml("<font size=\"-2\" color=\"black\">name: <font size=\"-2\" color=\"blue\">"+getName());
    _namespace.setHtml("<font size=\"-2\" color=\"black\">ns: <font size=\"-2\" color=\"red\">"+getNamespace());
}


QPixmap NodeItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);

    return pixmap;
}


QVariant NodeItem::itemChange(GraphicsItemChange change,
                              const QVariant &value)
{

    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (RemapArrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }
    //hier aktuelles item

    return value;
}
//! [1]
void NodeItem::removeArrow(RemapArrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);

}
//! [1]

//! [2]
void NodeItem::removeArrows()
{
    foreach (RemapArrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
//! [2]

//! [3]
void NodeItem::addArrow(RemapArrow *arrow)
{
    arrows.append(arrow);
}

int NodeItem::getNode_or_test()
{
    return _node_or_testInt;
}

QString NodeItem::getType()
{
    return _typeString;

}

QString NodeItem::getPkg()
{
    return _pkgString;

}

QString NodeItem::getArgs()
{
    return _argsString;

}

void NodeItem::setArgs(QString newArgs)
{
    _argsString = newArgs;
}

QString NodeItem::getMachine()
{
    return _machineString;
}

void NodeItem::setMachine(QString newMachine)
{
    _machineString = newMachine;
}

QString NodeItem::getTime_limit()
{
    return _time_limitString;
}

void NodeItem::setTime_limit(QString newTime_limit)
{
    _time_limitString = newTime_limit;
}

QString NodeItem::getRetry()
{
    return _retryString;
}

void NodeItem::setRetry(QString newRetry)
{
    _retryString = newRetry;
}

QString NodeItem::getLaunch_prefix()
{
    return _launch_prefixString;
}

void NodeItem::setLaunch_prefix(QString newLaunch_prefix)
{
    _launch_prefixString = newLaunch_prefix;
}

int NodeItem::getRequired()
{
    return _requiredInt;
}

void NodeItem::setRequired(int required)
{
    _requiredInt = required;
}

int NodeItem::getRespawn()
{
    return _respawnInt;
}

void NodeItem::setRespawn(int respawn)
{
    _respawnInt = respawn;
}

int NodeItem::getClear_params()
{
    return _clear_paramsInt;
}

void NodeItem::setClear_params(int clear)
{
    _clear_paramsInt = clear;
}

int NodeItem::getOutput()
{
    return _outputInt;
}

void NodeItem::setOutput(int output)
{
    _outputInt = output;
}

int NodeItem::getCwd()
{
    return _cwdInt;
}

void NodeItem::setCwd(int cwd)
{
    _cwdInt = cwd;
}

void NodeItem::setNode_or_test(int set)
{
    _node_or_testInt=set;

}

QStringList NodeItem::getPublications()
{
    return _publicationsStringList;
}

QStringList NodeItem::getSubscriptions()
{
    return _subscriptionsStringList;
}

QStringList NodeItem::getServices()
{
    return _servicesStringList;
}

QStringList NodeItem::getParameters()
{
    return _parametersStringList;
}

QPointF NodeItem::getLocation()
{
    return _location;
}
//! [3]

void NodeItem::addEnvItem(EnvItem *env)
{
    envItems.append(env);
    updateContains();

}

void NodeItem::removeEnvItems()
{
    foreach (EnvItem *env, envItems) {
        removeEnvItem(env);
        delete env;
    }

}

void NodeItem::removeEnvItem(EnvItem *env)
{
    int index = envItems.indexOf(env);

    if (index != -1)
        envItems.removeAt(index);
    updateContains();
}


void NodeItem::addParamItem(ParameterItem *param)
{
    paramItems.append(param);
    updateContains();
}

void NodeItem::removeParamItems()
{
    foreach (ParameterItem *param, paramItems) {
        removeParamItem(param);
        delete param;
    }

}

void NodeItem::removeParamItem(ParameterItem *param)
{
    int index = paramItems.indexOf(param);

    if (index != -1)
        paramItems.removeAt(index);
    updateContains();
}
void NodeItem::removeRosparamItem(RosparamItem *rosparam)
{
    int index = rosparamItems.indexOf(rosparam);

    if (index != -1)
        rosparamItems.removeAt(index);
    updateContains();
}

void NodeItem::removeRosparamItems()
{
    foreach (RosparamItem *rosparam, rosparamItems) {
        removeRosparamItem(rosparam);
        delete rosparam;
    }
}

void NodeItem::addRosparamItem(RosparamItem *rosparam)
{
    rosparamItems.append(rosparam);
    updateContains();
}

QString NodeItem::getIf()
{
    return _ifString;
}

void NodeItem::setIf(QString newIf)
{
    _ifString=newIf;
}

QString NodeItem::getUnless()
{
    return _unlessString;
}

void NodeItem::setUnless(QString newUnless)
{
    _unlessString = newUnless;
}

void NodeItem::setType(QString type)
{
    _typeString=type;
}

void NodeItem::updateContains()
{
    _containsString="";
    if (remapItems.count()!=0)
        _containsString.append(" remap");
    if (paramItems.count()!=0)
        _containsString.append(" param");
    if (rosparamItems.count()!=0)
        _containsString.append(" rosparam");
    if (envItems.count()!=0)
        _containsString.append(" env");
    if (remapItems.count()!=0 ||paramItems.count()!=0||rosparamItems.count()!=0||envItems.count()!=0)
        _contains.setHtml("<font size=\"-2\" color=\"black\">contains: <font size=\"-2\" color=\"red\">"+_containsString);
    else
        _contains.setHtml("<font size=\"-2\" color=\"black\">contains: <font size=\"-2\" color=\"red\">");

}

void NodeItem::setPkg(QString newPackage)
{
    _pkgString = newPackage;
}

