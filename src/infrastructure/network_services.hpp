#pragma once

#include <QString>

#include <QTcpServer>
#include <QUdpSocket>

/**
 * @brief Заглушка UDP discovery сервиса.
 */
class DiscoveryService
{
public:
    bool start(quint16 port);
    void stop();
    [[nodiscard]] QString lastError() const;

private:
    QUdpSocket socket_{};
    QString lastError_{};
};

/**
 * @brief Заглушка TCP control-plane сервера.
 */
class ControlServer
{
public:
    bool start(quint16 port);
    void stop();
    [[nodiscard]] QString lastError() const;

private:
    QTcpServer server_{};
    QString lastError_{};
};

/**
 * @brief Заглушка TCP file channel сервера.
 */
class FileChannelServer
{
public:
    bool start(quint16 port);
    void stop();
    [[nodiscard]] QString lastError() const;

private:
    QTcpServer server_{};
    QString lastError_{};
};

/**
 * @brief Заглушка UDP транспорта для голосового канала.
 */
class VoiceUdpTransport
{
public:
    bool start(quint16 port);
    void stop();
    [[nodiscard]] QString lastError() const;

private:
    QUdpSocket socket_{};
    QString lastError_{};
};
