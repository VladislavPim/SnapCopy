#pragma once

#include <QDialog>
#include <QCheckBox>
#include <QSlider>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>
#include <QComboBox>

class SettingsWindow : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

public slots:
    void applyTheme(const QString &theme); // применяет тему

signals:
    void settingsChanged();
    void themeChanged(const QString &theme); // сигнал об изменении темы

private slots:
    void onSave();
    void onCancel();
    void onThemeChanged(int index);

private:
    void loadSettings();
    void saveSettings();
    void updateStyleSheet(const QString &theme);

    QCheckBox *m_autoStartCheck;
    QSlider *m_fontSizeSlider;
    QComboBox *m_themeCombo;
    QPushButton *m_saveBtn;
    QPushButton *m_cancelBtn;
    QString m_currentTheme;
};