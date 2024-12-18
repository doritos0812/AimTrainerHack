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

## 24.11.08
* 코드 전체적으로 .cpp 파일 하나에 몰빵되어있는 것이 상당히 드럽고 눈엣가시여서 전체적인 리팩토링을 진행
* `main`, `capture_and_detect`, `set_monitor_area`, `set_target_color` 4 영역으로 분리
* 각 구현체에서 모듈화 가능한 부분은 최대한 모듈화하여 리팩토링
* 24.11.07일에 분석한 `GetCursorPos(&pt)`와는 무관한 문제로 보임...
  * 좌표값을 내장 전역변수로 저장해주면 문제가 발생하지 않는데, 사용자로부터 cin을 통한 input이든, 마우스 커서 위치던 간에 input을 받으면 계속 간헐적인 이슈가 발생
  * 원인 분석이 좀 시간이 걸릴 것 같아서 일단은 간단한 구현과 리펙토링만 먼저 진행하고 이슈로 남겨둘 예정
* TODO: 현재는 단순한 2D Aim Trainer에서만 동작이 가능함, 3D AimTrainer에서도 동작하도록 기능을 고도화하거나 기능 추가 작업 예정

## 24.11.09
* 3D AimTrainer에서도 동작 할 수 있도록 FPS mode를 추가함
* FPS 모드 특성상 cursor, 즉 crosshair가 무조건 화면 중앙에 위치하기 때문에 이 지점을 `cursor position`으로 지정
* 기존 2D Trainer와는 다르게, 특정 위치로 마우스를 '순간이동' 시키는 느낌인 `SetCursorPos이` 아닌 `mouse_event`를 통해 상대적인 위치로 '이동'을 시켜줘야 함
* Steam Application인 Aim Hero를 통해 테스트 진행
  * 다만, 소스코드상의 config 수정 뿐만 아니라, ingame 감도 혹은 타겟 색상 지정 등의 이슈가 필요함
* FPS Case에서 발견된 이슈들
  * 타겟이 너무 가까울 경우 (= 이동시켜야 하는 마우스 거리가 멀 경우) 타겟에 정확히 탄착되지 않는 이슈
    * 단순 특정 위치로 마우스 이동이 아닌, FPS 게임 내부에서 타겟 위치와 마우스 이동 간의 특수한 로직이 존재하는 것 처럼 보임
  * 총기반동이 존재할 경우 반동 만큼 탄착이 어긋나게 됨
    * Aim Hero 케이스에서는 임의로 감도를 없앨 수 있기 때문에 일단 이를 제외하고 테스트
    * 근데 여기까지 구현하는건 너무 주객전도 같음... 여기까지만 개발 후에 이 해킹 툴을 방어하기 위한 쉴드 툴을 개발할 예정 

# After AimbotShield 0.1.0...
https://github.com/doritos0812/AimbotShield/releases/tag/0.1.0

## 24.11.15
* AimbotShield가 Mouse Input의 픽셀 이동 속도를 바탕으로 핵을 감지한다는 정보를 입수
* 이에 대응하기 위해서 강제로 마우스 커서 이동거리에 제한을 두어 이동속도를 늦췄음
* 또한 추가로, 픽셀 이동간의 sleep을 줘서 좀 더 '인간적인' 마우스 이동을 재현

# After AimbotShield 0.1.1...
https://github.com/doritos0812/AimbotShield/releases/tag/0.1.1

## 24.11.18
* AimbotShield가 마우스 이동의 직선성과 일정한 거리의 움직임을 바탕으로 핵을 감지한다는 정보를 입수
* 이에 대응하기 위해서 타겟을 감지하고 이동하는 과정에서 랜덤값(-3 ~ 3)을 추가하여 직선성과 일정거리를 이동하는 것을 막음
* 이 동작을 통해 AimbotShield의 핵 감지 로직을 회피 가능

## 24.11.22
관련 내용 Notion 문서 링크 첨부
- https://www.notion.so/AimTrainerHack-Aimbot-Program-13ab2d181803802a8aebcfb2f46f85b4
