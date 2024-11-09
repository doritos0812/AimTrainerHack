// set_monitor_area.h
#ifndef SET_MONITOR_AREA_H
#define SET_MONITOR_AREA_H

#include <windows.h>
#include <stdbool.h>

// 전역 변수 선언
extern int width, height;					// 화면 영역의 너비와 높이
extern POINT topLeft, bottomRight, center;  // 화면 영역 좌상단 및 우하단 및 중심 좌표
extern bool wasPressed;						// 버튼 누름 상태 추적용 변수
extern bool isFpsMode;						// FPS 모드 여부 저장 변수

// 화면 해상도를 가져오는 함수
void getFullScreenResolution(int* width, int* height);

// 화면 중심부 좌표를 설정하는 함수
void setMonitorCenter();

// 특정 키가 눌렸을 때 동작을 수행하는 함수
bool waitForKeyPress(int key, bool* wasPressed, void (*action)());

// FpsMode 여부를 사용자로 부터 입력 받는 합수
void isFpsModeCheck();

// 좌상단 및 우하단 지점을 선택하는 함수
void selectArea();

// 전체 화면 해상도를 설정하는 함수
void setFullScreenResolution();

// 화면 영역을 설정하는 메인 함수
void setMonitorArea();

// 설정된 화면 영역 반환
POINT getMonitorArea();

// 설정된 영역의 너비 반환
int getMonitorWidth();

// 설정된 영역의 높이 반환
int getMonitorHeight();

// FpsMode 여부 반환
bool getFpsMode();

// 화면 중심부 좌표 반환
POINT getMonitorCenter();

#endif // SET_MONITOR_AREA_H
