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
bool wasPressed = false;
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
    printf("탐지할 색상을 설정하려면 원하는 위치를 클릭하세요...\n");

    // 마우스 눌림(press) 대기
    while (1) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {  // 마우스 왼쪽 버튼이 눌림 상태일 때
            if (!wasPressed) { // 이전에 눌리지 않았을 경우에만 실행
                POINT pt;
                GetCursorPos(&pt);
                targetColor = getColorAtCursor(pt); // 클릭 지점의 색상 설정
                printf("탐지할 색상 RGB: (%d, %d, %d)\n", targetColor.red, targetColor.green, targetColor.blue);
                wasPressed = true; // 눌린 상태로 설정
                Sleep(200); // 클릭 후 짧은 대기
                break;
            }
        }
        else {
            wasPressed = false; // 버튼에서 손을 뗀 상태로 초기화
        }
        Sleep(50); // CPU 사용량 절약
    }
}

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

bool checkForExitKey() {
    if (GetAsyncKeyState(VK_ESCAPE)) {
        return true;
    }
    return false;
}

int main() {
    setMonitorArea();

    setTargetColor();

    while (1) {
        if (checkForExitKey()) {
            printf("Exiting...\n");
            break;
        }
        captureAndDetectColor();
        Sleep(100); // 주기적 탐지
    }
    return 0;
}