/*
  * Original Code Copyright 2010 by David W. Drell
  * Taken from ...
  * 
  */

#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsPolygonItem>
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
#include "envItem.h"
#include "parameterItem.h"
#include "rosparamItem.h"
#include "remapItem.h"



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

class RemapArrow;


class NodeItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 4 };

    NodeItem(QString node, QString package, QStringList subscriptions,QStringList publications,QStringList services,QStringList arguments, QGraphicsPolygonItem *parent = 0, QGraphicsScene *scene = 0);
    QGraphicsTextItem _title;    ///< sample text to go in the title area.
    QGraphicsTextItem _name;    ///< sample text to go in the title area.
    QGraphicsTextItem _namespace;    ///< sample text to go in the title area.
    QGraphicsTextItem _contains;
    void setGridSpace(int space);
    void setColor(QColor color);
    void setLocation(QPointF point);
    QPointF getLocation();
    QPixmap image() const;
    int type() const
    { return Type;}

    void removeArrow(RemapArrow *arrow);
    void removeArrows();
    QList<RemapArrow *> arrows;
    void addArrow(RemapArrow *arrow);

    void removeRemapItem(RemapItem *remap);
    void removeRemapItems();
    QList<RemapItem *> remapItems;
    void addRemapItem(RemapItem *remap);

    QString getName();
    void setName(QString newName);
    QString getPkg();
    void setPkg(QString newPackage);
    QString getType();
    void setType(QString type);
    QStringList getSubscriptions();
    QStringList getPublications();
    QStringList getServices();
    QStringList getParameters();
    QString getNamespace();
    void setNamespace(QString newNamespace);
    QString getArgs();
    void setArgs(QString newArgs);
    QString getMachine();
    void setMachine(QString newMachine);
    QString getTime_limit();
    void setTime_limit(QString newTime_limit);
    QString getRetry();
    void setRetry(QString newRetry);
    QString getLaunch_prefix();
    void setLaunch_prefix(QString newLaunch_prefix);
    QString getIf();
    void setIf(QString newIf);
    QString getUnless();
    void setUnless(QString newUnless);
    int getRequired();
    void setRequired(int required);
    int getRespawn();
    void setRespawn(int respawn);
    int getClear_params();
    void setClear_params(int clear);
    int getOutput();
    void setOutput(int output);
    int getCwd();
    void setCwd(int cwd);
    int getNode_or_test();
    void setNode_or_test(int set);
    bool getNodeData();
    void updateNodeItem();
    void updateContains();
    void removeEnvItem(EnvItem *env);
    void removeEnvItems();
    QList<EnvItem *> envItems;
    void addEnvItem(EnvItem *env);

    void removeParamItem(ParameterItem *param);
    void removeParamItems();
    QList<ParameterItem *> paramItems;
    void addParamItem(ParameterItem *param);

    void removeRosparamItem(RosparamItem *rosparam);
    void removeRosparamItems();
    QList<RosparamItem *> rosparamItems;
    void addRosparamItem(RosparamItem *rosparam);

protected:

    virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events
    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent * event );
    virtual void mousePressEvent(QGraphicsSceneDragDropEvent *event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event );


    QColor  _outterborderColor; ///< the hover event handlers will toggle this between red and black
    QPen    _outterborderPen; ///< the pen is used to paint the red/black border
    QPointF _location;
    QPointF _dragStart;
    int     _gridSpace;
    qreal   _width;
    qreal   _height;


    QVariant itemChange(GraphicsItemChange change, const QVariant &value);



    virtual ~NodeItem();

signals:

private:
    int _node_or_testInt;
    QPolygonF myPolygon;
    QString _nameString;
    QString _typeString;
    QString _pkgString;
    QStringList _subscriptionsStringList;
    QStringList _publicationsStringList;
    QStringList _servicesStringList;
    QStringList _parametersStringList;
    QString _argsString;
    QString _namespaceString;
    QString _machineString;
    QString _time_limitString;
    QString _retryString;
    QString _launch_prefixString;
    QString _containsString;
    int _requiredInt;
    int _respawnInt;
    int _clear_paramsInt;
    int _outputInt;
    int _cwdInt;
    bool _blank;
    QString _ifString;
    QString _unlessString;

};

#endif // NODEITEM_H
