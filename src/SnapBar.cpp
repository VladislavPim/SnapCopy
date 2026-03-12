#include "SnapBar.h"
#include "HistoryWindow.h"
#include "SettingsWindow.h"
#include "Logger.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>

SnapBar::SnapBar(QWidget *parent)
    : QWidget(parent)
    , m_dragging(false)
    , m_listener(new ClipboardListener(this))
    , m_historyWindow(nullptr)
    , m_settingsWindow(nullptr)
    , m_historyAbove(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setFixedSize(400, 40);

    m_titleLabel = new QLabel("SnapCopy", this);
    m_titleLabel->setStyleSheet("color: white; font-weight: bold;");

    m_expandBtn = new QPushButton("▼", this);
    m_expandBtn->setFixedSize(30, 30);
    m_expandBtn->setStyleSheet(
        "QPushButton {"
        "   background: none;"
        "   color: white;"
        "   border: none;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover { color: #aaa; }"
    );

    m_settingsBtn = new QPushButton("⋮", this);
    m_settingsBtn->setFixedSize(30, 30);
    m_settingsBtn->setStyleSheet(
        "QPushButton {"
        "   background: none;"
        "   color: white;"
        "   border: none;"
        "   font-size: 16px;"
        "}"
        "QPushButton:hover { color: #aaa; }"
    );

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(0);
    layout->addWidget(m_titleLabel);
    layout->addStretch();
    layout->addWidget(m_expandBtn);
    layout->addWidget(m_settingsBtn);

    setLayout(layout);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(40, 40, 40, 200));
    setPalette(pal);
    setAutoFillBackground(true);
    setStyleSheet("border-radius: 5px;");

    connect(m_expandBtn, &QPushButton::clicked, this, &SnapBar::onExpandClicked);
    connect(m_settingsBtn, &QPushButton::clicked, this, &SnapBar::onSettingsClicked);

    loadPosition();

    connect(m_listener, &ClipboardListener::textCopied, this, &SnapBar::onTextCopied);
    if (!m_listener->startListening()) {
        Logger::instance().log(LOG_ERROR, "Failed to start clipboard listener");
    } else {
        Logger::instance().log(LOG_INFO, "Clipboard listener started");
    }

    Logger::instance().log(LOG_INFO, "SnapBar created");
}

SnapBar::~SnapBar()
{
    savePosition();
    if (m_historyWindow) {
        m_historyWindow->close();
    }
    if (m_settingsWindow) {
        m_settingsWindow->close();
    }
    Logger::instance().log(LOG_INFO, "SnapBar destroyed");
}

void SnapBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        m_dragging = true;
        event->accept();
    }
}

void SnapBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_dragPosition);
        updateHistoryPosition();
        event->accept();
    }
}

void SnapBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        savePosition();
        updateHistoryPosition();
        event->accept();
    }
}

void SnapBar::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (x() == 0 && y() == 0) {
        QScreen *screen = QApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();
        move(screenGeometry.width() - width() - 10,
             screenGeometry.height() - height() - 10);
    }
}

void SnapBar::updateHistoryPosition()
{
    if (!m_historyWindow || !m_historyWindow->isVisible()) return;

    QPoint newPos;
    if (m_historyAbove) {
        newPos = mapToGlobal(QPoint(0, -m_historyWindow->height()));
    } else {
        newPos = mapToGlobal(QPoint(0, height()));
    }

    QScreen *screen = QApplication::primaryScreen();
    QRect screenRect = screen->availableGeometry();
    int windowHeight = m_historyWindow->height();

    if (!m_historyAbove && newPos.y() + windowHeight > screenRect.bottom()) {
        newPos.setY(mapToGlobal(QPoint(0, -windowHeight)).y());
        m_historyAbove = true;
    } else if (m_historyAbove && newPos.y() < screenRect.top()) {
        newPos.setY(mapToGlobal(QPoint(0, height())).y());
        m_historyAbove = false;
    }

    m_historyWindow->move(newPos);
}

void SnapBar::onExpandClicked()
{
    Logger::instance().log(LOG_DEBUG, "Expand clicked");
    if (!m_historyWindow) {
        Logger::instance().log(LOG_INFO, "Creating HistoryWindow");
        m_historyWindow = new HistoryWindow(nullptr);
        QSettings settings;
        settings.beginGroup("Settings");
        QString theme = settings.value("theme", "dark").toString();
        settings.endGroup();
        if (m_historyWindow) {
            m_historyWindow->applyTheme(theme);
        }
    }

    if (m_historyWindow && m_historyWindow->isVisible()) {
        m_historyWindow->hide();
    } else if (m_historyWindow) {
        QPoint pos = mapToGlobal(QPoint(0, height()));
        QScreen *screen = QApplication::primaryScreen();
        QRect screenRect = screen->availableGeometry();
        int windowHeight = m_historyWindow->height();

        if (pos.y() + windowHeight > screenRect.bottom()) {
            pos.setY(mapToGlobal(QPoint(0, -windowHeight)).y());
            m_historyAbove = true;
        } else {
            m_historyAbove = false;
        }

        m_historyWindow->move(pos);
        m_historyWindow->show();
        Logger::instance().log(LOG_DEBUG, "HistoryWindow shown");
    }
}

void SnapBar::onSettingsClicked()
{
    Logger::instance().log(LOG_DEBUG, "Settings clicked");
    if (!m_settingsWindow) {
        Logger::instance().log(LOG_INFO, "Creating SettingsWindow");
        m_settingsWindow = new SettingsWindow(nullptr);
        connect(m_settingsWindow, &SettingsWindow::settingsChanged, this, [this]() {
            QSettings settings;
            settings.beginGroup("Settings");
            int fontSize = settings.value("fontSize", 12).toInt();
            QString theme = settings.value("theme", "dark").toString();
            settings.endGroup();
            Logger::instance().log(LOG_INFO, QString("Font size changed to %1, theme %2").arg(fontSize).arg(theme));
            if (m_historyWindow) {
                m_historyWindow->applyTheme(theme);
            }
        });
        connect(m_settingsWindow, &SettingsWindow::themeChanged, this, [this](const QString &theme) {
            if (m_historyWindow) {
                m_historyWindow->applyTheme(theme);
            }
        });
        QSettings settings;
        settings.beginGroup("Settings");
        QString theme = settings.value("theme", "dark").toString();
        settings.endGroup();
        m_settingsWindow->applyTheme(theme);
    }
    QPoint center = QApplication::primaryScreen()->geometry().center();
    m_settingsWindow->move(center - m_settingsWindow->rect().center());
    m_settingsWindow->show();
    m_settingsWindow->raise();
    m_settingsWindow->activateWindow();
}

void SnapBar::onTextCopied(const QString &text)
{
    Logger::instance().log(LOG_DEBUG, QString("Copied text, size %1 bytes").arg(text.toUtf8().size()));
    if (!m_historyWindow) {
        m_historyWindow = new HistoryWindow(nullptr);
        QSettings settings;
        settings.beginGroup("Settings");
        QString theme = settings.value("theme", "dark").toString();
        settings.endGroup();
        if (m_historyWindow) {
            m_historyWindow->applyTheme(theme);
        }
    }
    if (m_historyWindow) {
        m_historyWindow->addItem(text);
    }
}

void SnapBar::showHistory()
{
    if (!m_historyWindow) onExpandClicked();
    else if (!m_historyWindow->isVisible()) onExpandClicked();
}

void SnapBar::hideHistory()
{
    if (m_historyWindow && m_historyWindow->isVisible())
        m_historyWindow->hide();
}

bool SnapBar::isHistoryVisible() const
{
    return m_historyWindow && m_historyWindow->isVisible();
}

void SnapBar::loadPosition()
{
    QSettings settings;
    settings.beginGroup("SnapBar");
    QPoint pos = settings.value("position", QPoint(0, 0)).toPoint();
    settings.endGroup();
    if (!pos.isNull() && pos != QPoint(0, 0)) {
        move(pos);
    }
}

void SnapBar::savePosition()
{
    QSettings settings;
    settings.beginGroup("SnapBar");
    settings.setValue("position", pos());
    settings.endGroup();
}