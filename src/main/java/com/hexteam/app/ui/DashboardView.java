package com.hexteam.app.ui;

import com.hexteam.app.discovery.PeerRegistry;
import com.hexteam.app.security.NodeIdentityService;
import com.hexteam.app.session.SessionCoordinator;
import com.vaadin.flow.component.html.H2;
import com.vaadin.flow.component.html.Paragraph;
import com.vaadin.flow.component.orderedlayout.VerticalLayout;
import com.vaadin.flow.router.PageTitle;
import com.vaadin.flow.router.Route;
import org.springframework.beans.factory.annotation.Value;

@Route(value = "", layout = MainLayout.class)
@PageTitle("Dashboard")
public class DashboardView extends VerticalLayout {

    public DashboardView(NodeIdentityService identityService,
                         PeerRegistry peerRegistry,
                         SessionCoordinator sessionCoordinator,
                         @Value("${server.port}") int serverPort) {
        setSpacing(true);
        setPadding(true);

        H2 header = new H2("Состояние узла");
        add(
                header,
                new Paragraph("Display name: " + identityService.currentIdentity().displayName()),
                new Paragraph("Node ID: " + identityService.currentIdentity().nodeId()),
                new Paragraph("Fingerprint: " + identityService.fingerprint()),
                new Paragraph("Порт приложения: " + serverPort),
                new Paragraph("Найдено peer-ов online: " + peerRegistry.countOnlinePeers()),
                new Paragraph("Активных сессий: " + sessionCoordinator.allSessions().size()),
                new Paragraph("Статус запуска: узел инициализирован")
        );
    }
}
