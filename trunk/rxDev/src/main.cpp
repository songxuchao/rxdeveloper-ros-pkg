/**
 *   rxDev
 * @file main.cpp
 * main file to start up the GUI.
 *
 *
 *
 *   @author  Filip Müllers  <mueller4@cs.uni-bonn.de>
 *   @date    11.08.2011
 *   @version 1.0a
 *
 */




#include "rxdev.h"
#include <QtGui/QApplication>
#include <QDesktopWidget>

//todo: das gruppen zugehörigkeitszeug über 'collides with' lösen, müsste sauberer sein
using namespace std;

/*!\brief wrong arguments handling
 *
 * Show usage information if GUI is started with wrong arguments.
 */
void error( const char* msg = "Invalid argument" )
{
    if( msg )
    {
        fprintf( stderr, "rxdev: %s\n", msg );
    }
    else
    {
        fprintf(stderr, "Usage: rxdev\n");
    }
    exit( 1 );
}

/*!\brief main
 *
 * Initializes the Application.
 */
int main(int argc, char *argv[])
{

    ros::init(argc, argv, "rxdev");

    if( argc == 2 )
    {
        if( QString( argv[ 1 ] ) == "--help" ||
            QString( argv[ 1 ] ) == "-help" ||
            QString( argv[ 1 ] ) == "-h" )
        {
            error( 0 );
        }
    }

    QApplication a(argc, argv);
    a.setOrganizationName("ais-bonn");
    a.setApplicationName("rxDeveloper");
    RxDev w;

    w.show();

    return a.exec();
}

