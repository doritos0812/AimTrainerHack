#include <windows.h>
#include <stdio.h>
#include "set_target_color.h"

static bool wasPressed = false;
RGBColor targetColor = { 0, 0, 0 }; // Ž���� ���� �ʱ�ȭ

// Ŭ���� ��ǥ�� RGB ���� �������� �Լ�
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

// ����� Ŭ���� ���� Ž���� ������ �����ϴ� �Լ�
void setTargetColor() {
    printf("Ž���� ������ �����Ϸ��� ���ϴ� ��ġ�� Ŀ���� �ΰ� F3�� Ŭ���ϼ���...\n");

    while (1) {
        if (GetAsyncKeyState(VK_F3) & 0x8000) {  // ���콺 ���� ��ư�� ���� ������ ��
            if (!wasPressed) {
                POINT pt;
                GetCursorPos(&pt);
                targetColor = getColorAtCursor(pt); // Ŭ�� ������ ���� ����
                printf("Ž���� ���� RGB: (%d, %d, %d)\n", targetColor.red, targetColor.green, targetColor.blue);
                wasPressed = true; // ���� ���·� ����
                Sleep(200);
                break;
            }
        }
        else {
            wasPressed = false;
        }
        Sleep(50); // CPU ��뷮 ����
    }
}

// ������ ���� ��ȯ
RGBColor getTargetColor() {
    return targetColor;
}
