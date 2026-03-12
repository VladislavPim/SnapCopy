#pragma once

#include <QObject>
#include <windows.h>

class ClipboardListener : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardListener(QObject *parent = nullptr);
    ~ClipboardListener();

    bool startListening();
    void stopListening();

signals:
    void textCopied(QString text);

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool createHiddenWindow();
    void destroyHiddenWindow();

    HWND m_hwnd;
    static const wchar_t* WINDOW_CLASS_NAME;
};