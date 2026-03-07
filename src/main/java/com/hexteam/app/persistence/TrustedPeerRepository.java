package com.hexteam.app.persistence;

import org.springframework.data.jpa.repository.JpaRepository;

public interface TrustedPeerRepository extends JpaRepository<TrustedPeerEntity, String> {
}
