#include "Chatbot.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QSettings>
#include <QRegularExpression>

Chatbot::Chatbot(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &Chatbot::onNetworkReplyFinished);
}

void Chatbot::ask(const QString &prompt, const QString &apiKey)
{
    QString lower = prompt.toLower();
    QString key = apiKey;

    if (key.isEmpty())
    {
        QSettings settings;
        if (settings.value("Chatbot/enableRemote", false).toBool())
        {
            key = settings.value("Chatbot/apiKey").toString();
        }
    }
    if (lower.contains("hello") || lower.contains("hi"))
    {
        emit responseReady("Hello! How can I help with route planning or the app?");
        return;
    }
    if (lower.contains("fare") || lower.contains("distance"))
    {
        emit responseReady("You can get fare and distance by selecting stations and clicking 'Find Route'. I can also highlight paths on the map.");
        return;
    }

    QRegularExpression re("(?:route\\s+)?from\\s+([\\w\\s-]+?)\\s+(?:to|2)\\s+([\\w\\s-]+)", QRegularExpression::CaseInsensitiveOption);
    auto match = re.match(prompt);
    if (match.hasMatch())
    {
        QString from = match.captured(1).trimmed();
        QString to = match.captured(2).trimmed();
        if (!from.isEmpty() && !to.isEmpty())
        {
            emit routeRequested(from, to);
            emit responseReady(QString("Searching best route from '%1' to '%2'...").arg(from, to));
            return;
        }
    }

    if (!key.isEmpty())
    {
        QUrl url("https://api.openai.com/v1/chat/completions");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("Authorization", ("Bearer " + key).toUtf8());

        QJsonObject message;
        message["role"] = "user";
        message["content"] = prompt;

        QJsonArray messages;
        messages.append(message);

        QJsonObject body;
        body["model"] = "gpt-4o-mini"; // placeholder
        body["messages"] = messages;

        networkManager->post(request, QJsonDocument(body).toJson());
        return;
    }

    QTimer::singleShot(200, [this, prompt]() {
        emit responseReady(localAnswer(prompt));
    });
}

void Chatbot::onNetworkReplyFinished(QNetworkReply *reply)
{
    if (!reply) return;
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QString text = "Sorry, couldn't parse response.";
    if (doc.isObject())
    {
        QJsonObject obj = doc.object();
        if (obj.contains("choices") && obj["choices"].isArray())
        {
            auto choices = obj["choices"].toArray();
            if (!choices.isEmpty() && choices[0].toObject().contains("message"))
            {
                text = choices[0].toObject()["message"].toObject()["content"].toString();
            }
        }
    }
    emit responseReady(text);
    reply->deleteLater();
}

QString Chatbot::localAnswer(const QString &prompt)
{
    if (prompt.toLower().contains("how to implement chatbot"))
        return "Add a Chat UI panel and a Chatbot class; you can call an external LLM via QNetworkAccessManager or implement local rules.";
    return QString("You said: \"%1\"").arg(prompt);
}
