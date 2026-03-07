package com.hexteam.app.ui;

import com.hexteam.app.discovery.PeerDescriptor;
import com.hexteam.app.discovery.PeerRegistry;
import com.hexteam.app.session.SessionCoordinator;
import com.vaadin.flow.component.button.Button;
import com.vaadin.flow.component.grid.Grid;
import com.vaadin.flow.component.notification.Notification;
import com.vaadin.flow.component.orderedlayout.HorizontalLayout;
import com.vaadin.flow.component.orderedlayout.VerticalLayout;
import com.vaadin.flow.component.UI;
import com.vaadin.flow.router.PageTitle;
import com.vaadin.flow.router.Route;

@Route(value = "peers", layout = MainLayout.class)
@PageTitle("Peers")
public class PeersView extends VerticalLayout {

    public PeersView(PeerRegistry peerRegistry, SessionCoordinator sessionCoordinator) {
        setSizeFull();

        Grid<PeerDescriptor> grid = new Grid<>(PeerDescriptor.class, false);
        grid.addColumn(PeerDescriptor::displayName).setHeader("Имя");
        grid.addColumn(PeerDescriptor::nodeId).setHeader("Node ID");
        grid.addColumn(PeerDescriptor::state).setHeader("Состояние");
        grid.addColumn(peer -> String.join(", ", peer.capabilities())).setHeader("Возможности");
        grid.addColumn(PeerDescriptor::signalBaseUrl).setHeader("Signal URL").setAutoWidth(true);
        grid.setItems(peerRegistry.allPeers());
        grid.setSizeFull();
        UI.getCurrent().setPollInterval(2000);
        UI.getCurrent().addPollListener(event -> grid.setItems(peerRegistry.allPeers()));

        Button refresh = new Button("Обновить", click -> grid.setItems(peerRegistry.allPeers()));
        Button connect = new Button("Connect", click -> {
            PeerDescriptor selected = grid.asSingleSelect().getValue();
            if (selected == null) {
                Notification.show("Сначала выберите peer");
                return;
            }
            sessionCoordinator.startSession(selected.nodeId());
            Notification.show("Инициирована сессия с " + selected.displayName());
        });

        add(new HorizontalLayout(refresh, connect), grid);
    }
}
