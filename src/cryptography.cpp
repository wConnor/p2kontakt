#include "cryptography.hpp"

Cryptography::Cryptography()
{
    gpgme_new(&ctx);
    gpgme_check_version(nullptr);
    gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP);
}

std::string Cryptography::encrypt(std::string text, gpgme_key_t *recipients[])
{
    gpgme_error_t err;
    gpgme_data_t input, output;

    if (recipients[0] == nullptr) {
        qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(Cryptography::encrypt): Unable to encrypt as a key is not loaded.";
        return "";
    }

    gpgme_set_protocol(ctx, GPGME_PROTOCOL_OpenPGP);
    gpgme_set_armor(ctx, 1);

    err = gpgme_data_new_from_mem(&input, text.c_str(), text.size(), 1);
    err = gpgme_data_new(&output);
    err = gpgme_op_encrypt(ctx, *recipients, GPGME_ENCRYPT_ALWAYS_TRUST, input, output);
    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(Cryptography::encrypt): gpgme_op_encrypt result: " << gpgme_strerror(err);

    if (err)
        return "";

    char *buffer = new char[4096];
    gpgme_data_seek(output, 0, SEEK_SET);
    gpgme_data_read(output, buffer, 4096);

    gpgme_data_release(input);
    gpgme_data_release(output);

    std::string buffer_str = buffer;
    delete[] buffer;

    return buffer_str;
}

std::string Cryptography::decrypt(std::string text)
{
    gpgme_error_t err;
    gpgme_data_t input, output;

    gpgme_set_protocol(ctx, GPGME_PROTOCOL_OpenPGP);
    gpgme_set_armor(ctx, 1);

    err = gpgme_data_new_from_mem(&input, text.c_str(), text.size(), 1);
    err = gpgme_data_new(&output);
    err = gpgme_op_decrypt(ctx, input, output);
    qDebug().noquote() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "(Cryptography::decrypt): gpgme_op_decrypt result: " << gpgme_strerror(err);

    if (err)
        return "";

    char *buffer = new char[4096];
    gpgme_data_seek(output, 0, SEEK_SET);
    gpgme_data_read(output, buffer, 4096);

    gpgme_data_release(input);
    gpgme_data_release(output);

    std::string buffer_str = buffer;
    delete[] buffer;

    return buffer_str;
}

