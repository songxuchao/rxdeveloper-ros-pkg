#include "launchFileScene.h"
#include <QDebug>
#include "rxdev.h"



LaunchFileScene::LaunchFileScene(QObject *parent,int w, int h) : QGraphicsScene(parent),
    _width(w), _height(h)
{

}

void LaunchFileScene::handleWindowSizeChanged(int w, int h)
{
    _width = w;
    _height = h;
}




