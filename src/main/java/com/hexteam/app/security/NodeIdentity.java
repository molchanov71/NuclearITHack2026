package com.hexteam.app.security;

public record NodeIdentity(
        String nodeId,
        String displayName,
        String fingerprint,
        String publicKeyBase64
) {
}
