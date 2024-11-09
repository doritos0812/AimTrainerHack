#ifndef SET_TARGET_COLOR_H
#define SET_TARGET_COLOR_H

#include <windows.h>

typedef struct {
    BYTE red;
    BYTE green;
    BYTE blue;
} RGBColor;

// 사용자 클릭을 통해 탐지할 색상을 설정하는 함수
void setTargetColor(bool isFpsMode, POINT center);

// 설정된 색상 반환
RGBColor getTargetColor();

#endif // SET_TARGET_COLOR_H
