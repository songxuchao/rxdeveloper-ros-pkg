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
        fprintf(stderr, "Usage: a) rxdev\n");
        fprintf(stderr, "       b) rxdev <file.launch>\n");
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
    QString file ="";
    if( argc == 2 )
    {
        if( QString( argv[ 1 ] ) == "--help" ||
            QString( argv[ 1 ] ) == "-help" ||
                QString( argv[ 1 ] ) == "-?" ||
            QString( argv[ 1 ] ) == "-h" )
        {
            error( 0 );
        } else

            if (QString( argv[ 1 ] ).endsWith("launch")){
                file=( argv[ 1 ] );
//                qDebug()<<file;
            }

    }

    QApplication a(argc, argv);
    a.setOrganizationName("ais-bonn");
    a.setApplicationName("rxDeveloper");
    RxDev w;
    if (file!=""){
        if (!file.isEmpty()){
            w.settings.setValue("currentDir", QFileInfo(file).dir().absolutePath());
            if( !QFile::exists( file ) )
            {

                error( 0 );
            }


            TiXmlDocument doc( file.toStdString() );
            bool loadOkay = doc.LoadFile();
            if (loadOkay)
            {
                w.loadDocument( &doc );
                w.setWindowTitle("rxDeveloper - "+file);
            }
            else
            {
                qDebug()<<"\nFailed to load file: "<<file;
            }
        }
    }
    w.show();

    return a.exec();
}

