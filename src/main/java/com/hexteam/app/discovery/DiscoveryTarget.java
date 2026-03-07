package com.hexteam.app.discovery;

import java.net.InetAddress;

public record DiscoveryTarget(
        InetAddress localAddress,
        InetAddress broadcastAddress
) {
}
