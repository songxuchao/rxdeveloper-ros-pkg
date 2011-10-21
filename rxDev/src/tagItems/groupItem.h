
#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QGraphicsPixmapItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsRectItem;
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

class RemapArrow;

class GroupItem : public QGraphicsRectItem
 {
 public:
    enum { Type = UserType + 5 };
     GroupItem(QGraphicsRectItem *parent = 0, QGraphicsScene *scene = 0);
    void setLocation(QPointF point);
    void setColor(QColor color);
    int type() const
        { return Type;}
    QGraphicsTextItem _title;
    QGraphicsTextItem _namespace;    ///< sample text to go in the title area.
    QGraphicsTextItem _clear_params;    ///< sample text to go in the title area.

    QString getNamespace();
    void setNamespace(QString newNamespace);

    QString getIf();
    void setIf(QString newIf);
    QString getUnless();
    void setUnless(QString newUnless);
    int getClear_params();
    void setClear_params(int clear);
    int getWidth();
    int getHeight();
    void setWidth(qreal newWidth);
    void setHeight(qreal newHeight);

    bool getGroupData();
    void updateGroupItem();
    QPointF getLocation();

protected:
   virtual void mousePressEvent (QGraphicsSceneMouseEvent * event );
   virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );
   virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
   virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
   virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events

   virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView
   virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event );

    QRect createPath();
    QPointF _location;
   qreal   _width;
   int     _gridSpace;
   QColor  _outterborderColor; ///< the hover event handlers will toggle this between red and black
   QPen    _outterborderPen; ///< the pen is used to paint the red/black border
   QPointF _dragStart;
   qreal   _height;
   //virtual ~GroupItem();

private:


   QString _namespaceString;
   int _clear_paramsInt;
   QString _ifString;
   QString _unlessString;


};


#endif // GROUPITEM_H
