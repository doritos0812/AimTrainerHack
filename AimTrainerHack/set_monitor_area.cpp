#include <windows.h>
#include <stdio.h>
#include "set_monitor_area.h"

static bool wasPressed = false;
POINT topLeft, bottomRight;
int width, height;

// ����ڰ� ������ ȭ�� ���� ���� �Լ�
void setMonitorArea() {
    printf("ù ��° F2 ��ư���� �»�� ������ �����ϼ���.\n");
    while (true) {
        if (GetAsyncKeyState(VK_F2) & 0x8000) {
            if (!wasPressed) {
                GetCursorPos(&topLeft);
                printf("�»�� ����: (%d, %d)\n", topLeft.x, topLeft.y);
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

    printf("�� ��° F2 ��ư���� ���ϴ� ������ �����ϼ���.\n");
    while (true) {
        if (GetAsyncKeyState(VK_F2) & 0x8000) {
            if (!wasPressed) {
                GetCursorPos(&bottomRight);
                printf("���ϴ� ����: (%d, %d)\n", bottomRight.x, bottomRight.y);
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

    // �»�ܰ� ���ϴ� ��ǥ�� ���� ���
    width = bottomRight.x - topLeft.x;
    height = bottomRight.y - topLeft.y;

    printf("������ ����: �»�� (%d, %d), ���ϴ� (%d, %d), �ʺ�: %d, ����: %d\n",
        topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, width, height);
    printf("����� ������ ���������� �����Ǿ����ϴ�.\n");
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
