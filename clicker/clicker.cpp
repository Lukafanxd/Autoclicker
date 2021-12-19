#include <Windows.h>

#include <vector>
#include <string>
//#include <iostream>
#include <mutex>
#include <random>

#include "../menu/menu.h"

#include "clicker.h"

std::once_flag flag;

BOOL CALLBACK speichereFenster(HWND hwnd, LPARAM lParam) {
    const DWORD TITLE_SIZE = 1024;
    WCHAR windowTitle[TITLE_SIZE];

    GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

    int length = ::GetWindowTextLength(hwnd);
    std::wstring title(&windowTitle[0]);
    if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager") {
        return TRUE;
    }

    // Retrieve the pointer passed into this callback, and re-'type' it.
    // The only way for a C API to pass arbitrary data is by means of a void*.
    std::vector<std::wstring>& titles =
        *reinterpret_cast<std::vector<std::wstring>*>(lParam);
    titles.push_back(title);

    return TRUE;
}

void clicker::run()
{
    clicker::findWindow();

    while (true)
    {
        if (settings::leftClickerEnabled && (GetAsyncKeyState(VK_BACK)))
        {
            clicker::click();
        }
    }
}

void clicker::click()
{
    auto device = std::random_device();
    auto engine = std::mt19937(device());
    const auto gen = std::uniform_real_distribution<double>(settings::leftMinCps, settings::leftMaxCps);
    double delay = gen(engine);

    auto device2 = std::random_device();
    auto engine2 = std::mt19937(device2());
    const auto gen2 = std::uniform_real_distribution<double>(1 + (delay * 0.0001), 1 + (delay * 0.1));

    float delay2 = gen2(engine2);

    POINT p;
    GetCursorPos(&p);
    SendMessage((HWND)clicker::mcWindow, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(p.x, p.y));

    Sleep(delay2 / 1000);

    GetCursorPos(&p);
    SendMessage((HWND)clicker::mcWindow, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(p.x, p.y));

    Sleep(1000 / (delay + delay2));
}

void clicker::findWindow()
{
    std::vector<std::wstring> titles;
    EnumWindows(speichereFenster, reinterpret_cast<LPARAM>(&titles));
    for (const auto& title : titles)
    {
        if (((title.find(L"Minecraft") != std::string::npos) || (title.find(L"Lunar Client") != std::string::npos)) && (title.find(L"Launcher") == std::string::npos) && (title.find(L"output") == std::string::npos) && (title.find(L"Youtube") == std::string::npos) && (title.find(L"Brave") == std::string::npos) && (title.find(L"1.") != std::string::npos))
        {
            clicker::mcWindow = (int)FindWindow(NULL, title.c_str());
        }
    }
}
