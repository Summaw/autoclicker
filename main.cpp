#include <iostream>
#include <Windows.h>
#include <thread>
#include <atomic>

std::atomic<bool> isClicking(false);

void clickThread() {
    while (true) {
        if (isClicking) {
            INPUT input;
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(INPUT));

            Sleep(1);  
        }
        else {
            Sleep(10); 
        }
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        if (pKeyStruct->vkCode == VK_F2)
        {
            if (wParam == WM_KEYDOWN) {
                isClicking = !isClicking;
                std::cout << "Clicking " << (isClicking ? "started." : "stopped.") << std::endl;
            }
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

int main() {
    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, nullptr, 0);

    std::thread clicker(clickThread);
    clicker.detach();

    MSG message;
    while (GetMessage(&message, nullptr, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    UnhookWindowsHookEx(keyboardHook);

    return 0;
}
