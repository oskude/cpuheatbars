#include "server.h"
#include "procstat.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QJsonArray>
#include <QJsonDocument>

QT_USE_NAMESPACE

Server::Server(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Echo Server"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_clients()
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Server::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);
    }
    ps = ProcStat();
}

Server::~Server()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void Server::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);

    m_clients << pSocket;
}

void Server::processTextMessage(QString message)
{
    message = message;
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        QJsonArray jsonData = ps.getStats();
        QJsonDocument doc;
        doc.setArray(jsonData);
        pClient->sendTextMessage(doc.toJson());
    }
}

void Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
