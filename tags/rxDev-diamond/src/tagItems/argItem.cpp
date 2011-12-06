
#include "argItem.h"
#include <QDebug>
#include "../rxdev.h"
#include "argEdit.h"
#include "includeFileItem.h"
#include "groupItem.h"

ArgItem::ArgItem(QGraphicsScene *scene):
    //QGraphicsItem(parent, scene),
    _location(0,0),
    _width(200),
    _gridSpace(10),
    _outterborderColor(Qt::cyan),
    _outterborderPen(),
    _dragStart(0,0),
    _height(35),
    _value_or_defaultInt(0)
{
    this->setHandlesChildEvents(true);
    this->setParentItem(0);
    _outterborderPen.setWidth(1);
    _outterborderPen.setColor(_outterborderColor);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);

    _title.setPos(-4,_height);
    _title.setPlainText("ARG");
    _title.setParentItem(this);
    _title.setRotation(-90);
    _title.setDefaultTextColor(Qt::black);

    _name.setPos(10,0);
    _name.setTextWidth(_width);
    _name.setParentItem(this);
    _name.setDefaultTextColor(Qt::green);

    _value_or_default.setPos(10,15);
    _value_or_default.setTextWidth(_width);
    _value_or_default.setParentItem(this);
    _value_or_default.setDefaultTextColor(Qt::red);

    updateArgItem();

}


void ArgItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setColor(Qt::red);
    getArgData();
    setColor(Qt::cyan);
    event->setAccepted(true);
}

bool ArgItem::getArgData()
{
    ArgEdit argEdit(this);
    argEdit.setWindowTitle("Arg: "+getName());
    bool accept = argEdit.exec();
    if ((accept)){
        setName(argEdit.getName());
        setValue(argEdit.getValue());
        setValue_or_default(argEdit.getValue_or_default());
        setIf(argEdit.getIf());
        setUnless(argEdit.getUnless());
        updateArgItem();
        return true;
    } else
        return false;
}

void ArgItem::updateArgItem()
{
    _name.setHtml("<font size=\"-2\" color=\"black\">name: <font size=\"-2\" color=\"blue\">"+getName());

    if (_value_or_defaultInt == 0)
        _value_or_default.setHtml("<font size=\"-2\" color=\"black\">~<font size=\"-2\" color=\"blue\">");
    else if (_value_or_defaultInt == 1)
        _value_or_default.setHtml("<font size=\"-2\" color=\"black\">value: <font size=\"-2\" color=\"blue\">"+getValue());
    else if (_value_or_defaultInt == 2)
        _value_or_default.setHtml("<font size=\"-2\" color=\"black\">default: <font size=\"-2\" color=\"blue\">"+getValue());


}

QString ArgItem::getName()
{
    return _nameString;
}

void ArgItem::setName(QString newName)
{
    _nameString = newName;
}
QString ArgItem::getValue()
{
    return _valueString;
}
void ArgItem::setValue(QString newValue)
{
    _valueString = newValue;
}
int ArgItem::getValue_or_default()
{
    return _value_or_defaultInt;
}

void ArgItem::setValue_or_default(int valueOr)
{
    _value_or_defaultInt = valueOr;
}



void ArgItem::setLocation(QPointF point)
{
    _location=point;
}

void ArgItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    // allow the user to drag the box, capture the starting position on mouse-down

    if (event->button() == Qt::LeftButton) {
        setColor(Qt::red);
        event->setAccepted(true);

    } else
        event->setAccepted(false);


}
void ArgItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
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
            case IncludeFileItem::Type:                                                         //IncludeFile
                IncludeFileItem *includeFile;
                includeFile=qgraphicsitem_cast<IncludeFileItem *>(scene()->itemAt(_location));
                includeFile->addArgItem(this);
                scene()->removeItem(this);
                break;
            case 8:                                                             //TextItem -> check for parent
                if (scene()->itemAt(_location)->parentItem()->type() ==IncludeFileItem::Type){  //includeFile
                    IncludeFileItem *includeFile;
                    includeFile=qgraphicsitem_cast<IncludeFileItem *>(scene()->itemAt(_location)->parentItem());
                    includeFile->addArgItem(this);
                    scene()->removeItem(this);
                }
                break;
            default:

                break;
            }
        }
    }
}


void ArgItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    // user have moved the mouse, move the location of the box

    QPointF newPos = event->pos() ;
    _location += (newPos - _dragStart);

    this->setPos(_location);



    //                    if((*it)->type() == SVGPath::Type)
    //                     {
    //                          SVGPath *item = qgraphicsitem_cast <SVGPath (*it);
    //                          if(m_pTextItem->collidesWithPath(item->shape()))
    //                               it++;
    //                          else
    //                               it = collidingWithList.erase(it);
    //                     }
    //               }

    //event->setAccepted(true);// tell the base class we are handling this event
}

void ArgItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(100);
}
void ArgItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(0);
}



void ArgItem::setColor(QColor color)
{
    _outterborderColor = color;
    this->update(0,0,_width,_height);
}

QRectF ArgItem::boundingRect() const
{
    return QRectF(-6,0,_width+15,_height);
}

void ArgItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
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
    background.setStyle(Qt::Dense4Pattern);
    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->setBrush( background);

    painter->drawRect(0, 0, _width,_height);

}

void ArgItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{


}

QString ArgItem::getIf()
{
    return _ifString;
}

void ArgItem::setIf(QString newIf)
{
    _ifString=newIf;
}

QString ArgItem::getUnless()
{
    return _unlessString;
}

void ArgItem::setUnless(QString newUnless)
{
    _unlessString = newUnless;
}


