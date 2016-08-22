#include "server.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QRegularExpression>

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
    cpuRe.setPattern("^(cpu)([0-9]*)");
    // TODO: ...
    jiffies.resize(4);
    jiffies[0].resize(2);
    jiffies[1].resize(2);
    jiffies[2].resize(2);
    jiffies[3].resize(2);
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
        QJsonArray jsonData;
        QFile file("/proc/stat");
        file.open(QIODevice::ReadOnly);
        QByteArray contents = file.readAll();
        file.close();
        QTextStream in(&contents);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" ");
            QRegularExpressionMatch match = cpuRe.match(parts.at(0));
            if (match.hasMatch()) {
                if (!match.captured(2).isEmpty()) {
                    int cpuNr = match.captured(2).toInt();
                    int jiffTotal = 0;
                    int jiffWork = 0;
                    for (int i = 1; i <= 7; i++){
                        jiffTotal += parts.at(i).toInt();
                        if (i == 3) {
                            jiffWork = jiffTotal;
                        }
                    }
                    int usageTotal = jiffTotal - jiffies[cpuNr][0];
                    int usageWork = jiffWork - jiffies[cpuNr][1];
                    int usage = ((float)usageWork / (float)usageTotal) * 100;
                    jsonData.append(usage);
                    jiffies[cpuNr][0] = jiffTotal;
                    jiffies[cpuNr][1] = jiffWork;
                }
            } else {
                break;
            }
        }
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
