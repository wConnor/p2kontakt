#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <QObject>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>

#include <QEventLoop>
#include <QDebug>
#include <QDateTime>

class Network : public QObject
{
    Q_OBJECT

public:
    Network();
    ~Network();

    std::string get_public_ip();
    bool establish_connection(const std::string &ip_address, const std::string &port);
    bool send(const std::string &message, const std::string &ip);

    const std::string &get_connected_ip() const;
    void set_connected_ip(const std::string &new_connected_ip);
    const std::string &get_connected_port() const;
    void set_connected_port(const std::string &new_connected_port);
    bool get_server_node() const;
    void set_server_node(bool new_server_node);

private:
    QNetworkAccessManager *manager;
    std::string connected_ip;
    std::string connected_port;
    bool server_node;
};

#endif // NETWORK_HPP
