#pragma once

#include <QObject>
#include <QStringList>

class HistoryManager : public QObject
{
    Q_OBJECT
public:
    explicit HistoryManager(QObject *parent = nullptr);

    void addItem(const QString &text);
    void removeAt(int index);
    void clear();
    QStringList getAll() const;

signals:
    void historyChanged();

private:
    QStringList m_items;
    int m_maxSize = 100; // ограничим размер
};