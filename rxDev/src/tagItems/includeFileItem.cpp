

#include "includeFileItem.h"
#include <QDebug>
#include "../rxdev.h"
#include "includeFileEdit.h"
#include "groupItem.h"

IncludeFileItem::IncludeFileItem(QGraphicsRectItem *parent, QGraphicsScene *scene):
    QGraphicsWidget(parent),
    //QGraphicsItem(parent, scene),
    _location(0,0),
    _width(200),
    _gridSpace(10),
    _outterborderColor(Qt::cyan),
    _outterborderPen(),
    _dragStart(0,0),
    _height(60),
    _clearParamsInt(0)
{
    this->setHandlesChildEvents(true);
    _outterborderPen.setWidth(1);
    _outterborderPen.setColor(_outterborderColor);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);

    _title.setPos(-4,_height+2);
    _title.setPlainText("INCLUDE");
    _title.setParentItem(this);
    _title.setRotation(-90);
    _title.setDefaultTextColor(Qt::black);


    _file.setPos(10,-2);
    _file.setTextWidth(_width);
    _file.setParentItem(this);
    _file.setDefaultTextColor(Qt::green);

    _namespace.setPos(10,31);
    _namespace.setTextWidth(_width);
    _namespace.setParentItem(this);
    _namespace.setDefaultTextColor(Qt::red);

    _clearParams.setPos(10,41);
    _clearParams.setTextWidth(_width);
    _clearParams.setParentItem(this);
    _clearParams.setDefaultTextColor(Qt::red);

    updateIncludeFileItem();

}


void IncludeFileItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setColor(Qt::red);
    bool check = getFileData();
    check;
    setColor(Qt::cyan);
    event->setAccepted(true);
}

bool IncludeFileItem::getFileData()
{
    IncludeFileEdit fileEdit(this);
    fileEdit.setWindowTitle("Include file: "+getFile());
    bool accept = fileEdit.exec();
    if ((accept)){
        setFile(fileEdit.getFile());
        setNamespace(fileEdit.getNamespace());
        setClearParams(fileEdit.getClearParam());
        updateIncludeFileItem();
        setIf(fileEdit.getIf());
        setUnless(fileEdit.getUnless());
        QString file(fileEdit.getExpandFile());
        GroupItem *group;
        if (this->parentItem()){
            group =qgraphicsitem_cast<GroupItem *>(this->parentItem());
        }else
            group = new GroupItem;
//        qDebug()<<file;
        if (file!=""){
            if (QFile(file).exists()){
                expandItem(file,*group);
                if (this->parentItem()==0||this->parentItem()==group)
                    scene()->removeItem(this);
            }
        }
        return true;
    } else
        return false;
}

void IncludeFileItem::updateIncludeFileItem()
{
    _file.setHtml("<font size=\"-2\" color=\"black\">file: <font size=\"-2\" color=\"blue\">"+getFile());
    _namespace.setHtml("<font size=\"-2\" color=\"black\">ns: <font size=\"-2\" color=\"red\">"+getNamespace());
    if (_clearParamsInt == 1){
        _clearParams.setHtml("<font size=\"-2\" color=\"black\">clear_params: <font size=\"-2\" color=\"blue\">true");
    }else if(_clearParamsInt == 2){
        _clearParams.setHtml("<font size=\"-2\" color=\"black\">clear_params: <font size=\"-2\" color=\"blue\">false");
    }else{
        _clearParams.setHtml("<font size=\"-2\" color=\"black\">clear_params: <font size=\"-2\" color=\"blue\">");
    }
}

QString IncludeFileItem::getFile()
{
    return _fileString;
}

void IncludeFileItem::setFile(QString newFile)
{
    _fileString = newFile;
}
QString IncludeFileItem::getNamespace()
{
    return _namespaceString;
}
void IncludeFileItem::setNamespace(QString newNamespace)
{
    _namespaceString = newNamespace;
}
int IncludeFileItem::getClearParams()
{
    return _clearParamsInt;
}

void IncludeFileItem::setClearParams(int clear)
{
    _clearParamsInt = clear;
}



void IncludeFileItem::setLocation(QPointF point)
{
    _location=point;
}


void IncludeFileItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
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

void IncludeFileItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    // allow the user to drag the box, capture the starting position on mouse-down

    if (event->button() == Qt::LeftButton) {
        setColor(Qt::red);
        event->setAccepted(true);

    } else
        event->setAccepted(false);


}


void IncludeFileItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    // user have moved the mouse, move the location of the box

    QPointF newPos = event->pos() ;
    _location += (newPos - _dragStart);

    this->setPos(_location);
}

void IncludeFileItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(100);
}
void IncludeFileItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    setZValue(0);
}



void IncludeFileItem::setColor(QColor color)
{
    _outterborderColor = color;
    this->update(0,0,_width,_height);
}

QRectF IncludeFileItem::boundingRect() const
{
    return QRectF(-6,0,_width+15,_height);
}

void IncludeFileItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
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
    painter->setBrush(Qt::gray);
    painter->drawRect(0, 0, _width,_height);

}

void IncludeFileItem::addEnvItem(EnvItem *env)
{
    envItems.append(env);

}

void IncludeFileItem::removeEnvItems()
{
    foreach (EnvItem *env, envItems) {
        removeEnvItem(env);
        delete env;
    }

}

void IncludeFileItem::removeEnvItem(EnvItem *env)
{
    int index = envItems.indexOf(env);

    if (index != -1)
        envItems.removeAt(index);
}


void IncludeFileItem::addArgItem(ArgItem *arg)
{
    argItems.append(arg);

}

void IncludeFileItem::removeArgItems()
{
    foreach (ArgItem *arg, argItems) {
        removeArgItem(arg);
        delete arg;
    }

}

void IncludeFileItem::removeArgItem(ArgItem *arg)
{
    int index = argItems.indexOf(arg);

    if (index != -1)
        argItems.removeAt(index);
}
QString IncludeFileItem::getIf()
{
    return _ifString;
}

void IncludeFileItem::setIf(QString newIf)
{
    _ifString=newIf;
}

QString IncludeFileItem::getUnless()
{
    return _unlessString;
}

void IncludeFileItem::setUnless(QString newUnless)
{
    _unlessString = newUnless;
}
