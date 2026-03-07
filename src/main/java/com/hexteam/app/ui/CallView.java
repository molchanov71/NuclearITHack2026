package com.hexteam.app.ui;
 
import com.hexteam.app.metrics.DiagnosticsService;
import com.vaadin.flow.component.button.Button;
import com.vaadin.flow.component.html.Paragraph;
import com.vaadin.flow.component.orderedlayout.HorizontalLayout;
import com.vaadin.flow.component.orderedlayout.VerticalLayout;
import com.vaadin.flow.router.PageTitle;
import com.vaadin.flow.router.Route;

@Route(value = "call", layout = MainLayout.class)
@PageTitle("Call")
public class CallView extends VerticalLayout {

    public CallView(DiagnosticsService diagnosticsService) {
        setSpacing(true);
        add(
                new Paragraph("Экран звонка подготовлен как каркас для WebRTC-интеграции."),
                new Paragraph("Доступные действия пока ограничены диагностическими событиями.")
        );

        Button start = new Button("Start Call", click -> diagnosticsService.record("call", "Запрошен старт звонка"));
        Button end = new Button("End Call", click -> diagnosticsService.record("call", "Запрошено завершение звонка"));
        add(new HorizontalLayout(start, end));
    }
}
