#include "log_service.hpp"

#include <QDateTime>
#include <QDir>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

void UiLogBuffer::append(const QString &line)
{
    lines_.append(line);
    constexpr qsizetype maxLines = 200;
    if (lines_.size() > maxLines) {
        lines_.remove(0, lines_.size() - maxLines);
    }
}

QStringList UiLogBuffer::lines() const
{
    return lines_;
}

std::shared_ptr<spdlog::logger> LogService::initialize(const QString &logDir)
{
    QDir().mkpath(logDir);

    const QString filePath = logDir + QStringLiteral("/runtime.log");
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filePath.toStdString(), true);
    auto logger = std::make_shared<spdlog::logger>(
            "app",
            spdlog::sinks_init_list{consoleSink, fileSink});

    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    uiBuffer_.append(QStringLiteral("%1 | log service initialized")
                             .arg(QDateTime::currentDateTimeUtc().toString(Qt::ISODate)));
    return logger;
}

UiLogBuffer &LogService::uiBuffer()
{
    return uiBuffer_;
}

const UiLogBuffer &LogService::uiBuffer() const
{
    return uiBuffer_;
}
