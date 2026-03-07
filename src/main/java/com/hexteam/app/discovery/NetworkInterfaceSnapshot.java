package com.hexteam.app.discovery;

import java.util.List;

public record NetworkInterfaceSnapshot(
        String name,
        String displayName,
        List<String> addresses,
        List<String> broadcastAddresses
) {
}
