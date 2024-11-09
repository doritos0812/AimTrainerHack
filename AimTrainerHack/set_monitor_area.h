// set_monitor_area.h
#ifndef SET_MONITOR_AREA_H
#define SET_MONITOR_AREA_H

#include <windows.h>
#include <stdbool.h>

// ���� ���� ����
extern int width, height;					// ȭ�� ������ �ʺ�� ����
extern POINT topLeft, bottomRight, center;  // ȭ�� ���� �»�� �� ���ϴ� �� �߽� ��ǥ
extern bool wasPressed;						// ��ư ���� ���� ������ ����
extern bool isFpsMode;						// FPS ��� ���� ���� ����

// ȭ�� �ػ󵵸� �������� �Լ�
void getFullScreenResolution(int* width, int* height);

// ȭ�� �߽ɺ� ��ǥ�� �����ϴ� �Լ�
void setMonitorCenter();

// Ư�� Ű�� ������ �� ������ �����ϴ� �Լ�
bool waitForKeyPress(int key, bool* wasPressed, void (*action)());

// FpsMode ���θ� ����ڷ� ���� �Է� �޴� �ռ�
void isFpsModeCheck();

// �»�� �� ���ϴ� ������ �����ϴ� �Լ�
void selectArea();

// ��ü ȭ�� �ػ󵵸� �����ϴ� �Լ�
void setFullScreenResolution();

// ȭ�� ������ �����ϴ� ���� �Լ�
void setMonitorArea();

// ������ ȭ�� ���� ��ȯ
POINT getMonitorArea();

// ������ ������ �ʺ� ��ȯ
int getMonitorWidth();

// ������ ������ ���� ��ȯ
int getMonitorHeight();

// FpsMode ���� ��ȯ
bool getFpsMode();

// ȭ�� �߽ɺ� ��ǥ ��ȯ
POINT getMonitorCenter();

#endif // SET_MONITOR_AREA_H
