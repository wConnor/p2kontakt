QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/aboutwindow.cpp \
    src/addcontactdialog.cpp \
    src/authoriseipdialog.cpp \
    src/client.cpp \
    src/cryptography.cpp \
    src/decryptdialog.cpp \
    src/encryptdialog.cpp \
    src/exportdialog.cpp \
    src/identity.cpp \
    src/identitydialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/network.cpp \
    src/server.cpp

HEADERS += \
    include/aboutwindow.hpp \
    include/addcontactdialog.hpp \
    include/authoriseipdialog.hpp \
    include/client.hpp \
    include/cryptography.hpp \
    include/decryptdialog.hpp \
    include/encryptdialog.hpp \
    include/exportdialog.hpp \
    include/identity.hpp \
    include/identitydialog.hpp \
    include/mainwindow.hpp \
    include/network.hpp \
    include/server.hpp

FORMS += \
    include/aboutwindow.ui \
    include/addcontactdialog.ui \
    include/authoriseipdialog.ui \
    include/decryptdialog.ui \
    include/encryptdialog.ui \
    include/exportdialog.ui \
    include/identitydialog.ui \
    include/mainwindow.ui

TRANSLATIONS += \
    p2kontakt_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations
INCLUDEPATH = ./include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += gpgme
