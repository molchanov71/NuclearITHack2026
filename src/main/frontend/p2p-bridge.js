window.HexP2PBridge = (() => {
  const peerConnections = new Map();

  function ensureSession(sessionId) {
    if (!peerConnections.has(sessionId)) {
      peerConnections.set(sessionId, { sessionId, state: "NEW" });
    }
    return peerConnections.get(sessionId);
  }

  return {
    createOffer(sessionId) {
      const session = ensureSession(sessionId);
      session.state = "OFFERING";
      return Promise.resolve({ type: "offer", sdp: "placeholder-offer", sessionId });
    },
    applyOffer(sessionId, offer) {
      const session = ensureSession(sessionId);
      session.offer = offer;
      session.state = "REMOTE_OFFER_APPLIED";
    },
    applyAnswer(sessionId, answer) {
      const session = ensureSession(sessionId);
      session.answer = answer;
      session.state = "CONNECTED";
    },
    addIceCandidate(sessionId, candidate) {
      const session = ensureSession(sessionId);
      session.candidate = candidate;
    },
    sendChatMessage(sessionId, payload) {
      const session = ensureSession(sessionId);
      session.lastMessage = payload;
    },
    startAudioCall(sessionId) {
      const session = ensureSession(sessionId);
      session.call = "STARTED";
    },
    endCall(sessionId) {
      const session = ensureSession(sessionId);
      session.call = "ENDED";
    },
    cleanup(sessionId) {
      peerConnections.delete(sessionId);
    }
  };
})();
