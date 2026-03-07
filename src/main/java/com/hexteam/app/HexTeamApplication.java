package com.hexteam.app;

import com.hexteam.app.config.HexProperties;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.context.properties.EnableConfigurationProperties;

@SpringBootApplication
@EnableConfigurationProperties(HexProperties.class)
public class HexTeamApplication {

    public static void main(String[] args) {
        SpringApplication.run(HexTeamApplication.class, args);
    }
}
