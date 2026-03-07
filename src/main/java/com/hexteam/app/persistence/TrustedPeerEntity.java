package com.hexteam.app.persistence;

import com.hexteam.app.security.TrustState;
import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.EnumType;
import jakarta.persistence.Enumerated;
import jakarta.persistence.Id;
import jakarta.persistence.Table;

import java.time.Instant;

@Entity
@Table(name = "trusted_peers")
public class TrustedPeerEntity {

    @Id
    @Column(nullable = false, updatable = false)
    private String nodeId;

    @Column(nullable = false)
    private String displayName;

    @Column(nullable = false, length = 2048)
    private String publicKeyFingerprint;

    @Enumerated(EnumType.STRING)
    @Column(nullable = false)
    private TrustState trustState = TrustState.UNVERIFIED;

    @Column(nullable = false)
    private Instant updatedAt = Instant.now();

    public String getNodeId() {
        return nodeId;
    }

    public void setNodeId(String nodeId) {
        this.nodeId = nodeId;
    }

    public String getDisplayName() {
        return displayName;
    }

    public void setDisplayName(String displayName) {
        this.displayName = displayName;
    }

    public String getPublicKeyFingerprint() {
        return publicKeyFingerprint;
    }

    public void setPublicKeyFingerprint(String publicKeyFingerprint) {
        this.publicKeyFingerprint = publicKeyFingerprint;
    }

    public TrustState getTrustState() {
        return trustState;
    }

    public void setTrustState(TrustState trustState) {
        this.trustState = trustState;
    }

    public Instant getUpdatedAt() {
        return updatedAt;
    }

    public void setUpdatedAt(Instant updatedAt) {
        this.updatedAt = updatedAt;
    }
}
