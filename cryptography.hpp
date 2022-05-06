#ifndef CRYPTOGRAPHY_HPP
#define CRYPTOGRAPHY_HPP

#include <gpgme.h>

#include <string>
#include <QDateTime>
#include <QDebug>

class Cryptography
{
public:
    Cryptography();

    std::string encrypt(std::string text, gpgme_key_t *recipients[]);
    std::string decrypt(std::string text);

private:
    gpgme_ctx_t ctx;
};

#endif // CRYPTOGRAPHY_HPP
