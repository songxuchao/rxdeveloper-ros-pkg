
#include "parameterItem.h"
#include <QDebug>
#include "../rxdev.h"
#include "parameterEdit.h"
#include "groupItem.h"

ParameterItem::ParameterItem(QGraphicsRectItem *parent, QGraphicsScene *scene):
    QGraphicsWidget(parent),
    //QGraphicsItem(parent, scene),
    _location(0,0),
    _width(200),
    _gridSpace(10),
    _outterborderColor(Qt::darkCyan),
    _outterborderPen(),
    _dragStart(0,0),
    _height(50),
    _parameter_type(1)
{
    this->setHandlesChildEvents(true);
    _outterborderPen.setWidth(1);
    _outterborderPen.setColor(_outterborderColor);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);

    _paramTitle.setPos(-4,_height);
    _paramTitle.setPlainText("PARAM");
    _paramTitle.setParentItem(this);
    _paramTitle.setRotation(-90);
    _paramTitle.setDefaultTextColor(Qt::black);

    _rosparamTitle.setPos(-4,_height+2);
    _rosparamTitle.setPlainText("ROSPARAM");
    _rosparamTitle.setParentItem(this);
    _rosparamTitle.setRotation(-90);
    _rosparamTitle.setDefaultTextColor(Qt::black);

    _name.setPos(10,0);
    _name.setTextWidth(_width);
    _name.setParentItem(this);
    _name.setDefaultTextColor(Qt::green);

    _value.setPos(10,15);
    _value.setTextWidth(_width);
    _value.setParentItem(this);
    _value.setDefaultTextColor(Qt::red);

    _type.setPos(10,30);
    _type.setParentItem(this);
    _type.setTextWidth(_width);
    _type.setDefaultTextColor(Qt::blue);


    _namespace.setPos(10,45);
    _namespace.setParentItem(this);
    _namespace.setTextWidth(_width);
    _namespace.setDefaultTextColor(Qt::blue);

    updateParameterItem();

}



void ParameterItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setColor(Qt::red);
    getParamData();
    setColor(Qt::darkCyan);
    event->setAccepted(true);
}
void ParameterItem::setStandardParameter(int set)
{
    _parameter_type=set;
}
int ParameterItem::getStandardParameter()
{
    return _parameter_type;
}

bool ParameterItem::getParamData()
{
    ParameterEdit param(this);
    param.setWindowTitle("Parameter: "+getName());
    bool accept = param.exec();
    if (accept){
        setName(param.getName());
        setValue(param.getValue());
        setType(param.getType());
        setNamespace(param.getNamespace());
        setStandardParameter(param.getParamType());
        updateParameterItem();
        setIf(param.getIf());
        setUnless(param.getUnless());
        return true;
    } else
        return false;
}

void ParameterItem::updateParameterItem()
{
    if (_parameter_type==3)
        _name.setHtml("<font size=\"-2\" color=\"black\">param: <font size=\"-2\" color=\"blue\">"+getName());
    else
        _name.setHtml("<font size=\"-2\" color=\"black\">name: <font size=\"-2\" color=\"blue\">"+getName());
    _value.setHtml("<font size=\"-2\" color=\"black\">value: <font size=\"-2\" color=\"blue\">"+getValue());
    _type.setHtml("<font size=\"-2\" color=\"black\">type: <font size=\"-2\" color=\"green\">"+getType());
    _namespace.setHtml("<font size=\"-2\" color=\"black\">ns: <font size=\"-2\" color=\"red\">"+getNamespace());

}

QString ParameterItem::getName()
{
    return _nameString;
}

void ParameterItem::setName(QString newName)
{
    _nameString = newName;
}
QString ParameterItem::getValue()
{
    return _valueString;
}
void ParameterItem::setValue(QString newValue)
{
    _valueString = newValue;
}

QString ParameterItem::getType()
{
    return _typeString;
}
void ParameterItem::setType(QString newType)
{
    _typeString = newType;
}

QString ParameterItem::getNamespace()
{
    return _namespaceString;
}
void ParameterItem::setNamespace(QString newNamespace)
{
    _namespaceString = newNamespace;
}


void ParameterItem::setLocation(QPointF point)
{
    _location=point;
}


/*

void NodeItem::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
}


void ParameterItem::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{

    event->setAccepted(false);

}
*/
void ParameterItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
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

        if (scene()->itemAt(_location-QPoint(1,1))){

            switch (scene()->itemAt(_location-QPoint(1,1))->type()){
            case GroupItem::Type:                                                         //Group
                setParentItem(scene()->itemAt(_location-QPoint(1,1)));
                this->setPos(_location-(this->parentItem()->pos()));
                break;
            case NodeItem::Type:                                                         //Node or Test
                NodeItem *node;
                node=qgraphicsitem_cast<NodeItem *>(scene()->itemAt(_location-QPoint(1,1)));
                node->addParamItem(this);
                scene()->removeItem(this);
                break;
            case 8:                                                             //TextItem -> check for parent
                if (scene()->itemAt(_location-QPoint(1,1))->parentItem()->type() ==NodeItem::Type){  //Node or Test
                    NodeItem *node;
                    node=qgraphicsitem_cast<NodeItem *>(scene()->itemAt(_location-QPoint(1,1))->parentItem());
                    node->addParamItem(this);
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

void ParameterItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    // allow the user to drag the box, capture the starting position on mouse-down

    if (event->button() == Qt::LeftButton) {
        setColor(Qt::red);
        event->setAccepted(true);

    } else
        event->setAccepted(false);


}


void ParameterItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    // user have moved the mouse, move the location of the box

    QPointF newPos = event->pos() ;
    _location += (newPos - _dragStart);

    this->setPos(_location);
}

void ParameterItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(100);
}
void ParameterItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(0);
}



void ParameterItem::setColor(QColor color)
{
    _outterborderColor = color;
    this->update(0,0,_width,_height);
}

QRectF ParameterItem::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(0 - penWidth / 2, 0 - penWidth / 2,
                  _width + penWidth, _height + penWidth);
}

void ParameterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
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
    if (_parameter_type<3){
        _paramTitle.show();
        _rosparamTitle.hide();
        _namespace.hide();
        _height=50;
        _paramTitle.setPos(-4,_height);
    } else{
        background.setColor(Qt::gray);
        _namespace.show();
        _height=70;
        _paramTitle.hide();
        _rosparamTitle.show();
        _rosparamTitle.setPos(-4,_height+2);
    }


    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->setBrush( background);

    painter->drawRect(0, 0, _width,_height);

}

QString ParameterItem::getIf()
{
    return _ifString;
}

void ParameterItem::setIf(QString newIf)
{
    _ifString=newIf;
}

QString ParameterItem::getUnless()
{
    return _unlessString;
}

void ParameterItem::setUnless(QString newUnless)
{
    _unlessString = newUnless;
}
