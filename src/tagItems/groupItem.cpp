



#include "groupItem.h"
#include <QDebug>
#include "../rxdev.h"
#include "groupEdit.h"
#include "nodeItem.h"
#include "remapArrow.h"

GroupItem::GroupItem(QGraphicsRectItem *parent, QGraphicsScene *scene):
    QGraphicsRectItem(parent,scene),

    _location(0,0),
    _width(500),
    _gridSpace(10),
    _outterborderColor(Qt::black),
    _outterborderPen(),
    _dragStart(0,0),
    _height(500),
  _clear_paramsInt(0)
{
//    this->setHandlesChildEvents(true);
    _outterborderPen.setWidth(1);
    _outterborderPen.setColor(_outterborderColor);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);

    _title.setPos(20,-30);
    _title.setPlainText("<GROUP>");
    _title.setParentItem(this);

    _namespace.setPos(90,-30);
    _namespace.setTextWidth(_width);
    _namespace.setParentItem(this);

    _clear_params.setPos(10,-15);
    _clear_params.setTextWidth(_width);
    _clear_params.setParentItem(this);


    setZValue(-40);
    updateGroupItem();

}


/*
GroupItem::~GroupItem()
{

}
*/
void GroupItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

//    QList<QGraphicsItem *> list;
//    list=this->childItems();
//    qDebug()<<"Begin group items->";
//    for (int i = 0; i < list.size(); i++) {
//        if (list.at(i)->type() != 8)
//            qDebug()<<(list.at(i)->type())<<": "<<list.at(i)->pos()+this->pos();
//     }
//    qDebug()<<"<-End group items";
    setColor(Qt::red);
    getGroupData();
    setColor(Qt::cyan);
    event->setAccepted(true);
}

bool GroupItem::getGroupData()
{
    GroupEdit groupEdit(this);
    groupEdit.setWindowTitle("Group: "+getNamespace());
    bool accept = groupEdit.exec();
    if ((accept)){
        setNamespace(groupEdit.getNamespace());
//        qDebug()<<groupEdit.getClear_params();
        setClear_params(groupEdit.getClear_params());
        _width = groupEdit.getWidth();
        _height = groupEdit.getHeight();
        setIf(groupEdit.getIf());
        setUnless(groupEdit.getUnless());
        updateGroupItem();
        return true;
    } else
        return false;
}

void GroupItem::updateGroupItem()
{
    //qDebug()<<"cp "<<_clear_paramsInt;
    if (getNamespace().isEmpty())
        _namespace.setHtml("");
    else
        _namespace.setHtml("<font size=\"-2\" color=\"black\">ns: <font size=\"-2\" color=\"red\">"+getNamespace());
    if (_clear_paramsInt == 0)
    _clear_params.setHtml("");
    else if (_clear_paramsInt == 1)
        _clear_params.setHtml("<font size=\"-2\" color=\"black\">clear_params: <font size=\"-2\" color=\"blue\">true");
    else if (_clear_paramsInt == 2)
    _clear_params.setHtml("<font size=\"-2\" color=\"black\">clear_params: <font size=\"-2\" color=\"blue\">false");
}


QString GroupItem::getNamespace()
{
    return _namespaceString;
}
void GroupItem::setNamespace(QString newNamespace)
{
    _namespaceString = newNamespace;
}
int GroupItem::getClear_params()
{
    return _clear_paramsInt;
}

void GroupItem::setClear_params(int clear)
{
    _clear_paramsInt = clear;
}



void GroupItem::setLocation(QPointF point)
{
    _location=point;
}


void GroupItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{

    if (event->button() == Qt::LeftButton) {
        setColor(Qt::black);
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
        this->setPos(_location);
        event->setAccepted(true);// tell the base class we are handling this event

    }
}

void GroupItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    // allow the user to drag the box, capture the starting position on mouse-down

    if (event->button() == Qt::LeftButton) {
        setColor(Qt::red);
        event->setAccepted(true);


    } else
        event->setAccepted(false);


}


void GroupItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    // user have moved the mouse, move the location of the box

    QPointF newPos = event->pos() ;
    _location += (newPos - _dragStart);
    this->setPos(_location);
    QList<QGraphicsItem *> list;
    list=this->childItems();
    //qDebug()<<"Begin group items->";
    for (int i = 0; i < list.size(); i++) {
        if (list.at(i)->type() == NodeItem::Type){
            NodeItem *item;
            item=qgraphicsitem_cast<NodeItem *>(list.at(i));

           item->setLocation(item->pos()+this->pos());

           for (int i =0; i< item->arrows.size(); i++)
            item->arrows.at(i)->updatePosition();

        }
     }

}

void GroupItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    setSelected(true);
    //setZValue(zValue()+19);
}
void GroupItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    setSelected(false);
    //setZValue(zValue()-19);
}



void GroupItem::setColor(QColor color)
{
    _outterborderColor = color;
    this->update(0,0,_width,_height);
}

QRectF GroupItem::boundingRect() const
{

        return QRectF(0,0,_width,_height);


}

void GroupItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                            QWidget *widget)

{

    _outterborderPen.setColor( _outterborderColor );
    _outterborderPen.setWidth(2);
    _outterborderPen.setStyle(Qt::SolidLine);
    painter->setPen(_outterborderPen);
    if (isSelected()) {
        painter->setPen(QPen(Qt::red, 2, Qt::DashLine));
    }
    QBrush background (Qt::gray, Qt::CrossPattern);
    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->setBrush( background);
    QPointF topLeft (0, 0);
    QPointF bottomRight ( _width, _height );

    QRectF rect (topLeft, bottomRight);
    painter->drawRoundRect(rect,25,25);
    setRect(rect);
}

int GroupItem::getWidth()
{
    return _width;
}

int GroupItem::getHeight()
{
    return _height;
}

void GroupItem::setWidth(qreal newWidth)
{
    _width=newWidth;
}

void GroupItem::setHeight(qreal newHeight)
{
    _height=newHeight;
}

QPointF GroupItem::getLocation()
{
    return _location;
}

QString GroupItem::getIf()
{
    return _ifString;
}

void GroupItem::setIf(QString newIf)
{
    _ifString=newIf;
}

QString GroupItem::getUnless()
{
    return _unlessString;
}

void GroupItem::setUnless(QString newUnless)
{
    _unlessString = newUnless;
}

