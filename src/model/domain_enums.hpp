#pragma once

/**
 * @brief Состояние доступности узла в реестре peers.
 */
enum class PeerStatus
{
    Online,
    Stale,
    Offline
};

/**
 * @brief Состояние сетевой сессии с peer.
 */
enum class SessionStatus
{
    Connected,
    Degraded,
    Reconnecting,
    Closed
};

/**
 * @brief Состояние передачи файла.
 */
enum class TransferStatus
{
    Pending,
    Accepted,
    Transferring,
    Interrupted,
    Completed,
    Failed
};

/**
 * @brief Состояние звонка.
 */
enum class CallStatus
{
    Invited,
    Active,
    Degraded,
    Ended
};

/**
 * @brief Направление сообщения относительно текущего узла.
 */
enum class MessageDirection
{
    Incoming,
    Outgoing
};

/**
 * @brief Состояние доставки сообщения.
 */
enum class MessageDeliveryStatus
{
    Pending,
    Sent,
    Delivered,
    Failed
};

/**
 * @brief Степень доверия к удалённому узлу.
 */
enum class TrustLevel
{
    Unknown,
    Trusted,
    Blocked
};
