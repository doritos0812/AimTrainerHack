#include <windows.h>
#include <stdio.h>
#include "set_monitor_area.h"

// 전역 변수 초기화
int width = 0, height = 0;
POINT topLeft = { 0, 0 }, bottomRight = { 0, 0 };
bool wasPressed = false;

// 전체 화면 해상도 반환 함수
void getFullScreenResolution(int* width, int* height) {
    *width = GetSystemMetrics(SM_CXSCREEN);  // 화면 가로 해상도
    *height = GetSystemMetrics(SM_CYSCREEN); // 화면 세로 해상도
    printf("전체화면 해상도: %d x %d\n", *width, *height);
}

// 버튼을 기다리고 눌린 상태에 따라 동작을 처리하는 함수
bool waitForKeyPress(int key, bool* wasPressed, void (*action)()) {
    if (GetAsyncKeyState(key) & 0x8000) {  // 키가 눌렸을 때
        if (!(*wasPressed)) {
            action();  // 지정된 작업 실행
            *wasPressed = true;
            return true;
        }
    }
    else {
        *wasPressed = false;
    }
    return false;
}

// F2 버튼을 눌러서 좌상단과 우하단 좌표를 선택하는 함수
void selectArea() {
    printf("F2 버튼으로 좌상단 지점을 선택하세요.\n");
    while (!waitForKeyPress(VK_F2, &wasPressed, []() {
        GetCursorPos(&topLeft);
        printf("좌상단 지점: (%d, %d)\n", topLeft.x, topLeft.y);
        })) {
        Sleep(100);
    }

    printf("F2 버튼으로 우하단 지점을 선택하세요.\n");
    while (!waitForKeyPress(VK_F2, &wasPressed, []() {
        GetCursorPos(&bottomRight);
        printf("우하단 지점: (%d, %d)\n", bottomRight.x, bottomRight.y);
        })) {
        Sleep(100);
    }

    // 좌상단과 우하단 좌표로 영역 계산
    width = bottomRight.x - topLeft.x;
    height = bottomRight.y - topLeft.y;

    printf("지정된 영역: 좌상단 (%d, %d), 우하단 (%d, %d), 너비: %d, 높이: %d\n",
        topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, width, height);
}

// F 버튼을 눌러서 전체 화면 해상도를 설정하는 함수
void setFullScreenResolution() {
    getFullScreenResolution(&width, &height);
}

// 화면 영역 설정 함수
void setMonitorArea() {
    printf("F2 버튼을 눌러 마우스를 이용해 영역을 선택하거나, F 버튼을 눌러 전체 화면 해상도를 설정하세요.\n");

    // F 버튼을 눌렀을 때 전체 화면 해상도 적용
    while (true) {
        if (waitForKeyPress(0x46, &wasPressed, setFullScreenResolution)) {
            break;  // 해상도를 설정한 후 함수 종료
        }

        // F2 버튼을 눌렀을 때 마우스 클릭으로 영역 지정
        if (waitForKeyPress(VK_F2, &wasPressed, selectArea)) {
            break; // 영역 선택 후 함수 종료
        }

        Sleep(100); // CPU 사용량 절약
    }
}

// 설정된 화면 좌상단 영역 반환
POINT getMonitorArea() {
    return topLeft;
}

// 설정된 영역의 너비 반환
int getMonitorWidth() {
    return width;
}

// 설정된 영역의 높이 반환
int getMonitorHeight() {
    return height;
}
