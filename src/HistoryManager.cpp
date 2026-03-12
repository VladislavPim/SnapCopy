#include "HistoryManager.h"

HistoryManager::HistoryManager(QObject *parent)
    : QObject(parent)
{
}

void HistoryManager::addItem(const QString &text)
{
    // Избегаем дубликатов подряд (можно убрать)
    if (!m_items.isEmpty() && m_items.first() == text)
        return;

    m_items.prepend(text); // новый элемент в начало
    if (m_items.size() > m_maxSize) {
        m_items.removeLast();
    }
    emit historyChanged();
}

void HistoryManager::removeAt(int index)
{
    if (index >= 0 && index < m_items.size()) {
        m_items.removeAt(index);
        emit historyChanged();
    }
}

void HistoryManager::clear()
{
    m_items.clear();
    emit historyChanged();
}

QStringList HistoryManager::getAll() const
{
    return m_items;
}