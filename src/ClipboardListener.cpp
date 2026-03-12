#include "ClipboardListener.h"
#include <QDebug>

const wchar_t* ClipboardListener::WINDOW_CLASS_NAME = L"SnapCopyClipboardWindow";

ClipboardListener::ClipboardListener(QObject *parent)
    : QObject(parent)
    , m_hwnd(nullptr)
{
}

ClipboardListener::~ClipboardListener()
{
    stopListening();
}

bool ClipboardListener::startListening()
{
    if (!createHiddenWindow()) {
        qDebug() << "Failed to create hidden window";
        return false;
    }

    if (!AddClipboardFormatListener(m_hwnd)) {
        qDebug() << "AddClipboardFormatListener failed";
        destroyHiddenWindow();
        return false;
    }

    qDebug() << "ClipboardListener started";
    return true;
}

void ClipboardListener::stopListening()
{
    if (m_hwnd) {
        RemoveClipboardFormatListener(m_hwnd);
        destroyHiddenWindow();
    }
}

bool ClipboardListener::createHiddenWindow()
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClassEx(&wc)) {
        qDebug() << "Failed to register window class";
        return false;
    }

    m_hwnd = CreateWindowEx(
        0,
        WINDOW_CLASS_NAME,
        L"SnapCopyHiddenWindow",
        0, 0, 0, 0, 0,
        HWND_MESSAGE, // невидимое окно только для сообщений
        nullptr, hInstance, this
    );

    if (!m_hwnd) {
        qDebug() << "Failed to create hidden window";
        return false;
    }

    return true;
}

void ClipboardListener::destroyHiddenWindow()
{
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
        UnregisterClass(WINDOW_CLASS_NAME, GetModuleHandle(nullptr));
    }
}

LRESULT CALLBACK ClipboardListener::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CLIPBOARDUPDATE) {
        // Буфер обмена изменился
        if (OpenClipboard(hwnd)) {
            HANDLE hData = GetClipboardData(CF_UNICODETEXT);
            if (hData) {
                wchar_t* wText = (wchar_t*)GlobalLock(hData);
                if (wText) {
                    QString text = QString::fromWCharArray(wText);
                    GlobalUnlock(hData);

                    // Отправляем сигнал
                    ClipboardListener* self = (ClipboardListener*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
                    if (self) {
                        emit self->textCopied(text);
                    }
                }
            }
            CloseClipboard();
        }
        return 0;
    }
    else if (msg == WM_NCCREATE) {
        // Сохраняем указатель на объект
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}