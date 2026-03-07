package com.hexteam.app.signaling;

public enum SignalType {
    SESSION_INVITE,
    SESSION_ACCEPT,
    RTC_OFFER,
    RTC_ANSWER,
    ICE_CANDIDATE,
    CALL_START,
    CALL_END,
    ACK,
    ERROR
}
