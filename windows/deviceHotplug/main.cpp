#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <dbt.h>
#include <string>

using namespace std;
#define WND_CLASS_NAME TEXT("SampleAppWindowClass")

wstring appName = L"TEST";
PWSTR g_pszAppName = &appName[0];

INT_PTR WINAPI WinProcCallback(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
) {
    return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL InitWindowClass() {
    WNDCLASSEX wndClass;

    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndClass.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));
    wndClass.lpfnWndProc = reinterpret_cast<WNDPROC>(WinProcCallback);
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
    wndClass.hbrBackground = CreateSolidBrush(RGB(192, 192, 192));
    wndClass.hCursor = LoadCursor(0, IDC_ARROW);
    wndClass.lpszClassName = WND_CLASS_NAME;
    wndClass.lpszMenuName = NULL;
    wndClass.hIconSm = wndClass.hIcon;


    if (!RegisterClassEx(&wndClass))
    {
        /*ErrorHandler(L"RegisterClassEx");*/
        return FALSE;
    }
    return TRUE;
}


void MessagePump(
    HWND hWnd
)
// Routine Description:
//     Simple main thread message pump.
//

// Parameters:
//     hWnd - handle to the window whose messages are being dispatched

// Return Value:
//     None.
{
    MSG msg;
    int retVal;

    // Get all messages for any window that belongs to this thread,
    // without any filtering. Potential optimization could be
    // obtained via use of filter values if desired.

    while ((retVal = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (retVal == -1)
        {
            /*ErrorHandler(L"GetMessage");*/
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}


int main() {
    if (!InitWindowClass())
    {
        // InitWindowClass displays any errors
        return -1;
    }

    auto hInstanceExe = GetModuleHandle(NULL);;

    HWND hWnd = CreateWindowEx(
        WS_EX_CLIENTEDGE | WS_EX_APPWINDOW,
        WND_CLASS_NAME,
        g_pszAppName,
        WS_OVERLAPPEDWINDOW, // style
        CW_USEDEFAULT, 0,
        640, 480,
        NULL, NULL,
        hInstanceExe,
        NULL);

    if (hWnd == NULL)
    {
        //ErrorHandler(L"CreateWindowEx: main appwindow hWnd");
        auto error = GetLastError();
        cout << "Failure in CreateWindowEx: " + error << endl;
        return -2;
    }
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);
    MessagePump(hWnd);
    return 0;


}