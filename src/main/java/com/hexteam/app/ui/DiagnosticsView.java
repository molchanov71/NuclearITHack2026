package com.hexteam.app.ui;

import com.hexteam.app.metrics.DiagnosticEvent;
import com.hexteam.app.metrics.DiagnosticsService;
import com.vaadin.flow.component.grid.Grid;
import com.vaadin.flow.component.orderedlayout.VerticalLayout;
import com.vaadin.flow.component.UI;
import com.vaadin.flow.router.PageTitle;
import com.vaadin.flow.router.Route;

@Route(value = "diagnostics", layout = MainLayout.class)
@PageTitle("Diagnostics")
public class DiagnosticsView extends VerticalLayout {

    public DiagnosticsView(DiagnosticsService diagnosticsService) {
        setSizeFull();

        Grid<DiagnosticEvent> grid = new Grid<>(DiagnosticEvent.class, false);
        grid.addColumn(DiagnosticEvent::timestamp).setHeader("Время");
        grid.addColumn(DiagnosticEvent::category).setHeader("Категория");
        grid.addColumn(DiagnosticEvent::message).setHeader("Сообщение").setAutoWidth(true);
        grid.setItems(diagnosticsService.recentEvents());
        grid.setSizeFull();
        UI.getCurrent().setPollInterval(2000);
        UI.getCurrent().addPollListener(event -> grid.setItems(diagnosticsService.recentEvents()));
        add(grid);
    }
}
