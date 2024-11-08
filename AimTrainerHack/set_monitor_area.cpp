#include <windows.h>
#include <stdio.h>
#include "set_monitor_area.h"

// ���� ���� �ʱ�ȭ
int width = 0, height = 0;
POINT topLeft = { 0, 0 }, bottomRight = { 0, 0 };
bool wasPressed = false;

// ��ü ȭ�� �ػ� ��ȯ �Լ�
void getFullScreenResolution(int* width, int* height) {
    *width = GetSystemMetrics(SM_CXSCREEN);  // ȭ�� ���� �ػ�
    *height = GetSystemMetrics(SM_CYSCREEN); // ȭ�� ���� �ػ�
    printf("��üȭ�� �ػ�: %d x %d\n", *width, *height);
}

// ��ư�� ��ٸ��� ���� ���¿� ���� ������ ó���ϴ� �Լ�
bool waitForKeyPress(int key, bool* wasPressed, void (*action)()) {
    if (GetAsyncKeyState(key) & 0x8000) {  // Ű�� ������ ��
        if (!(*wasPressed)) {
            action();  // ������ �۾� ����
            *wasPressed = true;
            return true;
        }
    }
    else {
        *wasPressed = false;
    }
    return false;
}

// F2 ��ư�� ������ �»�ܰ� ���ϴ� ��ǥ�� �����ϴ� �Լ�
void selectArea() {
    printf("F2 ��ư���� �»�� ������ �����ϼ���.\n");
    while (!waitForKeyPress(VK_F2, &wasPressed, []() {
        GetCursorPos(&topLeft);
        printf("�»�� ����: (%d, %d)\n", topLeft.x, topLeft.y);
        })) {
        Sleep(100);
    }

    printf("F2 ��ư���� ���ϴ� ������ �����ϼ���.\n");
    while (!waitForKeyPress(VK_F2, &wasPressed, []() {
        GetCursorPos(&bottomRight);
        printf("���ϴ� ����: (%d, %d)\n", bottomRight.x, bottomRight.y);
        })) {
        Sleep(100);
    }

    // �»�ܰ� ���ϴ� ��ǥ�� ���� ���
    width = bottomRight.x - topLeft.x;
    height = bottomRight.y - topLeft.y;

    printf("������ ����: �»�� (%d, %d), ���ϴ� (%d, %d), �ʺ�: %d, ����: %d\n",
        topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, width, height);
}

// F ��ư�� ������ ��ü ȭ�� �ػ󵵸� �����ϴ� �Լ�
void setFullScreenResolution() {
    getFullScreenResolution(&width, &height);
}

// ȭ�� ���� ���� �Լ�
void setMonitorArea() {
    printf("F2 ��ư�� ���� ���콺�� �̿��� ������ �����ϰų�, F ��ư�� ���� ��ü ȭ�� �ػ󵵸� �����ϼ���.\n");

    // F ��ư�� ������ �� ��ü ȭ�� �ػ� ����
    while (true) {
        if (waitForKeyPress(0x46, &wasPressed, setFullScreenResolution)) {
            break;  // �ػ󵵸� ������ �� �Լ� ����
        }

        // F2 ��ư�� ������ �� ���콺 Ŭ������ ���� ����
        if (waitForKeyPress(VK_F2, &wasPressed, selectArea)) {
            break; // ���� ���� �� �Լ� ����
        }

        Sleep(100); // CPU ��뷮 ����
    }
}

// ������ ȭ�� �»�� ���� ��ȯ
POINT getMonitorArea() {
    return topLeft;
}

// ������ ������ �ʺ� ��ȯ
int getMonitorWidth() {
    return width;
}

// ������ ������ ���� ��ȯ
int getMonitorHeight() {
    return height;
}
