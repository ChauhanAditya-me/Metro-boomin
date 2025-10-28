#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QLineEdit;
class QCheckBox;
class QPushButton;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

private slots:
    void saveSettings();

private:
    QLineEdit *apiKeyEdit;
    QCheckBox *enableRemoteCheck;
    QCheckBox *rememberKeyCheck;
    QPushButton *saveBtn;
    QPushButton *cancelBtn;
};

#endif // SETTINGSDIALOG_H
