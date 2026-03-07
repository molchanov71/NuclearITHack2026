package com.hexteam.app.ui;

import com.vaadin.flow.component.applayout.AppLayout;
import com.vaadin.flow.component.html.H1;
import com.vaadin.flow.component.html.Nav;
import com.vaadin.flow.component.orderedlayout.Scroller;
import com.vaadin.flow.component.tabs.Tab;
import com.vaadin.flow.component.tabs.Tabs;
import com.vaadin.flow.router.HighlightConditions;
import com.vaadin.flow.router.RouterLink;
import com.vaadin.flow.server.auth.AnonymousAllowed;
import com.vaadin.flow.component.dependency.JsModule;

@JsModule("./p2p-bridge.js")
@AnonymousAllowed
public class MainLayout extends AppLayout {

    public MainLayout() {
        H1 title = new H1("Hex.Team LAN Node");
        title.getStyle().set("font-size", "var(--lumo-font-size-l)");
        title.getStyle().set("margin", "0");

        addToNavbar(title);
        addToDrawer(new Scroller(createNavigation()));
    }

    private Nav createNavigation() {
        Nav nav = new Nav();
        Tabs tabs = new Tabs();
        tabs.setOrientation(Tabs.Orientation.VERTICAL);
        tabs.add(
                createTab("Dashboard", DashboardView.class),
                createTab("Peers", PeersView.class),
                createTab("Chat", ChatView.class),
                createTab("Call", CallView.class),
                createTab("Transfers", TransfersView.class),
                createTab("Diagnostics", DiagnosticsView.class)
        );
        nav.add(tabs);
        return nav;
    }

    private Tab createTab(String label, Class<? extends com.vaadin.flow.component.Component> navigationTarget) {
        RouterLink link = new RouterLink(label, navigationTarget);
        link.setHighlightCondition(HighlightConditions.sameLocation());
        return new Tab(link);
    }
}
