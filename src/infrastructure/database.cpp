#include "database.hpp"

#include <QUuid>
#include <QSqlError>
#include <QSqlQuery>

Database::Database()
    : connectionName_(QStringLiteral("db-%1").arg(QUuid::createUuid().toString(QUuid::WithoutBraces)))
{
}

Database::~Database()
{
    shutdown();
}

bool Database::initialize(const QString &path)
{
    if (database_.isOpen()) {
        return true;
    }

    database_ = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connectionName_);
    database_.setDatabaseName(path);
    if (!database_.open()) {
        return false;
    }

    return initializeSchema();
}

void Database::shutdown()
{
    if (database_.isValid()) {
        const QString connectionName = connectionName_;
        if (database_.isOpen()) {
            database_.close();
        }
        database_ = QSqlDatabase();
        QSqlDatabase::removeDatabase(connectionName);
    }
}

const QSqlDatabase &Database::connection() const
{
    return database_;
}

bool Database::initializeSchema()
{
    static const char *statements[] = {
            "CREATE TABLE IF NOT EXISTS node_identity (node_id TEXT PRIMARY KEY, display_name TEXT NOT NULL, device_name TEXT NOT NULL, public_key BLOB NOT NULL, created_at TEXT NOT NULL)",
            "CREATE TABLE IF NOT EXISTS trusted_peers (peer_id TEXT PRIMARY KEY, trust_level INTEGER NOT NULL, updated_at TEXT NOT NULL)",
            "CREATE TABLE IF NOT EXISTS peers (peer_id TEXT PRIMARY KEY, display_name TEXT NOT NULL, addresses TEXT NOT NULL, capabilities TEXT NOT NULL DEFAULT '[]', discovery_port INTEGER NOT NULL DEFAULT 0, control_port INTEGER NOT NULL DEFAULT 0, file_port INTEGER NOT NULL DEFAULT 0, voice_port INTEGER NOT NULL DEFAULT 0, last_seen_at TEXT NOT NULL, status INTEGER NOT NULL, trust_level INTEGER NOT NULL)",
            "CREATE TABLE IF NOT EXISTS sessions (session_id TEXT PRIMARY KEY, peer_id TEXT NOT NULL, status INTEGER NOT NULL, connected_at TEXT NOT NULL, last_activity_at TEXT NOT NULL, encrypted INTEGER NOT NULL)",
            "CREATE TABLE IF NOT EXISTS messages (message_id TEXT PRIMARY KEY, peer_id TEXT NOT NULL, session_id TEXT NOT NULL, author_id TEXT NOT NULL, text TEXT NOT NULL, created_at TEXT NOT NULL, direction INTEGER NOT NULL, delivery_status INTEGER NOT NULL)",
            "CREATE TABLE IF NOT EXISTS file_transfers (transfer_id TEXT PRIMARY KEY, peer_id TEXT NOT NULL, manifest_id TEXT NOT NULL, status INTEGER NOT NULL, bytes_transferred INTEGER NOT NULL, bytes_total INTEGER NOT NULL, updated_at TEXT NOT NULL)",
            "CREATE TABLE IF NOT EXISTS file_manifests (manifest_id TEXT PRIMARY KEY, file_name TEXT NOT NULL, relative_path TEXT NOT NULL, sha256 BLOB NOT NULL, total_size INTEGER NOT NULL, chunk_size INTEGER NOT NULL, chunk_count INTEGER NOT NULL)",
            "CREATE TABLE IF NOT EXISTS metrics_snapshots (snapshot_id INTEGER PRIMARY KEY AUTOINCREMENT, peer_id TEXT NOT NULL, session_id TEXT NOT NULL, captured_at TEXT NOT NULL, latency_ms INTEGER NOT NULL, jitter_ms INTEGER NOT NULL, packet_loss REAL NOT NULL, throughput_kbps INTEGER NOT NULL)",
    };

    for (const char *statement : statements) {
        QSqlQuery query(database_);
        if (!query.exec(QString::fromUtf8(statement))) {
            return false;
        }
    }

    return true;
}
