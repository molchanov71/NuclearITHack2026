package com.hexteam.app.security;

import com.hexteam.app.config.HexProperties;
import com.hexteam.app.metrics.DiagnosticsService;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class NodeIdentityServiceTest {

    @TempDir
    Path tempDir;

    @Test
    void shouldGenerateStableIdentityAndVerifySignature() throws Exception {
        HexProperties properties = new HexProperties();
        properties.getStorage().setRoot(tempDir.toString());
        properties.getFiles().setTempDir(tempDir.resolve("tmp").toString());
        properties.getFiles().setFinalDir(tempDir.resolve("final").toString());

        DiagnosticsService diagnosticsService = new DiagnosticsService();
        NodeIdentityService first = new NodeIdentityService(properties, diagnosticsService, "Test Node");
        first.initialize();

        String payload = "hello";
        String signature = first.sign(payload);

        NodeIdentityService second = new NodeIdentityService(properties, diagnosticsService, "Test Node");
        second.initialize();

        assertEquals(first.currentIdentity().nodeId(), second.currentIdentity().nodeId());
        assertTrue(second.verify(payload, signature, first.currentIdentity().publicKeyBase64()));
        assertFalse(second.verify("wrong", signature, first.currentIdentity().publicKeyBase64()));
    }
}
