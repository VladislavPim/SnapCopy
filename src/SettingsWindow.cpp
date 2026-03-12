#include "SettingsWindow.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
    , m_currentTheme("dark")
{
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setFixedSize(350, 250);

    QVBoxLayout *layout = new QVBoxLayout(this);

    m_autoStartCheck = new QCheckBox("Автозагрузка", this);
    layout->addWidget(m_autoStartCheck);

    QHBoxLayout *fontLayout = new QHBoxLayout();
    fontLayout->addWidget(new QLabel("Размер текста:", this));
    m_fontSizeSlider = new QSlider(Qt::Horizontal, this);
    m_fontSizeSlider->setRange(8, 24);
    m_fontSizeSlider->setValue(12);
    fontLayout->addWidget(m_fontSizeSlider);
    layout->addLayout(fontLayout);

    QHBoxLayout *themeLayout = new QHBoxLayout();
    themeLayout->addWidget(new QLabel("Тема:", this));
    m_themeCombo = new QComboBox(this);
    m_themeCombo->addItem("Тёмная", "dark");
    m_themeCombo->addItem("Светлая", "light");
    themeLayout->addWidget(m_themeCombo);
    layout->addLayout(themeLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    m_saveBtn = new QPushButton("Сохранить", this);
    m_cancelBtn = new QPushButton("Отмена", this);
    btnLayout->addWidget(m_saveBtn);
    btnLayout->addWidget(m_cancelBtn);
    layout->addLayout(btnLayout);

    connect(m_saveBtn, &QPushButton::clicked, this, &SettingsWindow::onSave);
    connect(m_cancelBtn, &QPushButton::clicked, this, &SettingsWindow::onCancel);
    connect(m_themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SettingsWindow::onThemeChanged);

    loadSettings();
    updateStyleSheet(m_currentTheme);
    qDebug() << "SettingsWindow created";
}

SettingsWindow::~SettingsWindow()
{
    qDebug() << "SettingsWindow destroyed";
}

void SettingsWindow::applyTheme(const QString &theme)
{
    m_currentTheme = theme;
    updateStyleSheet(theme);
    // обновить выбор в комбобоксе
    int idx = m_themeCombo->findData(theme);
    if (idx >= 0) {
        m_themeCombo->setCurrentIndex(idx);
    }
    qDebug() << "SettingsWindow theme applied:" << theme;
}

void SettingsWindow::updateStyleSheet(const QString &theme)
{
    if (theme == "light") {
        setStyleSheet("background-color: #f0f0f0; color: black;");
        m_autoStartCheck->setStyleSheet("color: black;");
        m_fontSizeSlider->setStyleSheet("background-color: #e0e0e0;");
        m_themeCombo->setStyleSheet("background-color: white; color: black; border: 1px solid #aaa;");
        m_saveBtn->setStyleSheet("background-color: #c0c0c0; color: black; border: 1px solid #888; padding: 5px; border-radius: 3px;");
        m_cancelBtn->setStyleSheet("background-color: #c0c0c0; color: black; border: 1px solid #888; padding: 5px; border-radius: 3px;");
    } else {
        setStyleSheet("background-color: #2d2d2d; color: white;");
        m_autoStartCheck->setStyleSheet("color: white;");
        m_fontSizeSlider->setStyleSheet("background-color: #3e3e3e;");
        m_themeCombo->setStyleSheet("background-color: #3e3e3e; color: white; border: 1px solid #555;");
        m_saveBtn->setStyleSheet("background-color: #555; color: white; border: none; padding: 5px; border-radius: 3px;");
        m_cancelBtn->setStyleSheet("background-color: #555; color: white; border: none; padding: 5px; border-radius: 3px;");
    }
}

void SettingsWindow::loadSettings()
{
    QSettings settings;
    settings.beginGroup("Settings");
    bool autoStart = settings.value("autoStart", false).toBool();
    int fontSize = settings.value("fontSize", 12).toInt();
    QString theme = settings.value("theme", "dark").toString();
    settings.endGroup();

    m_autoStartCheck->setChecked(autoStart);
    m_fontSizeSlider->setValue(fontSize);
    int idx = m_themeCombo->findData(theme);
    if (idx >= 0) m_themeCombo->setCurrentIndex(idx);
    m_currentTheme = theme;

    qDebug() << "Settings loaded: autoStart =" << autoStart << ", fontSize =" << fontSize << ", theme =" << theme;
}

void SettingsWindow::saveSettings()
{
    QSettings settings;
    settings.beginGroup("Settings");
    settings.setValue("autoStart", m_autoStartCheck->isChecked());
    settings.setValue("fontSize", m_fontSizeSlider->value());
    settings.setValue("theme", m_themeCombo->currentData().toString());
    settings.endGroup();

#ifdef Q_OS_WIN
    QString appPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
    QSettings regSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (m_autoStartCheck->isChecked()) {
        regSettings.setValue("SnapCopy", appPath);
        qDebug() << "Auto-start enabled, path:" << appPath;
    } else {
        regSettings.remove("SnapCopy");
        qDebug() << "Auto-start disabled";
    }
#endif
    emit themeChanged(m_themeCombo->currentData().toString());
}

void SettingsWindow::onSave()
{
    qDebug() << "Settings saved";
    saveSettings();
    emit settingsChanged();
    accept();
}

void SettingsWindow::onCancel()
{
    qDebug() << "Settings cancelled";
    reject();
}

void SettingsWindow::onThemeChanged(int index)
{
    QString theme = m_themeCombo->itemData(index).toString();
    applyTheme(theme);
    // не сохраняем, пока не нажато Save
}