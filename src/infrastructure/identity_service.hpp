#pragma once

#include <QByteArray>

/**
 * @brief Ключевая информация локального узла для подписи и secure session.
 */
struct IdentityMaterial
{
    QByteArray publicKey;
    QByteArray secretKey;
    QByteArray nodeId;
};

/**
 * @brief Базовые криптографические операции узла на основе libsodium.
 */
class IdentityService
{
public:
    /**
     * @brief Инициализирует libsodium и загружает либо создаёт identity keys.
     * @param securityDir Каталог для хранения секретов узла.
     * @return Сгенерированный или загруженный комплект ключей.
     */
    [[nodiscard]] IdentityMaterial initialize(const QString &securityDir);

    /** @brief Подписывает произвольные данные long-term ключом узла. */
    [[nodiscard]] QByteArray sign(const QByteArray &payload) const;
    /** @brief Проверяет подпись long-term публичным ключом. */
    [[nodiscard]] bool verify(const QByteArray &payload, const QByteArray &signature, const QByteArray &publicKey) const;
    /** @brief Вычисляет производный симметричный ключ сессии. */
    [[nodiscard]] QByteArray deriveSessionKey(const QByteArray &peerPublicKey) const;
    /** @brief Генерирует nonce для AEAD-операций. */
    [[nodiscard]] QByteArray generateNonce() const;
    /** @brief Шифрует полезную нагрузку с использованием XChaCha20-Poly1305. */
    [[nodiscard]] QByteArray encrypt(const QByteArray &payload, const QByteArray &key, const QByteArray &nonce) const;
    /** @brief Расшифровывает полезную нагрузку с использованием XChaCha20-Poly1305. */
    [[nodiscard]] QByteArray decrypt(const QByteArray &payload, const QByteArray &key, const QByteArray &nonce) const;
    /** @brief Возвращает активные ключи узла. */
    [[nodiscard]] const IdentityMaterial &material() const;

private:
    IdentityMaterial material_{};
};
