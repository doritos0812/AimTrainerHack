#include <windows.h>
#include <stdio.h>
#include "set_monitor_area.h"

static bool wasPressed = false;
POINT topLeft, bottomRight, monitorArea;
int width, height;

// ����ڰ� ������ ȭ�� ���� ���� �Լ�
void setMonitorArea() {
    printf("�����̽��ٸ� ���� �� ������ Ŭ���ϼ���.\n");
    while (!GetAsyncKeyState(VK_SPACE)) {
        Sleep(100);
    }

    printf("ù ��° Ŭ������ �»�� ������ �����ϼ���.\n");
    while (true) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            if (!wasPressed) {
                GetCursorPos(&topLeft);
                monitorArea = topLeft;
                printf("�»�� ���� Ŭ��: (%d, %d)\n", monitorArea.x, monitorArea.y);
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

    printf("�� ��° Ŭ������ ���ϴ� ������ �����ϼ���.\n");
    while (true) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            if (!wasPressed) {
                GetCursorPos(&bottomRight);
                printf("���ϴ� ���� Ŭ��: (%d, %d)\n", bottomRight.x, bottomRight.y);
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
        monitorArea.x, monitorArea.y, bottomRight.x, bottomRight.y, width, height);
    printf("����� ������ ���������� �����Ǿ����ϴ�.\n");
}

// ������ ȭ�� ���� ��ȯ
POINT getMonitorArea() {
    return monitorArea;
}

// ������ ������ �ʺ� ��ȯ
int getMonitorWidth() {
    return width;
}

// ������ ������ ���� ��ȯ
int getMonitorHeight() {
    return height;
}
