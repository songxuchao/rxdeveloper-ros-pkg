#ifndef ARGITEM_H
#define ARGITEM_H

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




class ArgItem : public QGraphicsWidget
 {
    Q_OBJECT
 public:
    enum { Type = UserType + 9 };

     ArgItem(QGraphicsScene *scene = 0);
    void setLocation(QPointF point);
    void setColor(QColor color);
    int type() const
        { return Type;}
    QGraphicsTextItem _title;
    QGraphicsTextItem _name;    ///< sample text to go in the title area.
    QGraphicsTextItem _value;    ///< sample text to go in the title area.
    QGraphicsTextItem _value_or_default;    ///< sample text to go in the title area.

    QString getName();
    void setName(QString newName);
    QString getValue();
    void setValue(QString newValue);
    QString getIf();
    void setIf(QString newIf);
    QString getUnless();
    void setUnless(QString newUnless);
    int getValue_or_default();
    void setValue_or_default(int valueOr);
    bool getArgData();
    void updateArgItem();

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
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
    virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
private:

    QString _nameString;
    QString _valueString;
    QString _ifString;
    QString _unlessString;
    int _value_or_defaultInt;

 };

#endif // ARGITEM_H
