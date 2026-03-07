package com.hexteam.app.discovery;

import org.springframework.boot.autoconfigure.condition.ConditionalOnBean;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@ConditionalOnBean(DiscoveryService.class)
@RequestMapping("/api/discovery/probe")
public class DiscoveryProbeController {

    private final DiscoveryService discoveryService;

    public DiscoveryProbeController(DiscoveryService discoveryService) {
        this.discoveryService = discoveryService;
    }

    @GetMapping
    public DiscoveryAnnounce probe() {
        return discoveryService.currentAnnounceSnapshot();
    }
}
