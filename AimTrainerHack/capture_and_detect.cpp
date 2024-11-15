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

void performActionOnDetection(int x, int y) {
    printf("Detected target color at (%d, %d)\n", x + monitorArea.x, y + monitorArea.y);

    if (isFpsMode) {
        // 초기 위치
        int currentX = center.x;
        int currentY = center.y;

        // 임계값 설정 (프레임당 최대 이동량)
        const int MAX_DELTA = 25;

        // 반복적으로 이동하여 목표 지점에 도달
        while (currentX != x || currentY != y) {
            int dx = x - currentX;
            int dy = y - currentY;

            // dx, dy를 임계값 내로 제한
            if (abs(dx) > MAX_DELTA) dx = (dx > 0) ? MAX_DELTA : -MAX_DELTA;
            if (abs(dy) > MAX_DELTA) dy = (dy > 0) ? MAX_DELTA : -MAX_DELTA;

            mouse_event(MOUSEEVENTF_MOVE, dx, dy, 0, 0);

            // 현재 위치 갱신
            currentX += dx;
            currentY += dy;

            Sleep(5);
        }

        printf("Reached final position: (%d, %d)\n", x, y);
    }
    else {
        POINT currentPos;
        GetCursorPos(&currentPos);

        // 초기 위치
        int currentX = currentPos.x;
        int currentY = currentPos.y;

        // 타겟 위치
        int targetX = x + monitorArea.x;
        int targetY = y + monitorArea.y;

        // 임계값 설정 (프레임당 최대 이동량)
        const int MAX_DELTA = 25;

        // 반복적으로 이동하여 목표 지점에 도달
        while (currentX != targetX || currentY != targetY) {
            int dx = targetX - currentX;
            int dy = targetY - currentY;

            // dx, dy를 임계값 내로 제한
            if (abs(dx) > MAX_DELTA) dx = (dx > 0) ? MAX_DELTA : -MAX_DELTA;
            if (abs(dy) > MAX_DELTA) dy = (dy > 0) ? MAX_DELTA : -MAX_DELTA;

            SetCursorPos(currentX + dx, currentY + dy);

            // 현재 위치 갱신
            currentX += dx;
            currentY += dy;

            Sleep(5);
        }
    }

    // 최종 위치에서 클릭 수행
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
