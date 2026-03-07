package com.hexteam.app.files;

import java.util.Set;

public record FileManifest(
        String fileId,
        int totalChunks,
        Set<Integer> receivedChunks,
        String tempPath,
        String expectedHash
) {
}
