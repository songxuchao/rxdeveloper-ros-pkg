
#ifndef MACHINEITEM_H
#define MACHINEITEM_H

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
#include "envItem.h"

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




class MachineItem : public QGraphicsWidget
 {
    Q_OBJECT
 public:
    enum { Type = UserType + 7 };

     MachineItem(QGraphicsRectItem *parent = 0, QGraphicsScene *scene = 0);
     ~MachineItem();
    void setLocation(QPointF point);
    void setColor(QColor color);
    int type() const
        { return Type;}
    QGraphicsTextItem _title;
    QGraphicsTextItem _name;    ///< sample text to go in the title area.
    QGraphicsTextItem _address;    ///< sample text to go in the title area.
    QGraphicsTextItem _default;    ///< sample text to go in the title area.
    QGraphicsTextItem _timeout;    ///< sample text to go in the title area.

    QString getName();
    void setName(QString newName);
    QString getAddress();
    void setAddress(QString newAddress);
    QString getRos_root();
    void setRos_root(QString newRos_root);
    QString getRos_package_path();
    void setRos_package_path(QString newRos_package_path);
    QString getDefault();
    void setDefault(QString newDefault);
    QString getUser();
    void setUser(QString newUser);
    QString getPassword();
    void setPassword(QString newPassword);
    QString getTimeout();
    void setTimeout(QString newTimeout);
    QString getIf();
    void setIf(QString newIf);
    QString getUnless();
    void setUnless(QString newUnless);

    void removeEnvItem(EnvItem *env);
    void removeEnvItems();
    QList<EnvItem *> envItems;
    void addEnvItem(EnvItem *env);

    bool getMachineData();
    void updateMachineItem();

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
    QString _nameString;
    QString _addressString;
    QString _ros_rootString;
    QString _ros_package_pathString;
    QString _defaultString;
    QString _userString;
    QString _passwordString;
    QString _timeoutString;
    QString _ifString;
    QString _unlessString;
 };

#endif // MachineItem_H
