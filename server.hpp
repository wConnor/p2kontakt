#ifndef SERVER_HPP
#define SERVER_HPP

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QLabel>
#include <QDateTime>
#include <QDebug>

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool start_server(const quint16 &port);
    void append_to_connection_list(QTcpSocket *socket);
    void direct_message(QTcpSocket *socket, QByteArray &message);

    quint16 get_port() const;
    void set_port(quint16 new_port);
    QLabel *get_label() const;
    void set_label(QLabel *new_label);
    QLabel *get_status_label() const;
    void set_status_label(QLabel *new_status_label);

private:
    QTcpServer *server;
    std::vector<QTcpSocket*> connection_list;
    quint16 port;
    QLabel *status_label;

private slots:
    void new_connection();
    void read_socket();
    void log_error(QAbstractSocket::SocketError error);
    void discard_socket();

signals:

};

#endif // SERVER_HPP
