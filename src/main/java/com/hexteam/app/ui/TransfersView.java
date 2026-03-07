package com.hexteam.app.ui;

import com.hexteam.app.files.FileTransfer;
import com.hexteam.app.files.FileTransferService;
import com.vaadin.flow.component.grid.Grid;
import com.vaadin.flow.component.orderedlayout.VerticalLayout;
import com.vaadin.flow.router.PageTitle;
import com.vaadin.flow.router.Route;

@Route(value = "transfers", layout = MainLayout.class)
@PageTitle("Transfers")
public class TransfersView extends VerticalLayout {

    public TransfersView(FileTransferService fileTransferService) {
        Grid<FileTransfer> grid = new Grid<>(FileTransfer.class, false);
        grid.addColumn(FileTransfer::createdAt).setHeader("Создано");
        grid.addColumn(FileTransfer::fileName).setHeader("Файл");
        grid.addColumn(FileTransfer::fileSize).setHeader("Размер");
        grid.addColumn(FileTransfer::status).setHeader("Статус");
        grid.addColumn(FileTransfer::progressPercent).setHeader("Прогресс");
        grid.setItems(fileTransferService.recentTransfers());
        grid.setSizeFull();
        setSizeFull();
        add(grid);
    }
}
