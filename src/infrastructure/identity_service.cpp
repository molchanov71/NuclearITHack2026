#include "identity_service.hpp"

#include <QCryptographicHash>
#include <QDir>
#include <QFile>

#include <sodium.h>

namespace
{
QString publicKeyPath(const QString &securityDir)
{
    return securityDir + QStringLiteral("/identity_ed25519.pub");
}

QString secretKeyPath(const QString &securityDir)
{
    return securityDir + QStringLiteral("/identity_ed25519.sec");
}
}

IdentityMaterial IdentityService::initialize(const QString &securityDir)
{
    const int sodiumResult = sodium_init();
    Q_UNUSED(sodiumResult);
    QDir().mkpath(securityDir);

    QFile publicKeyFile(publicKeyPath(securityDir));
    QFile secretKeyFile(secretKeyPath(securityDir));

    QByteArray publicKey;
    QByteArray secretKey;

    if (publicKeyFile.exists() && secretKeyFile.exists() &&
        publicKeyFile.open(QIODevice::ReadOnly) &&
        secretKeyFile.open(QIODevice::ReadOnly)) {
        publicKey = publicKeyFile.readAll();
        secretKey = secretKeyFile.readAll();
    } else {
        publicKey.resize(crypto_sign_PUBLICKEYBYTES);
        secretKey.resize(crypto_sign_SECRETKEYBYTES);
        crypto_sign_keypair(reinterpret_cast<unsigned char *>(publicKey.data()),
                            reinterpret_cast<unsigned char *>(secretKey.data()));

        if (publicKeyFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            publicKeyFile.write(publicKey);
        }
        if (secretKeyFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            secretKeyFile.write(secretKey);
        }
    }

    material_ = IdentityMaterial{
            .publicKey = publicKey,
            .secretKey = secretKey,
            .nodeId = QCryptographicHash::hash(publicKey, QCryptographicHash::Sha256).toHex(),
    };

    return material_;
}

QByteArray IdentityService::sign(const QByteArray &payload) const
{
    QByteArray signature(crypto_sign_BYTES, Qt::Uninitialized);
    unsigned long long signatureSize = 0;
    crypto_sign_detached(reinterpret_cast<unsigned char *>(signature.data()),
                         &signatureSize,
                         reinterpret_cast<const unsigned char *>(payload.constData()),
                         static_cast<unsigned long long>(payload.size()),
                         reinterpret_cast<const unsigned char *>(material_.secretKey.constData()));
    signature.resize(static_cast<qsizetype>(signatureSize));
    return signature;
}

bool IdentityService::verify(const QByteArray &payload, const QByteArray &signature, const QByteArray &publicKey) const
{
    return crypto_sign_verify_detached(reinterpret_cast<const unsigned char *>(signature.constData()),
                                       reinterpret_cast<const unsigned char *>(payload.constData()),
                                       static_cast<unsigned long long>(payload.size()),
                                       reinterpret_cast<const unsigned char *>(publicKey.constData())) == 0;
}

QByteArray IdentityService::deriveSessionKey(const QByteArray &peerPublicKey) const
{
    QByteArray buffer = material_.publicKey + peerPublicKey;
    return QCryptographicHash::hash(buffer, QCryptographicHash::Sha256);
}

QByteArray IdentityService::generateNonce() const
{
    QByteArray nonce(crypto_aead_xchacha20poly1305_ietf_NPUBBYTES, Qt::Uninitialized);
    randombytes_buf(nonce.data(), static_cast<size_t>(nonce.size()));
    return nonce;
}

QByteArray IdentityService::encrypt(const QByteArray &payload, const QByteArray &key, const QByteArray &nonce) const
{
    QByteArray ciphertext(payload.size() + crypto_aead_xchacha20poly1305_ietf_ABYTES, Qt::Uninitialized);
    unsigned long long ciphertextSize = 0;
    crypto_aead_xchacha20poly1305_ietf_encrypt(
            reinterpret_cast<unsigned char *>(ciphertext.data()),
            &ciphertextSize,
            reinterpret_cast<const unsigned char *>(payload.constData()),
            static_cast<unsigned long long>(payload.size()),
            nullptr,
            0,
            nullptr,
            reinterpret_cast<const unsigned char *>(nonce.constData()),
            reinterpret_cast<const unsigned char *>(key.constData()));
    ciphertext.resize(static_cast<qsizetype>(ciphertextSize));
    return ciphertext;
}

QByteArray IdentityService::decrypt(const QByteArray &payload, const QByteArray &key, const QByteArray &nonce) const
{
    QByteArray plaintext(payload.size(), Qt::Uninitialized);
    unsigned long long plaintextSize = 0;
    if (crypto_aead_xchacha20poly1305_ietf_decrypt(
                reinterpret_cast<unsigned char *>(plaintext.data()),
                &plaintextSize,
                nullptr,
                reinterpret_cast<const unsigned char *>(payload.constData()),
                static_cast<unsigned long long>(payload.size()),
                nullptr,
                0,
                reinterpret_cast<const unsigned char *>(nonce.constData()),
                reinterpret_cast<const unsigned char *>(key.constData())) != 0) {
        return {};
    }

    plaintext.resize(static_cast<qsizetype>(plaintextSize));
    return plaintext;
}

const IdentityMaterial &IdentityService::material() const
{
    return material_;
}
