#ifndef CHATBOT_H
#define CHATBOT_H

#include <QObject>
#include <QString>

class QNetworkAccessManager;
class QNetworkReply;

class Chatbot : public QObject
{
    Q_OBJECT
public:
    explicit Chatbot(QObject *parent = nullptr);

    // Ask the chatbot (local first). If apiKey non-empty, can call remote LLM (optional).
    Q_INVOKABLE void ask(const QString &prompt, const QString &apiKey = QString());

signals:
    void responseReady(const QString &response);
    void routeRequested(const QString &from, const QString &to);

private slots:
    void onNetworkReplyFinished(QNetworkReply *reply);

private:
    QString localAnswer(const QString &prompt);
    QNetworkAccessManager *networkManager = nullptr;
};

#endif // CHATBOT_H
