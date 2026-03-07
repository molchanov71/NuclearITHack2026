# Hex.Team LAN Node

Локальный узел для P2P-взаимодействия в LAN на базе Spring Boot 3, Vaadin 24 и H2.

## Что уже реализовано

- каркас Spring Boot + Vaadin + Actuator + H2;
- конфигурация узла через `application.yml`;
- локальная идентичность узла на Ed25519;
- discovery-модуль с UDP announce и реестром peer-ов;
- signaling endpoint `/api/signal`;
- координатор сессий и базовые переходы состояний;
- persistence-скелет для доверенных peer-ов, сообщений и передач файлов;
- UI-скелет для dashboard, peers, chat, call, transfers и diagnostics;
- модульные и интеграционные тесты.

## Запуск

```bash
mvn spring-boot:run
```

По умолчанию приложение поднимается на `http://localhost:8080`.

Локальные ключи и состояние по умолчанию хранятся в `~/.hex-team-app`, а не в каталоге репозитория. Это важно, чтобы разные ноутбуки не делили одну и ту же идентичность узла при копировании проекта.

## Основные настройки

- `node.display-name` — отображаемое имя узла.
- `server.port` — HTTP-порт узла.
- `hex.discovery.port` — UDP-порт discovery.
- `hex.discovery.interval-ms` — интервал announce.
- `hex.discovery.ttl-ms` — время устаревания peer-а.
- `hex.signal.base-path` — путь signaling API.
- `hex.storage.root` — корень локального хранения.
- `hex.files.temp-dir` — временная директория файлов.
- `hex.files.final-dir` — директория итоговых файлов.

Если раньше проект запускался с `./data` внутри репозитория и эта папка была скопирована на другой ноутбук, удалите старые ключи `data/identity` или задайте новый `HEX_STORAGE_ROOT`, иначе разные машины будут иметь одинаковый `nodeId`.

## Тесты

```bash
mvn test
```

## Ограничения текущей версии

- WebRTC bridge пока реализован как единый JS-слой с заглушками;
- нет полноценного обмена offer/answer, чата по data channel и аудио;
- signaling-подписи пока только формируются, но не валидируются на входе.
