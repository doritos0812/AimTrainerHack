#ifndef SET_MONITOR_AREA_H
#define SET_MONITOR_AREA_H

#include <windows.h>

// 화면 영역 설정 함수
void setMonitorArea();

// 설정된 화면 영역 반환
POINT getMonitorArea();

// 설정된 영역의 너비 반환
int getMonitorWidth();

// 설정된 영역의 높이 반환
int getMonitorHeight();

#endif // SET_MONITOR_AREA_H
