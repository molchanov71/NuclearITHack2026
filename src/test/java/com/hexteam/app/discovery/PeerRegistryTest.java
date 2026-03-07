package com.hexteam.app.discovery;

import com.hexteam.app.config.HexProperties;
import com.hexteam.app.metrics.DiagnosticsService;
import org.junit.jupiter.api.Test;

import java.time.Clock;
import java.time.Instant;
import java.time.ZoneOffset;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertEquals;

class PeerRegistryTest {

    @Test
    void shouldSwitchPeerToOfflineWhenTtlExpired() {
        HexProperties properties = new HexProperties();
        properties.getDiscovery().setTtlMs(1_000);

        MutableClock clock = new MutableClock(Instant.parse("2026-03-07T10:00:00Z"));
        PeerRegistry registry = new PeerRegistry(properties, clock, new DiagnosticsService());
        registry.upsert(new DiscoveryAnnounce(
                "node-1",
                "Peer 1",
                "1.0",
                "http://localhost:8081/api/signal",
                "fingerprint",
                Set.of("chat"),
                clock.instant(),
                1
        ));

        clock.advanceMillis(1500);
        registry.updatePeerStates();

        assertEquals(PeerState.OFFLINE, registry.findByNodeId("node-1").state());
    }

    private static final class MutableClock extends Clock {

        private Instant current;

        private MutableClock(Instant current) {
            this.current = current;
        }

        @Override
        public ZoneOffset getZone() {
            return ZoneOffset.UTC;
        }

        @Override
        public Clock withZone(java.time.ZoneId zone) {
            return this;
        }

        @Override
        public Instant instant() {
            return current;
        }

        private void advanceMillis(long millis) {
            current = current.plusMillis(millis);
        }
    }
}
