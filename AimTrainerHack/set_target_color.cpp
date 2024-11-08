#include <windows.h>
#include <stdio.h>
#include "set_target_color.h"

static bool wasPressed = false;
RGBColor targetColor = { 0, 0, 0 }; // 탐지할 색상 초기화

// 클릭한 좌표의 RGB 값을 가져오는 함수
RGBColor getColorAtCursor(POINT pt) {
    RGBColor color;
    HDC hdcScreen = GetDC(NULL);
    COLORREF pixelColor = GetPixel(hdcScreen, pt.x, pt.y);
    color.red = GetRValue(pixelColor);
    color.green = GetGValue(pixelColor);
    color.blue = GetBValue(pixelColor);
    ReleaseDC(NULL, hdcScreen);
    return color;
}

// 사용자 클릭을 통해 탐지할 색상을 설정하는 함수
void setTargetColor() {
    printf("탐지할 색상을 설정하려면 원하는 위치에 커서를 두고 F3를 클릭하세요...\n");

    while (1) {
        if (GetAsyncKeyState(VK_F3) & 0x8000) {  // 마우스 왼쪽 버튼이 눌림 상태일 때
            if (!wasPressed) {
                POINT pt;
                GetCursorPos(&pt);
                targetColor = getColorAtCursor(pt); // 클릭 지점의 색상 설정
                printf("탐지할 색상 RGB: (%d, %d, %d)\n", targetColor.red, targetColor.green, targetColor.blue);
                wasPressed = true; // 눌린 상태로 설정
                Sleep(200);
                break;
            }
        }
        else {
            wasPressed = false;
        }
        Sleep(50); // CPU 사용량 절약
    }
}

// 설정된 색상 반환
RGBColor getTargetColor() {
    return targetColor;
}
