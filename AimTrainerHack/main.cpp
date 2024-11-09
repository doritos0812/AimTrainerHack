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
    isFpsModeCheck();
    setMonitorArea();
    setMonitorCenter();
    setTargetColor(getFpsMode(), getMonitorCenter());
    initializeCaptureAndDetect();

    // 시작 버튼 누를 시간 제공
    printf("Get Ready!! Starting in 3 sec...");
    Sleep(3000);

    while (1) {
        if (checkForExitKey()) {
            printf("Exiting...\n");
            break;
        }
        captureAndDetectColor();
        Sleep(30); // 주기적 탐지
    }
    return 0;
}
