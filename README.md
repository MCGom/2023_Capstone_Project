# 2023_Capstone_Project
2023년 캡스톤 프로젝트 진척 상황을 저장 및 종합하기 위한 레포지토리입니다.
# 프로젝트 소개
* 개발 언어 : C++
* 개발 환경 : VisualStudio2019, VisualStudio2022, Windows OS
* 개발 목표 :
  * 멀티 모니터 환경에서 원하는 화면을 캡처 가능한 프로그램
  * 사용 중인 문서 프로그램 혹은 수업용 프로그램에 상관 없이 화면을 캡처하여 필기를 진행할 수 있는 프로그램
  * 필기한 화면을 그대로 저장하여 수업 자료로 배포 가능한 프로그램
  * 일정 시간마다 쉬는 시간, 출석 체크 시간, 수업 시작 시간을 학생들에게 알릴 수 있는 프로그램
  * C++을 이용해 학업 및 수업 진행 도우미 프로그램 제작
<br/>
# 프로젝트 구성

* 메인 윈도우 구성
  <br/> 프로그램을 실행시 나오는 창입니다. 'X번 모니터 화면' 버튼은 캡처하고 싶은 모니터를 선택하는 버튼이며,
  <br/> 'X번 모니터에 필기 작성' 버튼은 캡처한 화면은 어떤 모니터에서 필기할지 선택하는 버튼입니다.
  <br/> '알람 설정' 메뉴는 알람 기능을 활성화 혹은 비활성화 할 수 있습니다.
  <br/> '도움말' 메뉴를 통해 프로그램의 다양한 기능을 설명하는 설명서를 열어볼 수 있습니다. <br/>
![CapstoneMainWindow](https://github.com/MCGom/2023_Capstone_Project/assets/100755492/8f2e8bcc-30df-4477-9216-5af75efd9960)<br/>
* 설명서 구성
  <br/> 메인 윈도우의 도움말 기능을 통해 띄워지는 설명서 화면입니다.<br/>
![CapstoneMainInfo](https://github.com/MCGom/2023_Capstone_Project/assets/100755492/e1040fa5-6d01-4d40-900b-6554038d04e0)<br/>
* 필기 화면 버튼 메뉴 구성
  <br/> 필기를 시작했을 때 사용할 수 있는 메뉴들입니다.
  <br/> 메뉴는 필기창 우하단에 생성되며, 더블 릭으로 숨기거나 보이게 할 수 있습니다.
  <br/> 각 메뉴에 대한 설명은 위의 설명서 화면과 같습니다. <br/>
![CapstoneMenubar](https://github.com/MCGom/2023_Capstone_Project/assets/100755492/540acbbc-1fa5-4eb0-b6ce-18f3e6f1b7d2)<br/>
* 각 알람 화면 구성
  <br/> 특정 시간마다 모니터 우상단에 띄워지는 알람 화면입니다.
  <br/> 위부터 쉬는 시간, 출석 체크 시간, 수업 시작 시간 알람 화면입니다.<br/>
  ![CapstoneBreaktimeAlarm](https://github.com/MCGom/2023_Capstone_Project/assets/100755492/93a8ce7c-71f5-487e-b17a-979fd4d92670)<br/>
  ![CapstoneAttendtimeAlarm](https://github.com/MCGom/2023_Capstone_Project/assets/100755492/5aebabec-8110-4e0c-ae98-88115f581c4b)<br/>
  ![CapstoneStarttimeAlarm](https://github.com/MCGom/2023_Capstone_Project/assets/100755492/f33c7bb3-7d07-46d0-a97d-a519cc8427f7)<br/>
* 출석 체크 알람 시간 설정 파일 구성
  <br/> 출석 체크 알람을 어느 시간에 띄울지 설정하는 파일입니다.
  <br/> 두 괄호 안에 수업 시작 시간을 입력하면, 입력한 시간 기준 5분 전에 출석 체크 알람을 띄워줍니다.
  <br/> 프로그램 실행시 자동으로 설정 파일을 작성해줍니다.
  <br/> 기본 시간은 10시와 14시이며, 이때 출석 체크 알람은 9시 55분과 13시 55분에 띄웁니다.<br/>
  ![CapstoneAttendtimeAlarmSetting](https://github.com/MCGom/2023_Capstone_Project/assets/100755492/333dde27-b83e-41ad-ae92-b4c806024dd6)



