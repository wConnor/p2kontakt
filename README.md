# p2kontakt

## Introduction
p2kontakt is an instant-messaging program that makes use of both end-to-end encryption and peer-to-peer networking in order to deliver a secure solution to those wishing to steer away from the typical client-server architecture that most applications use today.

End-to-end encryption is primarily handled by GNU Privacy Guard through the use of certificates, private keys, and public keys. The peer-to-peer architecture uses Qt's network libraries.

## Building
### Linux
p2kontakt uses Qt for the user-interface and networking, as well as GpgME to handle encryption and decryption. You will need GPG installed as well in addition to the previous two development libraries mentioned. GpgME should be available in most distro's repositories as 'libgpgme-dev' (or your package manager's equivalent) - installation of Qt for development may vary greatly.

Once both dependencies have been satisfied, building is as simple as:
```
$ mkdir build && cd build
$ qmake ..
$ make -j$(nproc)
```

Assuming that the build was successful, the result is a binary called 'p2kontakt' in the current directory (./p2kontakt/build). Running is as simple as:

```
$ ./p2kontakt
```

### Windows

## Screenshots
![Identity Creation](/images/identity-creation.png)

Identity creation screen that the user is prompted with upon first launch. Multiple different identities can be created.

![Chat Example](/images/chat-example.png)

Example of chat taking place between two users. Messages are fully encrypted using GPG whilst being transferred.
