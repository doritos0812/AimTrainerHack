#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "capture_and_detect.h"
#include "set_target_color.h"
#include "set_monitor_area.h"

// mouse 클릭
void mouseClick() {
    INPUT inputs[2] = { 0 };
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, inputs, sizeof(INPUT));
    printf("Mouse clicked at the detected position.\n");
}

// 화면 캡쳐 및 색상 감지 함수
void captureAndDetectColor() {
    POINT monitorArea = getMonitorArea();
    int width = getMonitorWidth();
    int height = getMonitorHeight();
    RGBColor targetColor = getTargetColor();

    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcMem, hBitmap);

    // 화면에서 메모리 DC로 영역 복사
    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, monitorArea.x, monitorArea.y, SRCCOPY);

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    BYTE* pPixels = (BYTE*)malloc(3 * width * height);
    GetDIBits(hdcMem, hBitmap, 0, height, pPixels, &bmi, DIB_RGB_COLORS);

    // 픽셀 데이터 검사
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            BYTE blue = pPixels[index];
            BYTE green = pPixels[index + 1];
            BYTE red = pPixels[index + 2];

            if (red == targetColor.red && green == targetColor.green && blue == targetColor.blue) {
                printf("Detected target color at (%d, %d)\n", x + monitorArea.x, y + monitorArea.y);
                SetCursorPos(x + monitorArea.x, y + monitorArea.y);
                mouseClick();
                free(pPixels);
                DeleteObject(hBitmap);
                DeleteDC(hdcMem);
                ReleaseDC(NULL, hdcScreen);
                return;
            }
        }
    }

    free(pPixels);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}
