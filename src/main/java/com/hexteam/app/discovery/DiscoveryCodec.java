package com.hexteam.app.discovery;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.stereotype.Component;

@Component
public class DiscoveryCodec {

    private final ObjectMapper objectMapper;

    public DiscoveryCodec(ObjectMapper objectMapper) {
        this.objectMapper = objectMapper;
    }

    public byte[] write(DiscoveryAnnounce announce) {
        try {
            return objectMapper.writeValueAsBytes(announce);
        } catch (JsonProcessingException exception) {
            throw new IllegalStateException("Не удалось сериализовать discovery announce", exception);
        }
    }

    public DiscoveryAnnounce read(byte[] payload, int length) {
        try {
            return objectMapper.readValue(new String(payload, 0, length), DiscoveryAnnounce.class);
        } catch (JsonProcessingException exception) {
            throw new IllegalStateException("Не удалось десериализовать discovery announce", exception);
        }
    }
}
