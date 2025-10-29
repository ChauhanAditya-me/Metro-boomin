#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Settings");
    setModal(true);

    apiKeyEdit = new QLineEdit;
    apiKeyEdit->setPlaceholderText("Paste API key here (kept in QSettings)");

    enableRemoteCheck = new QCheckBox("Enable remote LLM calls");
    rememberKeyCheck = new QCheckBox("Remember API key on this machine");
    rememberKeyCheck->setChecked(true);

    saveBtn = new QPushButton("Save");
    cancelBtn = new QPushButton("Cancel");

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel("API Key:"));
    mainLayout->addWidget(apiKeyEdit);
    mainLayout->addWidget(enableRemoteCheck);
    mainLayout->addWidget(rememberKeyCheck);

    auto *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    QSettings settings;
    apiKeyEdit->setText(settings.value("Chatbot/apiKey").toString());
    enableRemoteCheck->setChecked(settings.value("Chatbot/enableRemote", false).toBool());
    rememberKeyCheck->setChecked(settings.value("Chatbot/rememberKey", true).toBool());

    connect(saveBtn, &QPushButton::clicked, this, &SettingsDialog::saveSettings);
    connect(cancelBtn, &QPushButton::clicked, this, &SettingsDialog::reject);
}

void SettingsDialog::saveSettings()
{
    QSettings settings;
    settings.setValue("Chatbot/enableRemote", enableRemoteCheck->isChecked());
    settings.setValue("Chatbot/rememberKey", rememberKeyCheck->isChecked());
    if (rememberKeyCheck->isChecked())
        settings.setValue("Chatbot/apiKey", apiKeyEdit->text().trimmed());
    else
        settings.remove("Chatbot/apiKey");
    settings.sync();
    accept();
}
