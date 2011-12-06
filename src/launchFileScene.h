#ifndef LAUNCHFILESCENE_H
#define LAUNCHFILESCENE_H

#include <QGraphicsScene>
#include <QPainter>
#include <math.h>
#include <QRectF>


class LaunchFileScene : public QGraphicsScene
{

public:
    LaunchFileScene(QObject *parent = 0,int width=0, int height=0);
    void handleWindowSizeChanged(int w, int h);

private:

    int _width;
    int _height;

protected:

    void drawBackground ( QPainter * painter, const QRectF & rect )
    {

        const int pointBufferSize = 5000;
        static QPoint pointBuffer[pointBufferSize];

        int gridSize = 10;
        int x0 = gridSize * floor(rect.left() / gridSize);
        int y0 = gridSize * floor(rect.top() / gridSize);
        int x1 = gridSize * ceil(rect.right() / gridSize);
        int y1 = gridSize * ceil(rect.bottom() / gridSize);
        painter->save();
        QPen pen;
        pen.setColor(QColor(185, 185, 185));
        pen.setWidth(0);
        painter->setPen(pen);
        painter->setRenderHint(QPainter::Antialiasing, false);
        int pointsUsed = 0;
        for (int x = x0; x < x1; x += gridSize) {
            for (int y = y0; y < y1; y += gridSize) {
                pointBuffer[pointsUsed].setX(x);
                pointBuffer[pointsUsed].setY(y);
                pointsUsed++;
                if (pointsUsed == pointBufferSize) {
                    painter->drawPoints(pointBuffer, pointsUsed);
                    pointsUsed = 0;
                }
            }
        }
        if (pointsUsed > 0) {
            painter->drawPoints(pointBuffer, pointsUsed);
            pointsUsed = 0;
        }
        painter->restore();
    }
};



#endif
