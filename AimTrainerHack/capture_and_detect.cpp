#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "capture_and_detect.h"
#include "set_target_color.h"
#include "set_monitor_area.h"

// 전역 변수들로 선언하여 최초에만 초기화
static POINT monitorArea;
static int width;
static int height;
static bool isFpsMode;
static RGBColor targetColor;
static POINT center;

// 최초 한 번만 호출될 초기화 함수
void initializeCaptureAndDetect() {
    monitorArea = getMonitorArea();
    width = getMonitorWidth();
    height = getMonitorHeight();
    isFpsMode = getFpsMode();
    targetColor = getTargetColor();
    setMonitorCenter();
    center = getMonitorCenter();
}

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

// 색상 비교 함수
bool isPixelColorCorrect(int x, int y, int width, BYTE* pPixels, RGBColor color) {
    // 전역으로 설정된 width 값을 사용
    int index = (y * width + x) * 3;  // width 값 사용
    BYTE blue = pPixels[index];
    BYTE green = pPixels[index + 1];
    BYTE red = pPixels[index + 2];

    // 색상 비교
    return (red == color.red && green == color.green && blue == color.blue);
}

// 감지 후 작업 수행 함수
void performActionOnDetection(int x, int y) {
    printf("Detected target color at (%d, %d)\n", x + monitorArea.x, y + monitorArea.y);

    if (isFpsMode) {
        int dx = x - center.x;
        int dy = y - center.y;
        printf("dx = %d, dy = %d, center = (%d, %d)\n", dx, dy, center.x, center.y);
        mouse_event(MOUSEEVENTF_MOVE, dx, dy, 0, 0);
    }
    else {
        SetCursorPos(x + monitorArea.x, y + monitorArea.y);
    }
    mouseClick();
}

// 화면 캡쳐 및 색상 감지 함수
void captureAndDetectColor() {
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
            if (!isPixelColorCorrect(x, y, width, pPixels, targetColor)) continue;

            performActionOnDetection(x, y);

            // 메모리 정리 후 종료
            free(pPixels);
            DeleteObject(hBitmap);
            DeleteDC(hdcMem);
            ReleaseDC(NULL, hdcScreen);
            return;
        }
    }

    free(pPixels);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}
