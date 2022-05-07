#include "client.hpp"

Client::Client(QObject *parent) : QObject{parent}
{
    socket = new QTcpSocket(this);

    this->connect(this, &Client::new_message, this, &Client::render_message);
    this->connect(socket, &QTcpSocket::readyRead, this, &Client::read_socket);
    this->connect(socket, &QTcpSocket::disconnected, this, &Client::discard_socket);
    this->connect(socket, &QAbstractSocket::errorOccurred, this, &Client::log_error);
}

Client::~Client()
{
    if (socket->isOpen()) {
        socket->close();
    }
}

bool Client::establish_connection(const std::string &ip, const quint16 &port)
{
    this->host = QHostAddress(QString::fromStdString(ip));
    this->port = port;
    this->socket->connectToHost(this->host, this->port);
    this->connected = this->socket->waitForConnected();

    return this->connected;
}

void Client::read_socket()
{
    QByteArray buffer;
    QDataStream socket_stream(this->socket);

    socket_stream.startTransaction();
    socket_stream >> buffer;

    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Client] (read_socket): Read" << buffer.size() << "characters.";

    render_message(QString(buffer));
}

void Client::discard_socket()
{
    this->socket->deleteLater();
    this->socket = nullptr;
    this->status_label->setText("Disconnected.");
}

void Client::render_message(const QString &str)
{
    this->chat_box->moveCursor(QTextCursor::End);
    this->chat_box->insertPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " - <");
    this->chat_box->moveCursor(QTextCursor::End);
    this->chat_box->setTextColor(Qt::blue);
    this->chat_box->insertPlainText(QString::fromStdString(peer_name));
    this->chat_box->moveCursor(QTextCursor::End);
    this->chat_box->setTextColor(Qt::black);
    this->chat_box->insertPlainText(">: " + QString::fromStdString(crypto.decrypt(str.toStdString())) + '\n');
}

void Client::log_error(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Client] (log_error): Host not found.";
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Client] (log_error): " + socket->errorString();
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Client] (log_error): Connection refused.";
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Client] (log_error): " + socket->errorString();
        break;
    default:
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "[Client] (log_error): " + socket->errorString();
        break;
    }
}

bool Client::send_message(std::string &message)
{
    if (socket && socket->isOpen()) {
        QDataStream socket_stream(this->socket);
        QByteArray message_barray = QString::fromStdString(message).toUtf8();
        socket_stream << message_barray;

        return true;
    } else {
        return false;
    }
}

const QHostAddress &Client::get_host() const
{
    return host;
}

void Client::set_host(const QHostAddress &new_host)
{
    host = new_host;
}

quint16 Client::get_port() const
{
    return port;
}

void Client::set_port(quint16 new_port)
{
    port = new_port;
}

bool Client::get_connected() const
{
    return connected;
}

void Client::set_connected(bool new_connected)
{
    connected = new_connected;
}

QLabel *Client::get_status_label() const
{
    return status_label;
}

void Client::set_status_label(QLabel *new_status_label)
{
    status_label = new_status_label;
}

const std::string &Client::get_peer_fpr() const
{
    return peer_fpr;
}

void Client::set_peer_fpr(const std::string &new_peer_fpr)
{
    peer_fpr = new_peer_fpr;
}

const std::string &Client::get_peer_name() const
{
    return peer_name;
}

void Client::set_peer_name(const std::string &new_peer_name)
{
    peer_name = new_peer_name;
}

QTextEdit *Client::get_chat_box() const
{
    return chat_box;
}

void Client::set_chat_box(QTextEdit *new_chat_box)
{
    chat_box = new_chat_box;
}
