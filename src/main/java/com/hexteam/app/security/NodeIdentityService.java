package com.hexteam.app.security;

import com.hexteam.app.config.HexProperties;
import com.hexteam.app.metrics.DiagnosticsService;
import jakarta.annotation.PostConstruct;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.GeneralSecurityException;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.Base64;
import java.util.HexFormat;

@Service
public class NodeIdentityService {
    /**
     * Отвечает за стабильную идентичность узла:
     * генерацию ключей, загрузку из диска и криптографические операции.
     */

    private static final Logger log = LoggerFactory.getLogger(NodeIdentityService.class);

    private final HexProperties properties;
    private final DiagnosticsService diagnosticsService;
    private final String displayName;

    private NodeIdentity nodeIdentity;
    private PrivateKey privateKey;
    private PublicKey publicKey;

    public NodeIdentityService(HexProperties properties,
                               DiagnosticsService diagnosticsService,
                               @Value("${node.display-name}") String displayName) {
        this.properties = properties;
        this.diagnosticsService = diagnosticsService;
        this.displayName = displayName;
    }

    @PostConstruct
    public void initialize() throws IOException, GeneralSecurityException {
        Path identityDir = Path.of(properties.getStorage().getRoot(), "identity");
        Files.createDirectories(identityDir);
        Path privateKeyPath = identityDir.resolve("node-private.key");
        Path publicKeyPath = identityDir.resolve("node-public.key");

        if (Files.exists(privateKeyPath) && Files.exists(publicKeyPath)) {
            loadExistingKeys(privateKeyPath, publicKeyPath);
            log.info("Загружена существующая идентичность узла {}", nodeIdentity.nodeId());
            diagnosticsService.record("identity", "Загружена существующая идентичность узла");
            return;
        }

        KeyPairGenerator generator = KeyPairGenerator.getInstance("Ed25519");
        KeyPair keyPair = generator.generateKeyPair();
        this.privateKey = keyPair.getPrivate();
        this.publicKey = keyPair.getPublic();
        Files.writeString(privateKeyPath, Base64.getEncoder().encodeToString(privateKey.getEncoded()), StandardCharsets.UTF_8);
        Files.writeString(publicKeyPath, Base64.getEncoder().encodeToString(publicKey.getEncoded()), StandardCharsets.UTF_8);
        this.nodeIdentity = buildIdentity();
        log.info("Сгенерирована новая идентичность узла {}", nodeIdentity.nodeId());
        diagnosticsService.record("identity", "Сгенерирована новая идентичность узла");
    }

    public NodeIdentity currentIdentity() {
        return nodeIdentity;
    }

    /**
     * Подписывает полезную нагрузку приватным ключом узла.
     */
    public String sign(String payload) {
        try {
            Signature signature = Signature.getInstance("Ed25519");
            signature.initSign(privateKey);
            signature.update(payload.getBytes(StandardCharsets.UTF_8));
            return Base64.getEncoder().encodeToString(signature.sign());
        } catch (GeneralSecurityException exception) {
            throw new IllegalStateException("Не удалось подписать payload", exception);
        }
    }

    /**
     * Проверяет подпись по переданному публичному ключу peer-а.
     */
    public boolean verify(String payload, String signatureBase64, String publicKeyBase64) {
        try {
            Signature signature = Signature.getInstance("Ed25519");
            signature.initVerify(rebuildPublicKey(publicKeyBase64));
            signature.update(payload.getBytes(StandardCharsets.UTF_8));
            return signature.verify(Base64.getDecoder().decode(signatureBase64));
        } catch (GeneralSecurityException exception) {
            throw new IllegalStateException("Не удалось проверить подпись", exception);
        }
    }

    public String fingerprint() {
        return nodeIdentity.fingerprint();
    }

    private void loadExistingKeys(Path privateKeyPath, Path publicKeyPath) throws IOException, GeneralSecurityException {
        byte[] privateKeyBytes = Base64.getDecoder().decode(Files.readString(privateKeyPath, StandardCharsets.UTF_8).trim());
        byte[] publicKeyBytes = Base64.getDecoder().decode(Files.readString(publicKeyPath, StandardCharsets.UTF_8).trim());
        KeyFactory keyFactory = KeyFactory.getInstance("Ed25519");
        this.privateKey = keyFactory.generatePrivate(new PKCS8EncodedKeySpec(privateKeyBytes));
        this.publicKey = keyFactory.generatePublic(new X509EncodedKeySpec(publicKeyBytes));
        this.nodeIdentity = buildIdentity();
    }

    private PublicKey rebuildPublicKey(String publicKeyBase64) throws GeneralSecurityException {
        byte[] publicKeyBytes = Base64.getDecoder().decode(publicKeyBase64);
        return KeyFactory.getInstance("Ed25519").generatePublic(new X509EncodedKeySpec(publicKeyBytes));
    }

    private NodeIdentity buildIdentity() {
        String publicKeyBase64 = Base64.getEncoder().encodeToString(publicKey.getEncoded());
        String fingerprint = HexFormat.of().formatHex(Hashing.sha256(publicKey.getEncoded()));
        String nodeId = fingerprint.substring(0, 16);
        return new NodeIdentity(nodeId, displayName, fingerprint, publicKeyBase64);
    }
}
