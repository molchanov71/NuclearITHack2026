package com.hexteam.app.session;

public enum SessionState {
    NEW,
    INVITED,
    SIGNALING,
    CONNECTING,
    CONNECTED,
    DEGRADED,
    RECONNECTING,
    CLOSED
}
