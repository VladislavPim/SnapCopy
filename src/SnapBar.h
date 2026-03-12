#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSettings>
#include <QPoint>
#include <QPointer>
#include "ClipboardListener.h"

class HistoryWindow;
class SettingsWindow;

class SnapBar : public QWidget
{
    Q_OBJECT

public:
    explicit SnapBar(QWidget *parent = nullptr);
    ~SnapBar();

    void showHistory();
    void hideHistory();
    bool isHistoryVisible() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onExpandClicked();
    void onSettingsClicked();
    void onTextCopied(const QString &text);

private:
    void loadPosition();
    void savePosition();
    void updateHistoryPosition();

    QLabel *m_titleLabel;
    QPushButton *m_expandBtn;
    QPushButton *m_settingsBtn;
    QPoint m_dragPosition;
    bool m_dragging;

    ClipboardListener *m_listener;
    QPointer<HistoryWindow> m_historyWindow;
    SettingsWindow *m_settingsWindow;

    bool m_historyAbove;
};