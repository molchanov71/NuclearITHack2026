package com.hexteam.app.config;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.client.RestClient;

import java.time.Clock;

@Configuration
public class AppConfiguration {

    @Bean
    public Clock clock() {
        return Clock.systemUTC();
    }

    @Bean
    public RestClient restClient(@Value("${server.port}") int serverPort) {
        return RestClient.builder()
                .defaultHeader("X-Hex-Node-Port", String.valueOf(serverPort))
                .build();
    }
}
