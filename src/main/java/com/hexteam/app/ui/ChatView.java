package com.hexteam.app.ui;

import com.hexteam.app.chat.ChatService;
import com.hexteam.app.security.NodeIdentityService;
import com.vaadin.flow.component.button.Button;
import com.vaadin.flow.component.grid.Grid;
import com.vaadin.flow.component.notification.Notification;
import com.vaadin.flow.component.orderedlayout.HorizontalLayout;
import com.vaadin.flow.component.orderedlayout.VerticalLayout;
import com.vaadin.flow.component.textfield.TextField;
import com.vaadin.flow.router.PageTitle;
import com.vaadin.flow.router.Route;

@Route(value = "chat", layout = MainLayout.class)
@PageTitle("Chat")
public class ChatView extends VerticalLayout {

    public ChatView(ChatService chatService, NodeIdentityService identityService) {
        setSizeFull();

        Grid<com.hexteam.app.chat.ChatMessage> grid = new Grid<>(com.hexteam.app.chat.ChatMessage.class, false);
        grid.addColumn(com.hexteam.app.chat.ChatMessage::createdAt).setHeader("Время");
        grid.addColumn(com.hexteam.app.chat.ChatMessage::sessionId).setHeader("Session");
        grid.addColumn(com.hexteam.app.chat.ChatMessage::content).setHeader("Сообщение").setAutoWidth(true);
        grid.addColumn(com.hexteam.app.chat.ChatMessage::status).setHeader("Статус");
        grid.setItems(chatService.recentMessages());
        grid.setSizeFull();

        TextField messageField = new TextField();
        messageField.setPlaceholder("Черновик сообщения");
        messageField.setWidthFull();

        Button saveDraft = new Button("Сохранить локально", click -> {
            if (messageField.isEmpty()) {
                Notification.show("Введите текст сообщения");
                return;
            }
            chatService.savePendingMessage("draft-session", identityService.currentIdentity().nodeId(), "unknown", messageField.getValue());
            grid.setItems(chatService.recentMessages());
            messageField.clear();
        });

        add(grid, new HorizontalLayout(messageField, saveDraft));
    }
}
