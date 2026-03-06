#include "network_services.hpp"

bool DiscoveryService::start(quint16 port)
{
    if (!socket_.bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        lastError_ = socket_.errorString();
        return false;
    }

    lastError_.clear();
    return true;
}

void DiscoveryService::stop()
{
    socket_.close();
}

QString DiscoveryService::lastError() const
{
    return lastError_;
}

bool ControlServer::start(quint16 port)
{
    if (!server_.listen(QHostAddress::Any, port)) {
        lastError_ = server_.errorString();
        return false;
    }

    lastError_.clear();
    return true;
}

void ControlServer::stop()
{
    server_.close();
}

QString ControlServer::lastError() const
{
    return lastError_;
}

bool FileChannelServer::start(quint16 port)
{
    if (!server_.listen(QHostAddress::Any, port)) {
        lastError_ = server_.errorString();
        return false;
    }

    lastError_.clear();
    return true;
}

void FileChannelServer::stop()
{
    server_.close();
}

QString FileChannelServer::lastError() const
{
    return lastError_;
}

bool VoiceUdpTransport::start(quint16 port)
{
    if (!socket_.bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        lastError_ = socket_.errorString();
        return false;
    }

    lastError_.clear();
    return true;
}

void VoiceUdpTransport::stop()
{
    socket_.close();
}

QString VoiceUdpTransport::lastError() const
{
    return lastError_;
}
