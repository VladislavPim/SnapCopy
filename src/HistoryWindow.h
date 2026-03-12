#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QClipboard>
#include <QApplication>
#include <QLineEdit>
#include <QString>

class HistoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryWindow(QWidget *parent = nullptr);
    ~HistoryWindow();

    void addItem(const QString &text);
    void clearHistory();
    void focusSearch();  // устанавливает фокус в поле поиска

public slots:
    void applyTheme(const QString &theme);
    void onSearchTextChanged(const QString &text);

protected:
    void hideEvent(QHideEvent *event) override;

private slots:
    void onItemClicked(QListWidgetItem *item);
    void onRemoveClicked();
    void onCopyClicked();

private:
    void updateRemoveButtonState();
    void updateStyleSheet(const QString &theme);
    void saveToCache(const QString &text, const QDateTime &timestamp);

    QListWidget *m_listWidget;
    QLineEdit *m_searchEdit;
    QPushButton *m_removeBtn;
    QPushButton *m_copyBtn;
    QPushButton *m_clearBtn;
    QString m_currentTheme;
    QMap<QListWidgetItem*, QString> m_fullTextMap;
};