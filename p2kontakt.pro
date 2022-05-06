QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutwindow.cpp \
    addcontactdialog.cpp \
    authoriseipdialog.cpp \
    client.cpp \
    cryptography.cpp \
    decryptdialog.cpp \
    encryptdialog.cpp \
    exportdialog.cpp \
    identity.cpp \
    identitydialog.cpp \
    main.cpp \
    mainwindow.cpp \
    network.cpp \
    server.cpp

HEADERS += \
    aboutwindow.hpp \
    addcontactdialog.hpp \
    authoriseipdialog.hpp \
    client.hpp \
    cryptography.hpp \
    decryptdialog.hpp \
    encryptdialog.hpp \
    exportdialog.hpp \
    identity.hpp \
    identitydialog.hpp \
    mainwindow.hpp \
    network.hpp \
    server.hpp

FORMS += \
    aboutwindow.ui \
    addcontactdialog.ui \
    authoriseipdialog.ui \
    decryptdialog.ui \
    encryptdialog.ui \
    exportdialog.ui \
    identitydialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    p2kontakt_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += gpgme
