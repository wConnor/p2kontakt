#include "identity.hpp"

Identity::Identity()
{

}

Identity::Identity(QFile &file)
{
    load_identity(file);
}

bool Identity::create_identity(QString userid, QDateTime expiry_date, bool never_expire, std::string ip_address, bool retrieve_ip)
{
    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") <<
                "(create_identity): Requested to create an identity with parameters: \n"
                "\tUser ID: " + userid + "\n"
                "\tIP Address: " + QString::fromStdString(ip_address) + "\n"
                "\tRetrieve IP?: " + QVariant(retrieve_ip).toString() + "\n"
                "\tExpiry Date: " + expiry_date.toString() + "\n"
                "\tNever Expire?: " + QVariant(never_expire).toString();

    if (userid.isEmpty()) {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << ": Failed to create identity as the userid field is empty.";
        return false;
    }

    gpgme_error_t err;

    gpgme_check_version(nullptr);
    err = gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP);
    err = gpgme_new(&context);
    err = gpgme_set_protocol(context, GPGME_PROTOCOL_OpenPGP);

    never_expire ?
            err = gpgme_op_createkey(context, userid.toStdString().c_str(), nullptr, 0, 0, nullptr, GPGME_CREATE_NOEXPIRE) :
            err = gpgme_op_createkey(context, userid.toStdString().c_str(), nullptr, 0, expiry_date.toSecsSinceEpoch() - QDateTime::currentDateTime().toSecsSinceEpoch(), nullptr, 0);

    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(create_identity): gpgme_op_createkey result: " << gpgme_strerror(err);

    gpgme_genkey_result_t result;
    result = gpgme_op_genkey_result(context);

    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(create_identity): Generated key fingerprint: " << result->fpr;

    std::string file_name = identity_dir.u8string() + userid.toStdString() + '-' + QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd").toStdString() + ".p2i";

    gpgme_get_key(context, result->fpr, &keys[0], 0);
    this->key = keys[0];

    Network *network = new Network();
    if (retrieve_ip)
        ip_address = network->get_public_ip() + ',';

    std::ofstream identity_file;
    gpgme_key_t *recp = new gpgme_key_t[2];
    recp[0] = key;
    recp[1] = nullptr;
    identity_file.open(file_name);
    identity_file << result->fpr << '\n'
                  << crypto.encrypt(ip_address, &recp) << '\n';
    identity_file.close();

    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(create_identity): Written identity file to" << QString::fromStdString(file_name);

    this->userid = userid.toStdString();
    this->ip_address = ip_address;

    delete network;

    QFile file(QString::fromStdString(file_name));
    load_identity(file);

    return true;
}

void Identity::load_identity(QFile &file)
{
    gpgme_error_t err;

    QTextStream in(&file);
    QString fingerprint = in.readLine();
    std::string encrypted_text = "";
    opened_file = file.fileName().toStdString();
    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(Identity): Identity with fingerprint" << fingerprint << "requested for loading.";

    while(!in.atEnd())
        encrypted_text += in.readLine().toStdString() + '\n';

    gpgme_check_version(nullptr);

    err = gpgme_new(&context);
    err = gpgme_op_keylist_start(context, fingerprint.toStdString().c_str(), 1);
    err = gpgme_op_keylist_next(context, &key);
    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(Identity): gpgme_op_keylist_next result: " << gpgme_strerror(err);

    if (gpg_err_code(err) == GPG_ERR_EOF) {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(Identity): Key not found.";
    }
\
    std::string userid = key->uids->name, found_fingerprint = key->fpr;

    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(Identity): Loaded identity of user \'" << QString::fromStdString(userid) << "' (" << QString::fromStdString(found_fingerprint) << ')';

    this->keys[0] = key;

    std::string decrypted_text = crypto.decrypt(encrypted_text);

    load_contacts(decrypted_text);

    this->userid = userid;
}

bool Identity::export_contact_file(const std::string &file_path)
{
    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(export_public_key): Export of public key for" << key->fpr << "requested.";

    gpgme_data_t data;
    gpgme_error_t err;

    gpgme_check_version(nullptr);
    gpgme_set_protocol(context, GPGME_PROTOCOL_OpenPGP);
    gpgme_set_armor(context, 1);

    err = gpgme_data_new(&data);
    err = gpgme_op_export(context, key->fpr, 0, data);
    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(export_public_key): gpgme_op_export result:" << gpgme_strerror(err);

    char *buffer = new char[4096];
    gpgme_data_seek(data, 0, SEEK_SET);
    gpgme_data_read(data, buffer, 4096);

    std::ofstream exported_file;
    exported_file.open(file_path);
    exported_file << this->key->fpr << '\n'
                  << this->get_ip_address() << '\n'
                  << buffer;
    exported_file.close();

    delete[] buffer;

    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(export_public_key): Exported public key to file" << QString::fromStdString(file_path);

    return true;
}

bool Identity::add_contact(QFile &file)
{
    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(add_contact): Addition of contact from file" << file.fileName() << "requested.";

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << file.errorString();
        return false;
    }

    QTextStream in(&file);
    std::string contact_fpr = in.readLine().toStdString();
    std::string contact_ip = in.readLine().toStdString();
    std::string key_text = "";

    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(add_contact): Fingerprint of contact: " << QString::fromStdString(contact_fpr);

    while (!in.atEnd()) {
        key_text += in.readLine().toStdString() + '\n';
    }

    gpgme_data_t data;
    gpgme_error_t err;

    gpgme_check_version(nullptr);
    gpgme_set_protocol(context, GPGME_PROTOCOL_OpenPGP);
    gpgme_set_armor(context, 1);

    err = gpgme_data_new_from_mem(&data, key_text.c_str(), key_text.size(), 1);
    err = gpgme_op_import(context, data);
    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(add_contact): gpgme_op_import result:" << gpgme_strerror(err);

    err = gpgme_op_keysign(this->context, this->key, contact_fpr.c_str(), 0, 0);
    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(add_contact): gpgme_op_keysign result:" << gpgme_strerror(err);

    QFile identity_file(QString::fromStdString(opened_file));

    if (!identity_file.open(QIODevice::ReadOnly)) {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << identity_file.errorString();
        return false;
    }

    QTextStream id_in(&identity_file);
    std::string fingerprint = id_in.readLine().toStdString();
    std::string encrypted_text = "";
    std::string decrypted_text = "";

    while (!id_in.atEnd())
        encrypted_text += id_in.readLine().toStdString() + '\n';

    decrypted_text = crypto.decrypt(encrypted_text);
    std::string text_to_encrypt;

    load_contacts(decrypted_text);
    contacts_list.push_back(std::make_pair(contact_fpr, contact_ip));

    text_to_encrypt = ip_address + '\n';
    for (auto &c : contacts_list) {
        text_to_encrypt += c.first + ',' + c.second + '\n';
    }

    gpgme_key_t *recp = new gpgme_key_t[2];
    recp[0] = key;
    recp[1] = nullptr;

    identity_file.close();
    identity_file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    id_in << key->fpr << '\n' << QString::fromStdString(crypto.encrypt(text_to_encrypt, &recp));
    identity_file.flush();

    delete[] recp;

    return true;
}

void Identity::load_contacts(std::string decrypted_text)
{
    contacts_list.clear();
    std::istringstream iss(decrypted_text);
    std::string line;

    std::getline(iss, this->ip_address);
    std::array<std::string, 2> strings_temp;
    while (std::getline(iss, line)) {
        std::stringstream ss(line);
        int ctr = 0;
        while (ss.good()) {
            std::string substr;
            std::getline(ss, substr, ',');
            strings_temp[ctr] = substr;
            ++ctr;
        }
        contacts_list.push_back(std::make_pair(strings_temp[0], strings_temp[1]));
    }
}

bool Identity::authorise_ip(std::string ip_address, bool retrieve_ip)
{
    QFile identity_file(QString::fromStdString(opened_file));
    QTextStream in(&identity_file);

    if (!identity_file.open(QIODevice::ReadOnly)) {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << identity_file.errorString();
        return false;
    }

    std::string fingerprint = in.readLine().toStdString();
    std::string encrypted_text = "", decrypted_text = "";

    while (!in.atEnd())
        encrypted_text += in.readLine().toStdString() + '\n';

    decrypted_text = crypto.decrypt(encrypted_text);

    std::istringstream iss(decrypted_text);
    std::string line, ip_line;
    std::getline(iss, ip_line);
    std::stringstream ss(ip_line);

    std::vector<std::string> ip_list = {};
    while (ss.good()) {
        std::string substr;
        std::getline(ss, substr, ',');
        ip_list.push_back(substr);
    }

    if (retrieve_ip) {
        Network network;
        ip_address = network.get_public_ip();
    }

    ip_list.push_back(ip_address);
    allowed_ips = ip_list;

    std::string text_to_encrypt = "";

    for (auto &c : allowed_ips)
        text_to_encrypt += c + ',';

    text_to_encrypt += '\n';
    load_contacts(decrypted_text);

    for (auto &c : contacts_list)
        text_to_encrypt += c.first + ',' + c.second + '\n';

    gpgme_key_t *recipients = new gpgme_key_t[2];
    recipients[0] = key;
    recipients[1] = nullptr;

    QTextStream id_in(&identity_file);
    identity_file.close();
    identity_file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    id_in << QString::fromStdString(fingerprint) << '\n' << QString::fromStdString(crypto.encrypt(text_to_encrypt, &recipients));
    identity_file.flush();

    return true;
}

std::string Identity::resolve_name(const std::string &fingerprint)
{
    gpgme_error_t err;
    gpgme_key_t contact_key;

    err = gpgme_op_keylist_start(context, fingerprint.c_str(), 1);
    err = gpgme_op_keylist_next(context, &contact_key);

    return !err ? std::string(contact_key->uids->uid) : "";
}

std::string Identity::resolve_fingerprint(const std::string &name)
{
    gpgme_error_t err;
    gpgme_key_t contact_key;

    err = gpgme_op_keylist_start(context, name.c_str(), 1);
    err = gpgme_op_keylist_next(context, &contact_key);

    return !err ? std::string(contact_key->fpr) : "";
}

bool Identity::delete_identity()
{
    QFile identity_file(QString::fromStdString(opened_file));
    return identity_file.remove();
}

bool Identity::prepare_connection(QLabel &client_label, QLabel &server_label, const std::string &recipient_fpr, QTextEdit &chat_box)
{
    gpgme_op_keylist_start(context, recipient_fpr.c_str(), 0);
    gpgme_op_keylist_next(context, &this->recp_key);

    std::string recp_ip = "";
    for (auto &c : this->contacts_list) {
        if (c.first == recipient_fpr) {
            recp_ip = c.second;
        }
    }

    while (recp_ip.back() == ',') {
        recp_ip.pop_back();
    }

    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(prepare_connection): User" << this->key->fpr << "requested chat with "
                       << this->recp_key->fpr;

    std::string current_fpr = this->key->fpr;
    int comparison = current_fpr.compare(this->recp_key->fpr);

    if (this->ip_address == recp_ip) {
        recp_ip = "127.0.0.1";
    }

    /* compares whether the current fingerprint is lexicographically greater than the recipient.
       if so, the current user is selected as the host and the server is started.
       else, the current user is simply a client and is to connect to the other party's server */
    if (comparison > 0) {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(prepare_connection): Selected this peer as host.";
        this->server = new Server();
        server_label.setText("HOST");
        this->server->set_status_label(&server_label);
        this->server->start_server(9736);
        recp_ip = "127.0.0.1";
    }

    /* removes any possible leading commas after decryption. */
    while (ip_address.back() == ',') {
        ip_address.pop_back();
    }

    this->client = new Client();
    this->client->set_peer_name(std::string(this->recp_key->uids->uid));
    this->client->set_peer_fpr(recipient_fpr);
    this->client->establish_connection(recp_ip, 9736) ?
                qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(prepare_connection): Client connection established to " + QString::fromStdString(recp_ip) + "." :
                qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(prepare_connection): Failed to establish connection with " + QString::fromStdString(recp_ip) + ".";
    this->client->set_chat_box(&chat_box);
    this->client->set_status_label(&client_label);

    return true;
}

bool Identity::send_message(const std::string &message)
{
    gpgme_key_t *recipients = new gpgme_key_t[3];
    recipients[0] = this->key;
    recipients[1] = this->recp_key;
    recipients[2] = nullptr;

    std::string encrypted_message = crypto.encrypt(message, &recipients);

    delete[] recipients;

    return this->client->send_message(encrypted_message);
}

const std::string &Identity::get_userid() const
{
    return userid;
}

void Identity::set_userid(const std::string &new_userid)
{
    userid = new_userid;
}

const QDate &Identity::get_expiry_date() const
{
    return expiry_date;
}

void Identity::set_expiry_date(const QDate &new_expiry_date)
{
    expiry_date = new_expiry_date;
}

const std::string &Identity::get_ip_address() const
{
    return ip_address;
}

void Identity::set_ip_address(const std::string &new_ip_address)
{
    ip_address = new_ip_address;
}

gpgme_key_t Identity::get_key() const
{
    return key;
}

void Identity::set_key(const gpgme_key_t new_key)
{
    key = new_key;
}

const std::vector<std::pair<std::string, std::string> > &Identity::get_contacts_list() const
{
    return contacts_list;
}

void Identity::set_contacts_list(const std::vector<std::pair<std::string, std::string> > &new_contacts_list)
{
    contacts_list = new_contacts_list;
}
