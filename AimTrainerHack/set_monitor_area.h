// set_monitor_area.h
#ifndef SET_MONITOR_AREA_H
#define SET_MONITOR_AREA_H

#include <windows.h>
#include <stdbool.h>

// ���� ���� ����
extern int width, height;          // ȭ�� ������ �ʺ�� ����
extern POINT topLeft, bottomRight;  // ȭ�� ���� �»�� �� ���ϴ� ��ǥ
extern bool wasPressed;             // ��ư ���� ���� ������ ����

// ȭ�� �ػ󵵸� �������� �Լ�
void getFullScreenResolution(int* width, int* height);

// Ư�� Ű�� ������ �� ������ �����ϴ� �Լ�
bool waitForKeyPress(int key, bool* wasPressed, void (*action)());

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

#endif // SET_MONITOR_AREA_H
