package com.hexteam.app.signaling;

import com.hexteam.app.session.SessionCoordinator;
import com.hexteam.app.session.SessionState;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.web.servlet.MockMvc;

import java.time.Instant;
import java.util.Map;

import static org.assertj.core.api.Assertions.assertThat;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@SpringBootTest
@AutoConfigureMockMvc
@ActiveProfiles("test")
class SignalControllerIntegrationTest {

    @Autowired
    private MockMvc mockMvc;

    @Autowired
    private SessionCoordinator sessionCoordinator;

    @Test
    void shouldAcceptIncomingInviteAndCreateSession() throws Exception {
        String payload = """
                {
                  "type": "SESSION_INVITE",
                  "messageId": "msg-1",
                  "sessionId": "session-1",
                  "senderNodeId": "peer-a",
                  "targetNodeId": "test-node",
                  "createdAt": "2026-03-07T10:00:00Z",
                  "signature": "demo",
                  "payload": {
                    "displayName": "Remote peer"
                  }
                }
                """;

        mockMvc.perform(post("/api/signal")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(payload))
                .andExpect(status().isAccepted());

        assertThat(sessionCoordinator.allSessions())
                .singleElement()
                .satisfies(session -> {
                    assertThat(session.sessionId()).isEqualTo("session-1");
                    assertThat(session.state()).isEqualTo(SessionState.INVITED);
                });
    }
}
