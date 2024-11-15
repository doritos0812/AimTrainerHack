#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "capture_and_detect.h"
#include "set_target_color.h"
#include "set_monitor_area.h"

// ���� ������� �����Ͽ� ���ʿ��� �ʱ�ȭ
static POINT monitorArea;
static int width;
static int height;
static bool isFpsMode;
static RGBColor targetColor;
static POINT center;

// ���� �� ���� ȣ��� �ʱ�ȭ �Լ�
void initializeCaptureAndDetect() {
    monitorArea = getMonitorArea();
    width = getMonitorWidth();
    height = getMonitorHeight();
    isFpsMode = getFpsMode();
    targetColor = getTargetColor();
    setMonitorCenter();
    center = getMonitorCenter();
}

// mouse Ŭ��
void mouseClick() {
    INPUT inputs[2] = { 0 };
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, inputs, sizeof(INPUT));
    printf("Mouse clicked at the detected position.\n");
}

// ���� �� �Լ�
bool isPixelColorCorrect(int x, int y, int width, BYTE* pPixels, RGBColor color) {
    // �������� ������ width ���� ���
    int index = (y * width + x) * 3;  // width �� ���
    BYTE blue = pPixels[index];
    BYTE green = pPixels[index + 1];
    BYTE red = pPixels[index + 2];

    // ���� ��
    return (red == color.red && green == color.green && blue == color.blue);
}

void performActionOnDetection(int x, int y) {
    printf("Detected target color at (%d, %d)\n", x + monitorArea.x, y + monitorArea.y);

    if (isFpsMode) {
        // �ʱ� ��ġ
        int currentX = center.x;
        int currentY = center.y;

        // �Ӱ谪 ���� (�����Ӵ� �ִ� �̵���)
        const int MAX_DELTA = 25;

        // �ݺ������� �̵��Ͽ� ��ǥ ������ ����
        while (currentX != x || currentY != y) {
            int dx = x - currentX;
            int dy = y - currentY;

            // dx, dy�� �Ӱ谪 ���� ����
            if (abs(dx) > MAX_DELTA) dx = (dx > 0) ? MAX_DELTA : -MAX_DELTA;
            if (abs(dy) > MAX_DELTA) dy = (dy > 0) ? MAX_DELTA : -MAX_DELTA;

            mouse_event(MOUSEEVENTF_MOVE, dx, dy, 0, 0);

            // ���� ��ġ ����
            currentX += dx;
            currentY += dy;

            Sleep(5);
        }

        printf("Reached final position: (%d, %d)\n", x, y);
    }
    else {
        POINT currentPos;
        GetCursorPos(&currentPos);

        // �ʱ� ��ġ
        int currentX = currentPos.x;
        int currentY = currentPos.y;

        // Ÿ�� ��ġ
        int targetX = x + monitorArea.x;
        int targetY = y + monitorArea.y;

        // �Ӱ谪 ���� (�����Ӵ� �ִ� �̵���)
        const int MAX_DELTA = 25;

        // �ݺ������� �̵��Ͽ� ��ǥ ������ ����
        while (currentX != targetX || currentY != targetY) {
            int dx = targetX - currentX;
            int dy = targetY - currentY;

            // dx, dy�� �Ӱ谪 ���� ����
            if (abs(dx) > MAX_DELTA) dx = (dx > 0) ? MAX_DELTA : -MAX_DELTA;
            if (abs(dy) > MAX_DELTA) dy = (dy > 0) ? MAX_DELTA : -MAX_DELTA;

            SetCursorPos(currentX + dx, currentY + dy);

            // ���� ��ġ ����
            currentX += dx;
            currentY += dy;

            Sleep(5);
        }
    }

    // ���� ��ġ���� Ŭ�� ����
    mouseClick();
}

// ȭ�� ĸ�� �� ���� ���� �Լ�
void captureAndDetectColor() {
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcMem, hBitmap);

    // ȭ�鿡�� �޸� DC�� ���� ����
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

    // �ȼ� ������ �˻�
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (!isPixelColorCorrect(x, y, width, pPixels, targetColor)) continue;

            performActionOnDetection(x, y);

            // �޸� ���� �� ����
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
