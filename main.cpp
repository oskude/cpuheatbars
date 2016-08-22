#include <QApplication>
#include <QMainWindow>
#include <QWebEngineView>
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow w;
    QWebEngineView view;
    Server *server = new Server(1234);

    QObject::connect(server, &Server::closed, &a, &QCoreApplication::quit);

    w.setWindowFlags(
        Qt::WindowStaysOnTopHint
        | Qt::X11BypassWindowManagerHint
        | Qt::NoDropShadowWindowHint
    );
    w.resize(100, 27);
    w.move(1200,0);
    w.setCentralWidget(&view);
    w.show();

    view.load(QUrl("qrc:/resources/main.html"));

    return a.exec();
}
