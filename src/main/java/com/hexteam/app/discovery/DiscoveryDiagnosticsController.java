package com.hexteam.app.discovery;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.Collection;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/api/diagnostics/discovery")
public class DiscoveryDiagnosticsController {

    private final NetworkAddressResolver networkAddressResolver;
    private final PeerRegistry peerRegistry;

    public DiscoveryDiagnosticsController(NetworkAddressResolver networkAddressResolver, PeerRegistry peerRegistry) {
        this.networkAddressResolver = networkAddressResolver;
        this.peerRegistry = peerRegistry;
    }

    @GetMapping
    public Map<String, Object> state() {
        return Map.of(
                "primaryAddress", networkAddressResolver.resolvePrimaryAddress().getHostAddress(),
                "broadcastAddresses", networkAddressResolver.broadcastAddresses().stream().map(address -> address.getHostAddress()).toList(),
                "interfaces", networkAddressResolver.interfaceSnapshots(),
                "peers", peerRegistry.allPeers()
        );
    }
}
