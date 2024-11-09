#ifndef SET_TARGET_COLOR_H
#define SET_TARGET_COLOR_H

#include <windows.h>

typedef struct {
    BYTE red;
    BYTE green;
    BYTE blue;
} RGBColor;

// ����� Ŭ���� ���� Ž���� ������ �����ϴ� �Լ�
void setTargetColor(bool isFpsMode, POINT center);

// ������ ���� ��ȯ
RGBColor getTargetColor();

#endif // SET_TARGET_COLOR_H
