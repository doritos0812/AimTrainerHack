#include <windows.h>
#include <stdio.h>
#include "set_target_color.h"
#include "set_monitor_area.h"
#include "capture_and_detect.h"

bool checkForExitKey() {
    if (GetAsyncKeyState(VK_ESCAPE)) {
        return true;
    }
    return false;
}

int main() {
    setMonitorArea();
    setTargetColor();

    while (1) {
        if (checkForExitKey()) {
            printf("Exiting...\n");
            break;
        }
        captureAndDetectColor();
        Sleep(100); // 주기적 탐지
    }
    return 0;
}
