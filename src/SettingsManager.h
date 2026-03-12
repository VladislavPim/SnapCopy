#pragma once
#include <QObject>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject *parent = nullptr);
    ~SettingsManager();
};