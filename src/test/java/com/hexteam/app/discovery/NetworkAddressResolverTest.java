package com.hexteam.app.discovery;

import org.junit.jupiter.api.Test;

import java.net.InetAddress;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertNotNull;

class NetworkAddressResolverTest {

    @Test
    void shouldResolveAtLeastLoopbackAsPrimaryAddress() {
        NetworkAddressResolver resolver = new NetworkAddressResolver();

        InetAddress address = resolver.resolvePrimaryAddress();
        List<InetAddress> broadcasts = resolver.broadcastAddresses();

        assertNotNull(address);
        assertNotNull(broadcasts);
    }
}
