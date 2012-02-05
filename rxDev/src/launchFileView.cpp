#include "launchFileView.h"
#include <QDebug>
#include "tagItems/remapArrow.h"
#include "rxdev.h"
#include "tagItems/remapArrowEdit.h"
#include "tagItems/groupItem.h"
#include "tagItems/launchEdit.h"


/*!\brief Graphicsview with special features
 *
 * initializes the extended QGraphicsView
 */
LaunchFileView::LaunchFileView(QWidget *parent) : QGraphicsView(parent),
    _handScrolling(false),
    deprecated(false),
    message("")
{
    setAcceptDrops(true);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::RubberBandDrag);
    line = new QGraphicsLineItem();
    // Comment the line below, and drop starts working
    //setScene(new QGraphicsScene(this));

    // Initialize scaling box
    comboBox_sceneScale = new QComboBox;
    QStringList scales;
    scales << tr("20%") //elem 0
           << tr("25%")
           << tr("30%")
           << tr("35%")
           << tr("40%")
           << tr("45%")
           << tr("50%")
           << tr("55%")
           << tr("60%")
           << tr("65%")
           << tr("70%")
           << tr("75%")
           << tr("80%")
           << tr("85%")
           << tr("90%")
           << tr("95%")
           << tr("100%") //elem 16
           << tr("115%")
           << tr("125%")
           << tr("150%")
           << tr("175%")
           << tr("200%")
           << tr("250%")
           << tr("300%"); ////elem 23
    comboBox_sceneScale->addItems(scales);
    comboBox_sceneScale->setEditable(true);
    comboBox_sceneScale->setCurrentIndex(16);


    //is needed for program startup, otherwise program can crash
    selectedBox = new NodeItem("dummyname","dummypkg",QStringList(""),QStringList(""),QStringList(""),QStringList(""));
}

/*!\brief reimplementation of the wheelevent
 *
 * allows scaling of the view if the control-key is pressed
 */
void LaunchFileView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {

        if (event->delta() /240.0 >0 &&comboBox_sceneScale->currentIndex()<23){
            comboBox_sceneScale->setCurrentIndex(comboBox_sceneScale->currentIndex()+1);
        }else if (event->delta() /240.0 <0 && comboBox_sceneScale->currentIndex()>0){
            comboBox_sceneScale->setCurrentIndex(comboBox_sceneScale->currentIndex()-1);
        }


    } else {
        QGraphicsView::wheelEvent(event);
    }
}


void LaunchFileView::dragEnterEvent(QDragEnterEvent *event) {

    event->accept();
}

/*!\brief reimplementation of the dropEvent
 *
 * Creates a node item if a node is dropped in the view.
 * The neccessary nodeinformation is fetched during the mousepressaction for the drag event
 * in RxDev::selectionHandle_availableNodes.
 */
void LaunchFileView::dropEvent(QDropEvent *event) {
    NodeItem * newNode;
    newNode = new NodeItem(selectedNodeName,selectedNodePackage,selectedNodeSubscriptions,
                           selectedNodePublications,selectedNodeServices,selectedNodeParameters);
    newNode->setLocation(mapToScene(event->pos()));

    newNode->setPos(mapToScene(event->pos()));

    if (newNode->getNodeData()==true){
        scene()->addItem(newNode);
    }

    selectedBox->setColor(Qt::black);
    selectedBox= newNode;
}


void LaunchFileView::dragMoveEvent(QDragMoveEvent *event){

    //   qDebug() << event;

}

/*!\brief reimplementation of the mousePressEvent
 *
 * ...
 */
void LaunchFileView::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::MidButton) {
        _handScrolling = true;
        _handScrollingOrigin = QPoint(event->pos());
        _handScrollingValueX = horizontalScrollBar()->value();
        _handScrollingValueY = verticalScrollBar()->value();
        _savedCursor = viewport()->cursor();
        viewport()->setCursor(Qt::ClosedHandCursor);
        event->accept();
        QGraphicsView::mousePressEvent(event);
        return;
    }
    if (itemAt((event->pos()))){ //else tool crashes on first click
        if (event->button() == Qt::LeftButton){
            if (selectedBox){
                selectedBox->setZValue(0);
                selectedBox->setColor(Qt::black);
                if (itemAt((event->pos()))->type() == NodeItem::Type) {  //NodeItem::Type means the node itself
                    selectedBox= qgraphicsitem_cast<NodeItem *>(itemAt((event->pos())));
                }else if (itemAt(event->pos())->type() == 8 && itemAt(event->pos())->parentItem()->type() == NodeItem::Type){   //8 means the textitem in the node
                    selectedBox= qgraphicsitem_cast<NodeItem *>(itemAt((event->pos()))->parentItem());
                }
                selectedBox->setZValue(100);
            }
            switch (myMode) {
            case DragItem:

                QGraphicsView::mousePressEvent(event);
                event->setAccepted(true);// tell the base class we are handling this event

                break;
                //! [6] //! [7]
            case InsertLine:
                if (itemAt((event->pos()))->type() == NodeItem::Type || (itemAt(event->pos())->type() == 8 && itemAt(event->pos())->parentItem()->type() == NodeItem::Type)) {
                    line = new QGraphicsLineItem(QLineF(mapToScene(event->pos()),
                                                        mapToScene(event->pos())));

                    scene()->addItem(line);
                }

                event->setAccepted(true);// tell the base class we are handling this event

                QGraphicsView::mousePressEvent(event);
                break;
            case DeleteItem:

                //qDebug()<<itemAt((event->pos()));


                if (itemAt((event->pos()))->type() == 65550) { //Arrowtype RemapArrow::Type
                    RemapArrow *arrow = qgraphicsitem_cast<RemapArrow *>(itemAt((event->pos())));
//                    RemapItem *item;
//                    for (int i=0; i< arrow->startItem()->remapItems.count();i++){
//                        item = arrow->startItem()->remapItems.at(i);


//                        if (item->getFrom()==arrow->getFrom() && item->getTo()==arrow->getTo()){
//                            arrow->startItem()->removeRemapItem(item);
//                        }

//                    }
                    arrow->startItem()->removeArrow(arrow);
                    arrow->endItem()->removeArrow(arrow);
                    scene()->removeItem(itemAt((event->pos())));
                    delete itemAt((event->pos()));
                }
                event->accept();
                break;
            default:
                break;
            }
        }
    }else{
    }


}

/*!\brief reimplementation of the mouseMoveEvent
 *
 * ...
 */
void LaunchFileView::mouseMoveEvent(QMouseEvent *event)
{
    /// Otherwise we get SegFault after adding an item and selecting Linemode before clicking once to the graphicsview


    if (_handScrolling) {
        QPoint delta = event->pos() - _handScrollingOrigin;
        horizontalScrollBar()->setValue(_handScrollingValueX - delta.x());
        verticalScrollBar()->setValue(_handScrollingValueY - delta.y());
        event->accept();
        return;
    }else if (myMode == InsertLine && line != 0) {

        QLineF newLine(line->line().p1(), mapToScene(event->pos()));
        line->setLine(newLine);

    } else if (myMode == DragItem) {
        QGraphicsView::mouseMoveEvent(event);
    } else{
        event->setAccepted(false);
    }
}


/*!\brief reimplementation of the mouseReleaseEvent
 *
 * ...
 */
void LaunchFileView::mouseReleaseEvent(QMouseEvent *event)
{

    if (_handScrolling) {
        _handScrolling = false;
        viewport()->setCursor(_savedCursor);

        event->accept();
        return;
    }
    if (itemAt((event->pos()))){ //else tool crashes
        if (myMode== DeleteItem && event->button() == Qt::LeftButton) {

            if (itemAt(event->pos())->type() == NodeItem::Type) {

                qgraphicsitem_cast<NodeItem *>(itemAt(event->pos()))->removeArrows();
                scene()->removeItem(itemAt(event->pos())); ///removes the Node from the scene
            } else if (itemAt(event->pos())->type() == 8){  // Don't try to kill the View
                if (itemAt(event->pos())->parentItem()->type() == NodeItem::Type){
                    qgraphicsitem_cast<NodeItem *>((itemAt(event->pos()))->parentItem())->removeArrows();
                    scene()->removeItem((itemAt(event->pos()))->parentItem()); ///removes the Node from the scene
                }else
                    scene()->removeItem((itemAt(event->pos()))->parentItem()); ///removes the Parameter

            }else if(itemAt(event->pos())->type() == GroupItem::Type) {
                QList<QGraphicsItem *> list;
                list=itemAt(event->pos())->childItems();
                //qDebug()<<"Begin group items->";

                for (int i = 0; i < list.size(); i++) {
                    if (list.at(i)->type() != 8)

                        (list.at(i))->setParentItem(0);
                  //  qDebug()<<(list.at(i)->type());
                }
                //qDebug()<<"<-End group items";
                scene()->removeItem(itemAt(event->pos()));
            }


            event->setAccepted(true);// tell the base class we are handling this event


        }
    }if (line != 0 && myMode == InsertLine) {

            QList<QGraphicsItem *> startItems = scene()->items(line->line().p1());
            if (startItems.count() && startItems.first() == line)
                startItems.removeFirst();

            QList<QGraphicsItem *> endItems = scene()->items(line->line().p2());
            if (endItems.count() && endItems.first() == line)
                endItems.removeFirst();

            if (startItems.count() &&startItems.first()->type() == 8)
                startItems.first() = startItems.first()->parentItem();
            if (endItems.count() && endItems.first()->type() == 8)
                endItems.first() = endItems.first()->parentItem();


            scene()->removeItem(line); //remove temporary line
            delete line;
            //! [11] //! [12]



            if (startItems.count() > 0 && endItems.count() > 0 &&
                    startItems.first()->type() == NodeItem::Type &&
                    endItems.first()->type() == NodeItem::Type &&
                    startItems.first() != endItems.first()) {
                NodeItem *startItem =
                        qgraphicsitem_cast<NodeItem *>(startItems.first());
                NodeItem *endItem =
                        qgraphicsitem_cast<NodeItem *>(endItems.first());


                RemapArrow *arrow = new RemapArrow(startItem, endItem);
                if (arrow->getArrowData()==true){

                    startItem->addArrow(arrow);
                    endItem->addArrow(arrow);
                    arrow->setZValue(-20);
                    scene()->addItem(arrow);
                    arrow->updatePosition();
                }
            }
        }

    //! [12] //! [13]
    line = 0;

    QGraphicsView::mouseReleaseEvent(event);
}

/*!\brief change mouse mode
 *
 * changes the mode for the mouse.
 */
void LaunchFileView::setMode(Mode mode)
{
    myMode = mode;
}

void LaunchFileView::mouseDoubleClickEvent(QMouseEvent *event)
{
        if(itemAt(event->pos())){
            QGraphicsView::mouseDoubleClickEvent(event);
        }
        else{
            LaunchEdit launch(this);
            launch.setWindowTitle("Launch:");
            bool accept = launch.exec();
            if (accept){
                if (launch.getDeprecated()){
                    setDeprecated(true);
                    setMessage(launch.getMessage());
                }
                else{
                    setDeprecated(false);
                    setMessage("");
                }

            }


            event->accept();
        }



}

void LaunchFileView::setDeprecated(bool depri)
{
    deprecated=depri;
}

void LaunchFileView::setMessage(QString new_message)
{
    message=new_message;
}

bool LaunchFileView::getDeprecated()
{
    return deprecated;
}

QString LaunchFileView::getMessage()
{
    return message;
}






