#include <windows.h>
#include <stdio.h>
#include "set_monitor_area.h"

static bool wasPressed = false;
POINT topLeft, bottomRight, monitorArea;
int width, height;

// 사용자가 설정할 화면 영역 설정 함수
void setMonitorArea() {
    printf("스페이스바를 눌러 두 지점을 클릭하세요.\n");
    while (!GetAsyncKeyState(VK_SPACE)) {
        Sleep(100);
    }

    printf("첫 번째 클릭으로 좌상단 지점을 선택하세요.\n");
    while (true) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            if (!wasPressed) {
                GetCursorPos(&topLeft);
                monitorArea = topLeft;
                printf("좌상단 지점 클릭: (%d, %d)\n", monitorArea.x, monitorArea.y);
                wasPressed = true;
                Sleep(200);
                break;
            }
        }
        else {
            wasPressed = false;
        }
        Sleep(100);
    }

    printf("두 번째 클릭으로 우하단 지점을 선택하세요.\n");
    while (true) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            if (!wasPressed) {
                GetCursorPos(&bottomRight);
                printf("우하단 지점 클릭: (%d, %d)\n", bottomRight.x, bottomRight.y);
                wasPressed = true;
                Sleep(200);
                break;
            }
        }
        else {
            wasPressed = false;
        }
        Sleep(100);
    }

    // 좌상단과 우하단 좌표로 영역 계산
    width = bottomRight.x - topLeft.x;
    height = bottomRight.y - topLeft.y;

    printf("지정된 영역: 좌상단 (%d, %d), 우하단 (%d, %d), 너비: %d, 높이: %d\n",
        monitorArea.x, monitorArea.y, bottomRight.x, bottomRight.y, width, height);
    printf("모니터 영역이 성공적으로 설정되었습니다.\n");
}

// 설정된 화면 영역 반환
POINT getMonitorArea() {
    return monitorArea;
}

// 설정된 영역의 너비 반환
int getMonitorWidth() {
    return width;
}

// 설정된 영역의 높이 반환
int getMonitorHeight() {
    return height;
}
