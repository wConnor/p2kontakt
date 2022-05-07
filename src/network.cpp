#include "network.hpp"

Network::Network()
{

}

std::string Network::get_public_ip()
{
    QEventLoop loop;
    QUrl url("https://www.ifconfig.me/ip");
    std::string ip_address = "";
    manager = new QNetworkAccessManager();

    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(get_public_ip): HTTP response requested from :" << url.url();

    QObject::connect(manager, &QNetworkAccessManager::finished, this, [&ip_address](QNetworkReply *reply) {
        if (reply->error()) {
            qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(get_public_ip): Failed to make HTTP request:" << reply->errorString();
            return;
        }

        ip_address = reply->readAll().toStdString();
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(get_public_ip): Retrieved response:" << QString::fromStdString(ip_address);
    });
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    manager->get(QNetworkRequest(url));

    loop.exec();
    return ip_address;
}

Network::~Network()
{

}
