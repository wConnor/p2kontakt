#include "server.hpp"

Server::Server(QObject *parent)
    : QObject{parent}
{
    this->server = new QTcpServer();
}

bool Server::start_server(const quint16 &port)
{
    this->port = port;
    if (server->listen(QHostAddress::Any, this->port)) {
        connect(server, &QTcpServer::newConnection, this, &Server::new_connection);
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(start_server): Server listening on port " + port;
        return true;
    } else {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(start_server): Failed to start server on port " + port;
        return false;
    }
}

void Server::append_to_connection_list(QTcpSocket *socket)
{
    connection_list.push_back(socket);

    connect(socket, &QTcpSocket::readyRead, this, &Server::read_socket);
    connect(socket, &QTcpSocket::disconnected, this, &Server::discard_socket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &Server::log_error);
}

void Server::new_connection()
{
    while (server->hasPendingConnections()) {
        auto connection = server->nextPendingConnection();
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Server] (new_connection): New connection from "
                              + connection->peerAddress().toString();
        this->append_to_connection_list(connection);
    }
}

void Server::discard_socket()
{
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    auto it = std::find(connection_list.begin(), connection_list.end(), socket);

    if (it != connection_list.end()) {
        connection_list.erase(it);
    }

    socket->deleteLater();
}

void Server::log_error(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Server] (log_error): Host not found.";
        //qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Server] log_error): " + socket->errorString();
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Server] (log_error): Connection refused.";
        //qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Server] (log_error): " + socket->errorString();
        break;
    default:
        //qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Server] (log_error): " + socket->errorString();
        break;
    }
}

void Server::read_socket()
{
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray buffer;
    QDataStream socket_stream(socket);

    socket_stream.startTransaction();
    socket_stream >> buffer;

    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Server] (read_socket): Read" << buffer.size() << "characters.";

    for (auto &c : connection_list) {
        if (c != socket && c->isOpen())  {
            QDataStream dest_socket_stream(c);
            dest_socket_stream << buffer;
            qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Server] (read_socket): Directed message to " + socket->peerAddress().toString();

            return;
        }
    }
    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Server] (read_socket): Message has not been forwarded as other party is unavailable.";
}

quint16 Server::get_port() const
{
    return port;
}

void Server::set_port(quint16 new_port)
{
    port = new_port;
}

QLabel *Server::get_status_label() const
{
    return status_label;
}

void Server::set_status_label(QLabel *new_status_label)
{
    status_label = new_status_label;
}
