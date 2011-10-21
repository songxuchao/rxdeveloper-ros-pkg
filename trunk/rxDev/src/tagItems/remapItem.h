
#ifndef REMAPITEM_H
#define REMAPITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QGraphicsWidget>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsPolygonItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE




class RemapItem : public QGraphicsWidget
 {
    Q_OBJECT
 public:
    enum { Type = UserType + 11 };

    RemapItem(QGraphicsRectItem *parent = 0, QGraphicsScene *scene = 0);
    void setLocation(QPointF point);
    void setColor(QColor color);
    int type() const
        { return Type;}
    QGraphicsTextItem _title;
    QGraphicsTextItem _from;    ///< sample text to go in the title area.
    QGraphicsTextItem _to;    ///< sample text to go in the title area.


    QString getFrom();
    void setFrom(QString newFrom);
    QString getTo();
    void setTo(QString newTo);

    QString getIf();
    void setIf(QString newIf);
    QString getUnless();
    void setUnless(QString newUnless);
    bool getRemapData();
    void updateRemapItem();

 protected:
    QPointF _location;
    qreal   _width;
    int     _gridSpace;
    QColor  _outterborderColor; ///< the hover event handlers will toggle this between red and black
    QPen    _outterborderPen; ///< the pen is used to paint the red/black border
    QPointF _dragStart;
    qreal   _height;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event );
    virtual void mousePressEvent (QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events

    virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
private:

    QString _fromString;
    QString _toString;
    QString _ifString;
    QString _unlessString;

 };

#endif // REMAPITEM_H
