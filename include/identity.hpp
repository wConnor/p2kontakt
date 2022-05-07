#ifndef IDENTITY_HPP
#define IDENTITY_HPP

#ifdef _WIN32
    #define IDENTITY_HOME std::filesystem::current_path + std::filesytem::path::preferred_separator + "identities" + std::filesystem::path::preferred_separator;
    #define PUBLIC_KEY_HOME std::filesystem::current_path + std::filesytem::path::preferred_separator + "public_keys" + std::filesystem::path::preferred_separator;
#elif defined __unix__
    #define IDENTITY_HOME std::string(std::getenv("HOME")) + std::filesystem::path::preferred_separator + ".config" + std::filesystem::path::preferred_separator + "p2kontakt" + std::filesystem::path::preferred_separator + "identities" + std::filesystem::path::preferred_separator;
    #define PUBLIC_KEY_HOME std::string(std::getenv("HOME")) + std::filesystem::path::preferred_separator + ".config" + std::filesystem::path::preferred_separator + "p2kontakt" + std::filesystem::path::preferred_separator + "public_keys" + std::filesystem::path::preferred_separator;
#elif defined __APPLE__
    #define IDENTITY_HOME std::string(std::getenv("HOME")) + std::filesystem::path::preferred_separator + ".config" + std::filesystem::path::preferred_separator + "p2kontakt" + std::filesystem::path::preferred_separator + "identities" + std::filesystem::path::preferred_separator;
    #define PUBLIC_KEY_HOME std::string(std::getenv("HOME")) + std::filesystem::path::preferred_separator + ".config" + std::filesystem::path::preferred_separator + "p2kontakt" + std::filesystem::path::preferred_separator + "public_keys" + std::filesystem::path::preferred_separator;
#endif

#include "cryptography.hpp"

#include "network.hpp"
#include "client.hpp"
#include "server.hpp"

#include <gpgme.h>

#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>

#include <QFile>

class Identity
{
public:
    Identity();
    Identity(QFile &id);

    bool create_identity(QString userid, QDateTime expiry_date, bool never_expire, std::string ip_address, bool retrieve_ip);
    void load_identity(QFile &file);
    bool export_contact_file(const std::string &file_path);
    bool add_contact(QFile &file);
    void load_contacts(std::string decrypted_text);
    bool authorise_ip(std::string ip_address, bool retrieve_ip);
    std::string resolve_fingerprint(const std::string &name);
    std::string resolve_name(const std::string &fingerprint);
    bool delete_identity();
    bool prepare_connection(QLabel &client_label, QLabel &server_label, const std::string &recipient_fpr, QTextEdit &chat_box);
    bool send_message(const std::string &message);

    const std::string &get_userid() const;
    void set_userid(const std::string &new_userid);
    const QDate &get_expiry_date() const;
    void set_expiry_date(const QDate &new_expiry_date);
    const std::string &get_ip_address() const;
    void set_ip_address(const std::string &new_ip_address);
    gpgme_key_t get_key() const;
    void set_key(const gpgme_key_t new_key);

    const std::vector<std::pair<std::string, std::string> > &get_contacts_list() const;
    void set_contacts_list(const std::vector<std::pair<std::string, std::string> > &new_contacts_list);

private:
    gpgme_ctx_t context;
    gpgme_key_t key;
    gpgme_key_t keys[2] = {nullptr, nullptr};
    gpgme_key_t recp_key;
    std::vector<std::pair<std::string, std::string>> contacts_list; // fingerprint + ip
    std::string userid;
    QDate expiry_date;
    std::string ip_address;
    std::vector<std::string> allowed_ips;
    std::string opened_file;
    std::filesystem::path identity_dir = IDENTITY_HOME;

    Cryptography crypto;
    Server *server;
    Client *client;
};

#endif // IDENTITY_HPP
