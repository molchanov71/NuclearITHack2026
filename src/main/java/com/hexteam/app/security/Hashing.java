package com.hexteam.app.security;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public final class Hashing {

    private Hashing() {
    }

    public static byte[] sha256(byte[] data) {
        try {
            return MessageDigest.getInstance("SHA-256").digest(data);
        } catch (NoSuchAlgorithmException exception) {
            throw new IllegalStateException("Алгоритм SHA-256 недоступен", exception);
        }
    }
}
