#include <Windows.h>

#include <vector>
#include <string>
//#include <iostream>
#include <mutex>
#include <random>

#include "../menu/menu.h"

#include "throwPot.h"

BOOL CALLBACK peichereFenster(HWND hwnd, LPARAM lParam) {
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

void throwPot::run()
{
    throwPot::findWindow();

    while (true)
    {
        if (settings::throwPotEnabled && GetAsyncKeyState('R') & 1)
        {
            throwPot::throwPotion();
        }
    }
}

void throwPotion1(char keyFirstSlot, char keyPotSlot)
{
    auto device = std::random_device();
    auto engine = std::mt19937(device());
    const auto gen = std::uniform_real_distribution<double>(settings::delay - 10, settings::delay + 10);
    double delay = gen(engine);

    auto device2 = std::random_device();
    auto engine2 = std::mt19937(device2());
    const auto gen2 = std::uniform_real_distribution<double>(1, 10);
    double delay2 = gen2(engine2);

    POINT p;

    Sleep(delay2);

    keybd_event(VkKeyScan(keyPotSlot), 0, 0, 0);

    Sleep(delay2);

    keybd_event(VkKeyScan(keyPotSlot), 0, KEYEVENTF_KEYUP, 0);

    Sleep(delay);

    GetCursorPos(&p);
    SendMessage((HWND)throwPot::mcWindow, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(p.x, p.y));

    Sleep(delay2);

    SendMessage((HWND)throwPot::mcWindow, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(p.x, p.y));

    Sleep(delay);

    keybd_event(VkKeyScan(keyFirstSlot), 0, 0, 0);

    Sleep(delay2);

    keybd_event(VkKeyScan(keyFirstSlot), 0, KEYEVENTF_KEYUP, 0);
}

void throwPot::throwPotion()
{
    switch (throwPot::nextSlot)
    {
    case 3:
        throwPotion1('F', 'Z');
        throwPot::nextSlot = 4;
        break;
    case 4:
        throwPotion1('F', '4');
        throwPot::nextSlot = 5;
        break;
    case 5:
        throwPotion1('F', 'H');
        throwPot::nextSlot = 6;
        break;
    case 6:
        throwPotion1('F', '6');
        throwPot::nextSlot = 7;
        break;
    case 7:
        throwPotion1('F', '7');
        throwPot::nextSlot = 8;
        break;
    case 8:
        throwPotion1('F', '8');
        throwPot::nextSlot = 9;
        break;
    case 9:
        throwPotion1('F', 'G');
        throwPot::nextSlot = 3;
        break;
    }
}

void throwPot::findWindow()
{
    std::vector<std::wstring> titles;
    EnumWindows(peichereFenster, reinterpret_cast<LPARAM>(&titles));
    for (const auto& title : titles)
    {
        if (((title.find(L"Minecraft") != std::string::npos) || (title.find(L"Lunar Client") != std::string::npos)) && (title.find(L"Launcher") == std::string::npos) && (title.find(L"output") == std::string::npos) && (title.find(L"Youtube") == std::string::npos) && (title.find(L"Brave") == std::string::npos) && (title.find(L"1.") != std::string::npos))
        {
            throwPot::mcWindow = (int)FindWindow(NULL, title.c_str());
        }
    }
}