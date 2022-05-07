#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "cryptography.hpp"

#include <QObject>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QHostAddress>

#include <QDataStream>
#include <QByteArray>
#include <QEventLoop>
#include <QDebug>
#include <QLabel>
#include <QTextEdit>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    bool establish_connection(const std::string &host, const quint16 &port);
    bool send_message(std::string &message);

    const QHostAddress &get_host() const;
    void set_host(const QHostAddress &new_host);
    quint16 get_port() const;
    void set_port(quint16 new_port);
    bool get_connected() const;
    void set_connected(bool new_connected);
    QLabel *get_status_label() const;
    void set_status_label(QLabel *new_status_label);
    const std::string &get_peer_fpr() const;
    void set_peer_fpr(const std::string &new_peer_fpr);
    const std::string &get_peer_name() const;
    void set_peer_name(const std::string &new_peer_name);
    QTextEdit *get_chat_box() const;
    void set_chat_box(QTextEdit *new_chat_box);

private:
    QHostAddress host;
    quint16 port;
    bool connected = false;
    QTcpSocket *socket;
    QLabel *status_label;
    QTextEdit *chat_box;
    std::string peer_name, peer_fpr;

    Cryptography crypto;

private slots:
    void read_socket();
    void discard_socket();
    void render_message(const QString &str);
    void log_error(QAbstractSocket::SocketError error);

signals:
    void new_message(QString);
};

#endif // CLIENT_HPP
