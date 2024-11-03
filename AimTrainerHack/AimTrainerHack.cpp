#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    BYTE red;
    BYTE green;
    BYTE blue;
} RGBColor;

POINT topLeft, bottomRight, monitorArea;
int width, height;

void mouseClick() {
    INPUT inputs[2] = { 0 };
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, inputs, sizeof(INPUT));
    printf("Mouse clicked at the detected position.\n");
}

void setMonitorArea() {
    bool wasPressed = false;
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


void captureAndDetectColor(RGBColor targetColor) {
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

            if (red >= targetColor.red && green == targetColor.green && blue == targetColor.blue) {
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

bool checkForExitKey() {
    if (GetAsyncKeyState(VK_ESCAPE)) {
        return true;
    }
    return false;
}

int main() {
    RGBColor targetColor = { 255, 87, 34 };

    setMonitorArea();

    while (1) {
        if (checkForExitKey()) {
            printf("Exiting...\n");
            break;
        }
        captureAndDetectColor(targetColor);
        Sleep(100); // 주기적 탐지
    }
    return 0;
}