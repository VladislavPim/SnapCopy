#include "HistoryWindow.h"
#include "Logger.h"
#include <QDir>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>

HistoryWindow::HistoryWindow(QWidget *parent)
    : QWidget(parent)
    , m_currentTheme("dark")
{
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setFixedSize(400, 350);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Поиск...");
    connect(m_searchEdit, &QLineEdit::textChanged, this, &HistoryWindow::onSearchTextChanged);
    mainLayout->addWidget(m_searchEdit);

    m_listWidget = new QListWidget(this);
    connect(m_listWidget, &QListWidget::itemClicked, this, &HistoryWindow::onItemClicked);
    mainLayout->addWidget(m_listWidget);

    QHBoxLayout *btnLayout = new QHBoxLayout();

    m_copyBtn = new QPushButton("Копировать", this);
    m_copyBtn->setEnabled(false);
    connect(m_copyBtn, &QPushButton::clicked, this, &HistoryWindow::onCopyClicked);

    m_removeBtn = new QPushButton("Удалить", this);
    m_removeBtn->setEnabled(false);
    connect(m_removeBtn, &QPushButton::clicked, this, &HistoryWindow::onRemoveClicked);

    m_clearBtn = new QPushButton("Очистить всё", this);
    connect(m_clearBtn, &QPushButton::clicked, this, &HistoryWindow::clearHistory);

    btnLayout->addWidget(m_copyBtn);
    btnLayout->addWidget(m_removeBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(m_clearBtn);

    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);

    updateStyleSheet("dark");
    Logger::instance().log(LOG_INFO, "HistoryWindow created");
}

HistoryWindow::~HistoryWindow()
{
    Logger::instance().log(LOG_INFO, "HistoryWindow destroyed");
}

void HistoryWindow::addItem(const QString &text)
{
    QString displayText = text;
    if (displayText.length() > 100) {
        displayText = displayText.left(100) + "...";
    }
    QListWidgetItem *item = new QListWidgetItem(displayText);
    m_fullTextMap[item] = text;
    m_listWidget->addItem(item);
    m_listWidget->scrollToBottom();

    QDateTime now = QDateTime::currentDateTime();
    saveToCache(text, now);

    Logger::instance().log(LOG_DEBUG, QString("Item added to history, size %1 bytes").arg(text.toUtf8().size()));
}

void HistoryWindow::clearHistory()
{
    m_listWidget->clear();
    m_fullTextMap.clear();
    m_copyBtn->setEnabled(false);
    m_removeBtn->setEnabled(false);
    Logger::instance().log(LOG_INFO, "History cleared");
}

void HistoryWindow::focusSearch()
{
    if (!m_searchEdit) {
        Logger::instance().log(LOG_ERROR, "m_searchEdit is null in focusSearch");
        return;
    }
    m_searchEdit->setFocus();
    m_searchEdit->selectAll();
}

void HistoryWindow::hideEvent(QHideEvent *event)
{
    m_searchEdit->clear();
    QWidget::hideEvent(event);
}

void HistoryWindow::onSearchTextChanged(const QString &text)
{
    for (int i = 0; i < m_listWidget->count(); ++i) {
        QListWidgetItem *item = m_listWidget->item(i);
        QString full = m_fullTextMap.value(item);
        bool match = full.contains(text, Qt::CaseInsensitive);
        item->setHidden(!match);
    }
}

void HistoryWindow::applyTheme(const QString &theme)
{
    m_currentTheme = theme;
    updateStyleSheet(theme);
    Logger::instance().log(LOG_DEBUG, "HistoryWindow theme applied: " + theme);
}

void HistoryWindow::updateStyleSheet(const QString &theme)
{
    if (theme == "light") {
        setStyleSheet("background-color: #f0f0f0; color: black;");
        m_searchEdit->setStyleSheet("background-color: white; color: black; border: 1px solid #aaa; padding: 3px;");
        m_listWidget->setStyleSheet(
            "QListWidget { background-color: white; border: 1px solid #ccc; }"
            "QListWidget::item { color: black; padding: 4px; border-bottom: 1px solid #ddd; }"
            "QListWidget::item:selected { background-color: #c0c0c0; }"
        );
        m_copyBtn->setStyleSheet("QPushButton { background-color: #e0e0e0; color: black; border: 1px solid #aaa; padding: 5px; border-radius: 3px; }"
                                 "QPushButton:hover { background-color: #d0d0d0; }"
                                 "QPushButton:disabled { background-color: #f0f0f0; color: #888; }");
        m_removeBtn->setStyleSheet("QPushButton { background-color: #f0a0a0; color: black; border: 1px solid #a00; padding: 5px; border-radius: 3px; }"
                                   "QPushButton:hover { background-color: #e08080; }"
                                   "QPushButton:disabled { background-color: #f0d0d0; color: #888; }");
        m_clearBtn->setStyleSheet("QPushButton { background-color: #c0c0c0; color: black; border: 1px solid #888; padding: 5px; border-radius: 3px; }"
                                  "QPushButton:hover { background-color: #b0b0b0; }");
    } else {
        setStyleSheet("background-color: #2d2d2d; color: white;");
        m_searchEdit->setStyleSheet("background-color: #3e3e3e; color: white; border: 1px solid #555; padding: 3px;");
        m_listWidget->setStyleSheet(
            "QListWidget { background-color: #3e3e3e; border: 1px solid #555; }"
            "QListWidget::item { color: white; padding: 4px; border-bottom: 1px solid #555; }"
            "QListWidget::item:selected { background-color: #2d7ca0; }"
        );
        m_copyBtn->setStyleSheet("QPushButton { background-color: #2d7ca0; color: white; border: none; padding: 5px; border-radius: 3px; }"
                                 "QPushButton:hover { background-color: #3a9ad9; }"
                                 "QPushButton:disabled { background-color: #555; }");
        m_removeBtn->setStyleSheet("QPushButton { background-color: #a02d2d; color: white; border: none; padding: 5px; border-radius: 3px; }"
                                   "QPushButton:hover { background-color: #c23b3b; }"
                                   "QPushButton:disabled { background-color: #555; }");
        m_clearBtn->setStyleSheet("QPushButton { background-color: #555; color: white; border: none; padding: 5px; border-radius: 3px; }"
                                  "QPushButton:hover { background-color: #777; }");
    }
}

void HistoryWindow::onItemClicked(QListWidgetItem *item)
{
    m_copyBtn->setEnabled(true);
    m_removeBtn->setEnabled(true);
}

void HistoryWindow::onRemoveClicked()
{
    int row = m_listWidget->currentRow();
    if (row >= 0) {
        QListWidgetItem *item = m_listWidget->takeItem(row);
        m_fullTextMap.remove(item);
        delete item;
    }
    m_copyBtn->setEnabled(m_listWidget->currentItem() != nullptr);
    m_removeBtn->setEnabled(m_listWidget->currentItem() != nullptr);
}

void HistoryWindow::onCopyClicked()
{
    QListWidgetItem *item = m_listWidget->currentItem();
    if (item) {
        QString fullText = m_fullTextMap.value(item);
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(fullText);
        Logger::instance().log(LOG_DEBUG, "Copied to clipboard: " + fullText.left(50) + "...");
    }
}

void HistoryWindow::saveToCache(const QString &text, const QDateTime &timestamp)
{
    QDir dir(QCoreApplication::applicationDirPath());
    if (!dir.exists("cache")) {
        dir.mkdir("cache");
    }
    QString filename = QString("cache/%1.snap").arg(timestamp.toString("yyyy-MM-dd_hh-mm-ss"));
    QFile file(dir.absoluteFilePath(filename));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << text;
        file.close();
        Logger::instance().log(LOG_DEBUG, QString("Saved to cache: %1, size %2 bytes").arg(filename).arg(text.toUtf8().size()));
    } else {
        Logger::instance().log(LOG_ERROR, "Failed to write cache file: " + filename);
    }
}