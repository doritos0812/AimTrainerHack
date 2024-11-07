# AimTrainerHack

## 24.11.03

* FPS 게임의 해킹툴을 방어하고자 하는 방안을 고안하던 도중, 역으로 해킹툴을 개발해보면 그에 대한 대응 방안 또한 알 수 있지 않을까 하는 아이디어로 시작한 프로젝트
* `MonitorArea`를 시작점으로 하여 `width`, `height`만큼 확장된 영역을 감지 영역으로 지정
* 화면 전체를 캡처하여 `hdcScreen`를 추출하고 `BitBlt`를 통해 지정 영역의 RGB Pixels 값을 추출
  * 각 pixel의 RGB값을 탐색하여, 원하는 Target의 RGB값과 일치하는 Target 탐지 시, 해당 위치를 클릭하여 Shoot 하도록 하는 간단한 Hack Tool
* 현재는 cpp 파일 하나로 동작하는 간단한 동작이지만, 추후 스크린 지정, 타겟 shape 지정 등의 더 고도화된 GUI 툴을 개발할 예정

## 24.11.07

* 실행 시 마다 불특정하게 target 위치를 잘못 잡는 경우가 많다. 거의 1/2 꼴로 나타나는 이슈
* 테스트 결과 (10 out of 10) 마우스 클릭을 통해 감지할 위치를 정하는 `setMonitorArea()` 함수를 실행하면 나타나는 것으로 보임
* `getColorAtCursor()` 합수 실행은 문제 없는 것으로 보임
  * 이를 통해 `HDC hdcScreen = GetDC(NULL);` 는 괜찮지만, `GetCursorPos(&pt)`는 문제가 있는 것으로 판단
* 해당 함수 동작에 어떤 사이드 이펙트를 야기하는지 분석 예정
