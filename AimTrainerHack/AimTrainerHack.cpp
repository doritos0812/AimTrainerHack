#include <windows.h>
#include <stdio.h>

POINT monitorArea = { 800, 500 };
int width = 900, height = 600;


void mouseClick() {
    INPUT inputs[2] = { 0 };
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, inputs, sizeof(INPUT));
    printf("Mouse clicked at the detected position.\n");
}


void captureAndDetectColor() {
    HDC hdcScreen = GetDC(NULL); // 화면의 DC 가져오기
    HDC hdcMem = CreateCompatibleDC(hdcScreen); // 메모리 DC 생성
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcMem, hBitmap);

    // 화면에서 메모리 DC로 영역 복사
    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, monitorArea.x, monitorArea.y, SRCCOPY);

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // 상단에서 하단 방향으로 이미지 저장
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24; // 24비트(RGB)
    bmi.bmiHeader.biCompression = BI_RGB;

    // 메모리 할당 후 픽셀 데이터 가져오기
    BYTE* pPixels = (BYTE*)malloc(3 * width * height);
    GetDIBits(hdcMem, hBitmap, 0, height, pPixels, &bmi, DIB_RGB_COLORS);

    // 픽셀 데이터 검사
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            BYTE blue = pPixels[index];
            BYTE green = pPixels[index + 1];
            BYTE red = pPixels[index + 2];

            if (red >= 230 && green >= 80 && green <= 90 && blue >= 30 && blue <= 40) {
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

int main() {
    while (1) {
        captureAndDetectColor();
        Sleep(100); // 주기적 탐지
    }
    return 0;
}