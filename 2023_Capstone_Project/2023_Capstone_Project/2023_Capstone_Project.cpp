// 2023_Capstone_Project.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "2023_Capstone_Project.h"
#include <wingdi.h>
#include <commdlg.h>
#include <atlImage.h> 
#include <time.h>
#include <thread>
#include <CommCtrl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

#include <vector>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst, g_inst, t_inst, b_inst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
WCHAR ChildWindowClass[MAX_LOADSTRING];
WCHAR TimerWindowClass[MAX_LOADSTRING];

//메인 윈도우가 종료 되었다면 알람 스레드를 종료시키기 위한 플래그 변수
int is_Exit = 0;
//캡처 화면의 버튼 메뉴가 열려있는지 확인하기 위한 플래그 변수
int btn_Opened = 0;
//알람 기능이 활성화 되어 있는지 확인하기 위한 플래그 변수
int is_Activated = 1;
//알람창의 텍스트가 업데이트 되었는지 확인하기 위한 플래그 변수
int is_Updated = 0;

//캡쳐 출력 화면의 HWND
HWND captured_Wnd;
//화면 캡쳐에 사용한 HDC
HDC hMemDC, hScrDC, hCapDC;
//화면 캡쳐에 사용할 bitmap 선언
BITMAP bmp;
//화면 캡쳐에 사용할 hbitmap 선언
HBITMAP hBitmap;

//그림판 기능에 사용한 HDC 및 hbitmap
HDC hDrawDC;
HBITMAP hDrawBM;

//알림 기능을 위한 HWND
HWND timer_Wnd;

//버튼 생성을 위한 HWND
HWND button_Wnd;

//메인 창의 핸들을 저장할 HWND
HWND g_hWnd;
//모니터 갯수에 따른 버튼을 생성할 때 사용하는 벡터
vector<HWND> g_button(4);
vector<HWND> c_button(4);
//모니터 갯수에 따른 버튼의 이미지를 지정하기 위한 백터
vector<HBITMAP> g_button_bitmap(4);
vector<HBITMAP> c_button_bitmap(4);
//모니터 선택 이미지의 아이디 값을 저장하는배열
int monitor_button_imgs[] = { IDB_MONITOR_1, IDB_MONITOR_2, IDB_MONITOR_3 };
//캡처 화면 표시 모니터 선택 이미지의 아이디 값을 저장하는 배열
int c_monitor_button_imgs[] = { IDB_C_MONITOR_1, IDB_C_MONITOR_2, IDB_C_MONITOR_3 };
//모니터 정보를 저장하는 벡터
vector<MONITORINFO> g_mi;
//모니터 갯수를 저장하는 정수
int g_monitor_count;
//현재 그리기 종류 구분
int g_line_kind = 0;
//현재 그리기 굵기 구분
int g_line_size = 5;
//모니터 번호 지정
int g_monitor_num;
int c_x, c_y;
//현재 그리기 색 구분
COLORREF g_line_color = RGB(0, 0, 0, 1);
//마우스 이벤트에서 그릴 때 사용할 HPEN
HPEN os_pen, my_pen;
//WM_PAINT에서 그릴때 사용할 HPEN
HPEN paint_pen, paint_os_pen;
//마우스 이벤트에서 그릴 때 사용할 HBRUSH
HBRUSH os_b, my_b;
//WM_PAINT에서 그릴 때 사용할 HBRUSH
HBRUSH paint_b, paint_os_b;
//임시 그림용 펜
HPEN tmp_pen, o_pen;
//임시 그림용 브러쉬
HBRUSH tmp_b, o_b;
//캡쳐 화면의 dc를 저장할 HDC
HDC g_hdc;
//색상 변경을 위한 버튼 핸들
HWND color_btn;
//색상 변경 다이얼로그
CHOOSECOLOR cc;
//커스텀 색 변경을 위한 배열
COLORREF crTemp[16];
//알람창에 띄울 문자열
LPCWSTR alarm_Text;


//마우스가 눌린 상태인지 확인하기 위한 변수
bool g_p = false;
//좌표 값 저장을 위한 정수
int g_x, g_y;
//지우개 구현에 사용할 임시 이미지
CImage erase_image;
//색상 팔레트 버튼 생성용
HBITMAP palette_Bitmap;
HWND palette_Button;
//굵기 조절 버튼 생성용
HBITMAP size_Bitmap;
HWND size_Button;
//자유 곡선 선택 버튼 생성용
HBITMAP free_Bitmap;
HWND free_Button;
//직선 선택 버튼 생성용
HBITMAP line_Bitmap;
HWND line_Button;
//사각형 선택 버튼 생성용
HBITMAP rect_Bitmap;
HWND rect_Button;
//원형 선택 버튼 생성용
HBITMAP elli_Bitmap;
HWND elli_Button;
//지우개 선택 버튼 생성용
HBITMAP erase_Bitmap;
HWND erase_Button;
//나가기 버튼 생성용
HBITMAP exit_Bitmap;
HWND exit_Button;
//저장 버튼 생성용
HBITMAP save_Bitmap;
HWND save_Button;
RECT hCapWnd_Rect;


//펜 굵기를 받아오기 위한 정수
int pos = 0;
//슬라이더 핸들을 받기 위한 변수
HWND hSlider;
//펜 굵기 설정 다이얼로그에 관한 코드

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
ATOM                TimerRegisterClass(HINSTANCE hInstance);
ATOM                ChildRegisterClass(HINSTANCE hInstance);
ATOM                ButtonRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    TimerWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ButtonProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    LINE_SIZE(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    COLORDLG(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    INFODLG(HWND, UINT, WPARAM, LPARAM);

//선택한 모니터 번호에 따른 캡처 화면 생성하는 함수
void screen_change(int);
//캡처한 화면에 작성한 필기를 저장하는 함수
void screen_save(HWND);
//일정 시간마다 알람을 띄우는 함수
void timer_alarm(int, int);
//모니터의 갯수 및 번호를 받아오는 함수
static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdc, LPRECT lpRect, LPARAM lParam);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    //수업 시작 알림에 사용하는 코드
    //수업 시작 알림 설정 파일을 받아오는 문자열
    string line = "";
    //수업 시작 알림 설정 파일의 문자열을 합치는 문자열
    string setting = "";
    //설정 파일 이름
    std::string filename = "setting.txt";
    //설정 파일 읽어옴
std:ifstream file_in(filename);
    //수업 시작 저장용 정수
    int firstNumber;
    int secondNumber;
    //설정 파일이 존재하는지에 따라 수행
    if (file_in.good()) {
        // 파일이 존재할 경우 읽기
        std::ifstream infile(filename);
        //파일에 저장된 문자열들 불러옴
        while (std::getline(infile, line)) {
            setting = setting + line;
        }
        //파일 닫기
        infile.close();
    }
    else {
        //파일이 존재하지 않을 경우 생성하고 쓰기

        std::ofstream outfile(filename);

        if (outfile.is_open()) {
            outfile << "수업 시작 시간\n";
            outfile << ": (10) (14)\n";
            outfile << "괄호 안에 수업 시작 시간을 넣어주세요\n첫 번째 괄호는 오전 수업\n두 번째 괄호는 오후 수업 시작 시간입니다.\n괄호 밖은 수정하지 말아주세요";
            outfile.close();
            setting = "수업 시작 시간\n: (10) (14)";
        }
        else {
            std::cerr << "Unable to create file.\n";
        }
    }
    //수업 시작 시간을 받아올 패턴
    std::regex pattern("\\((\\d+)\\) \\((\\d+)\\)");

    //패턴에 따라 시간을 받아옴
    std::smatch match;
    if (std::regex_search(setting, match, pattern)) {

        firstNumber = std::stoi(match[1].str());
        secondNumber = std::stoi(match[2].str());

    }
    else {
        std::cerr << "패턴을 찾을 수 없음\n";
    }
    //알림 스레드 활성화
    thread alarm_Timer(timer_alarm, firstNumber - 1, secondNumber - 1);

    //알람창의 설정
    TimerRegisterClass(t_inst);
    //알람창 생성
    timer_Wnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_OVERLAPPEDWINDOW, L"Alarm", L"Alarm", WS_POPUP, 1000, 30, 875, 100, NULL, NULL, t_inst, NULL);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2023CAPSTONEPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2023CAPSTONEPROJECT));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    //알람 쓰레드 종료
    alarm_Timer.join();
    file_in.close();
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//

//메인 창 클래스 등록
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2023CAPSTONEPROJECT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY2023CAPSTONEPROJECT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//캡쳐 화면을 보여주기 위한 창의 클래스 등록
ATOM ChildRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW childwcex;

    childwcex.cbSize = sizeof(WNDCLASSEX);

    //더블 클릭 지원
    childwcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    childwcex.lpfnWndProc = ChildWndProc;
    childwcex.cbClsExtra = 0;
    childwcex.cbWndExtra = 0;
    childwcex.hInstance = hInstance;
    childwcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2023CAPSTONEPROJECT));
    childwcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
    childwcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    childwcex.lpszMenuName = NULL;
    childwcex.lpszClassName = L"Capture";
    childwcex.hIconSm = LoadIcon(childwcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&childwcex);
}
ATOM ButtonRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW childwcex;

    childwcex.cbSize = sizeof(WNDCLASSEX);

    childwcex.style = CS_HREDRAW | CS_VREDRAW;
    childwcex.lpfnWndProc = ButtonProc;
    childwcex.cbClsExtra = 0;
    childwcex.cbWndExtra = 0;
    childwcex.hInstance = hInstance;
    childwcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2023CAPSTONEPROJECT));
    childwcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    childwcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    childwcex.lpszMenuName = NULL;
    childwcex.lpszClassName = L"Buttons";
    childwcex.hIconSm = LoadIcon(childwcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&childwcex);
}

//알람 창 클래스 등록
ATOM TimerRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW timerwcex;

    timerwcex.cbSize = sizeof(WNDCLASSEX);

    //더블 클릭 지원
    timerwcex.style = CS_HREDRAW | CS_VREDRAW;
    timerwcex.lpfnWndProc = TimerWndProc;
    timerwcex.cbClsExtra = 0;
    timerwcex.cbWndExtra = 0;
    timerwcex.hInstance = hInstance;
    timerwcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2023CAPSTONEPROJECT));
    timerwcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    timerwcex.hbrBackground = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    timerwcex.lpszMenuName = NULL;
    timerwcex.lpszClassName = L"Alarm";
    timerwcex.hIconSm = LoadIcon(timerwcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&timerwcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU | WS_BORDER,
       CW_USEDEFAULT, CW_USEDEFAULT, 570, 220, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
                //캡쳐를 원하는 모니터에 대응하는 버튼을 눌렀을 때 실행
            case 0x1001:
            {
                //1번 모니터
                g_monitor_num = 0;
                screen_change(g_monitor_num);

            }
            break;
            case 0x1001 + 1:
            {
                //2번 모니터
                g_monitor_num = 1;
                screen_change(g_monitor_num);
            }
            break;
            case 0x1001 + 2:
            {
                //3번 모니터
                g_monitor_num = 2;
                screen_change(g_monitor_num);
            }
            break;
            //캡처 화면을 띄울 모니터를 선택했을시 실행
            case 0x1011:
            {
                //1번 모니터
                g_monitor_num = 0;
                SetWindowPos(captured_Wnd, NULL, g_mi[g_monitor_num].rcMonitor.left, g_mi[g_monitor_num].rcMonitor.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                SetWindowPos(button_Wnd, NULL, g_mi[g_monitor_num].rcMonitor.left + 10, g_mi[g_monitor_num].rcMonitor.top + 1000, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            }
            break;
            case 0x1012:
            {
                //1번 모니터
                g_monitor_num = 1;
                SetWindowPos(captured_Wnd, NULL, g_mi[g_monitor_num].rcMonitor.left, g_mi[g_monitor_num].rcMonitor.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                SetWindowPos(button_Wnd, NULL, g_mi[g_monitor_num].rcMonitor.left + 10, g_mi[g_monitor_num].rcMonitor.top + 1000, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            }
            break;
            case 0x1013:
            {
                //1번 모니터
                g_monitor_num = 2;
                SetWindowPos(captured_Wnd, NULL, g_mi[g_monitor_num].rcMonitor.left, g_mi[g_monitor_num].rcMonitor.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                SetWindowPos(button_Wnd, NULL, g_mi[g_monitor_num].rcMonitor.left + 10, g_mi[g_monitor_num].rcMonitor.top + 1000, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            }
            break;
            //알람창 표시 여부 선택시 실행
            case ID_ALARM_ON:
            {
                //알람창 활성화
                is_Activated = 1;
            }
            break;
            case ID_ALARM_OFF:
            {
                //알람창 비활성화
                is_Activated = 0;
                ShowWindow(timer_Wnd, SW_HIDE);
            }
            break;
            case ID_INFO:
            {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_INTRO), hWnd, INFODLG);
            }
            break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
    {
        //모니터 구분을 위한 정수
        int i = 0;
        g_hWnd = hWnd;

        //모니터의 갯수만큼 MobitorEnumProc을 반복하여 모니터 정보를 얻음
        EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, NULL);
        //모니터 갯수만큼 반복
        for (i = 0; i < g_monitor_count; i++)
        {
            //모니터 캡쳐를 위한 버튼 생성
            g_button[i] = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | BS_DEFPUSHBUTTON | BS_BITMAP, ((i * 160) + 15), 10, 160, 60, hWnd, (HMENU)(0x1001 + i), hInst, NULL);
            g_button_bitmap[i] = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(monitor_button_imgs[i]));
            SendMessage(g_button[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)g_button_bitmap[i]);
            ShowWindow(g_button[i], SW_SHOW);
        }
        //모니터 갯수만큼 반복
        for (i = 0; i < g_monitor_count; i++)
        {
            //모니터 캡쳐 화면을 표시할 모니터 선택 버튼 생성
            c_button[i] = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | BS_DEFPUSHBUTTON | BS_BITMAP, ((i * 160) + 15), 85, 160, 60, hWnd, (HMENU)(0x1011 + i), hInst, NULL);
            c_button_bitmap[i] = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(c_monitor_button_imgs[i]));
            SendMessage(c_button[i], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)c_button_bitmap[i]);
            ShowWindow(c_button[i], SW_SHOW);
        }
        //모니터의 해상도를 얻음
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);
        ChildRegisterClass(g_inst);
        ButtonRegisterClass(b_inst);

        //캡쳐 화면 출력을 위한 창 생성
        captured_Wnd = CreateWindowEx(0, L"Capture", L"Capture", WS_POPUP, g_mi[0].rcMonitor.left, g_mi[0].rcMonitor.top, g_mi[0].rcMonitor.right, g_mi[0].rcMonitor.bottom, NULL, NULL, g_inst, NULL);
        //캡처 화면 버튼을 위한 창 생성
        button_Wnd = CreateWindowEx(WS_EX_TOPMOST, L"Buttons", L"Buttons", WS_POPUP, g_mi[0].rcMonitor.left + 10, g_mi[0].rcMonitor.top + 1000, g_mi[0].rcMonitor.left + 365, g_mi[0].rcMonitor.top + 40, NULL, NULL, b_inst, NULL);


        //캡쳐를 위한 설정
        hMemDC = CreateCompatibleDC(GetDC(captured_Wnd));
        hCapDC = GetDC(captured_Wnd);


    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        //캡쳐에 사용한 HDC 정리
        DeleteDC(hMemDC);
        DeleteDC(hCapDC);
        //알람 스레드에 메인 윈도우가 종료되었다고 알림
        is_Exit = 1;

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

        break;
    case WM_CREATE:
    {
        //그리기를 위한 설정

        GetClientRect(hWnd, &hCapWnd_Rect);

        c_x = hCapWnd_Rect.right - hCapWnd_Rect.left;
        c_y = hCapWnd_Rect.bottom - hCapWnd_Rect.top;

        hDrawDC = CreateCompatibleDC(GetDC(hWnd));
        my_b = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
        os_b = (HBRUSH)SelectObject(hDrawDC, my_b);
        hDrawBM = CreateCompatibleBitmap(GetDC(hWnd), c_x, c_y);
        (HBITMAP)SelectObject(hDrawDC, hDrawBM);
        FillRect(hDrawDC, &hCapWnd_Rect, CreateSolidBrush(0x00030502));
        TransparentBlt(GetDC(hWnd), 0, 0, c_x, c_y, hDrawDC, 0, 0, c_x, c_y, 0x00030502);


    }
    break;
    case WM_ACTIVATE:
    {
        //화면의 그림이 alt+tab시 지워지는 현상을 막기 위한 다시 그리기
        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
    case WM_LBUTTONDOWN:
    {
        //마우스를 눌린 상태로 변경
        g_p = TRUE;
        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
        my_pen = CreatePen(PS_SOLID, g_line_size, g_line_color);
        os_pen = (HPEN)SelectObject(hDrawDC, my_pen);

    }
    break;
    case WM_MOUSEMOVE:
    {
        //마우스가 눌린 상태인지 확인
        if (TRUE == g_p)
        {
            //자유 곡선 그리기
            if (0 == g_line_kind) {
                int n_x;
                int n_y;
                n_x = LOWORD(lParam);
                n_y = HIWORD(lParam);
                //자유 곡선을 그림
                MoveToEx(hDrawDC, g_x, g_y, NULL);
                LineTo(hDrawDC, n_x, n_y);
                g_x = n_x;
                g_y = n_y;
            }
            //지우개 구현
            else if (4 == g_line_kind)
            {
                int n_x;
                int n_y;
                n_x = LOWORD(lParam);
                n_y = HIWORD(lParam);
                BitBlt(hDrawDC, g_x - 25, g_y - 25, 50, 50, erase_image.GetDC(), g_x - 25, g_y - 25, SRCCOPY);
                erase_image.ReleaseDC();
                g_x = n_x;
                g_y = n_y;
            }


            TransparentBlt(GetDC(hWnd), 0, 0, c_x, c_y, hDrawDC, 0, 0, c_x, c_y, 0x00030502);
        }
        break;
    }
    case WM_LBUTTONUP:
    {
        int x, y;

        x = LOWORD(lParam);
        y = HIWORD(lParam);
        //마우스가 눌린 상태가 아니었다면 break
        if (FALSE == g_p)
        {
            break;
        }
        //마우스를 뗀 상태로 변경
        g_p = FALSE;
        //그리기 유형에 맞는 그리기 작업 실행
        if (1 == g_line_kind)
        {
            //직선
            MoveToEx(hDrawDC, g_x, g_y, NULL);
            LineTo(hDrawDC, x, y);
        }
        if (2 == g_line_kind)
        {
            //사각형
            Rectangle(hDrawDC, g_x, g_y, x, y);
        }
        if (3 == g_line_kind)
        {
            //타원형
            Ellipse(hDrawDC, g_x, g_y, x, y);
        }
        TransparentBlt(GetDC(hWnd), 0, 0, c_x, c_y, hDrawDC, 0, 0, c_x, c_y, 0x00030502);
        SelectObject(hDrawDC, os_pen);
        DeleteObject(my_pen);
    }
    break;
    case WM_LBUTTONDBLCLK:
    {
        if (0 == btn_Opened)
        {
            ShowWindow(button_Wnd, SW_SHOW);
            btn_Opened = 1;
        }
        else
        {

            ShowWindow(button_Wnd, SW_HIDE);
            btn_Opened = 0;
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hWnd, &ps);

        TransparentBlt(GetDC(hWnd), 0, 0, c_x, c_y, hDrawDC, 0, 0, c_x, c_y, 0x00030502);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
    {
        //사용한 DC 및 오브젝트 해제
        ReleaseDC(captured_Wnd, g_hdc);
        SelectObject(hDrawDC, os_b);
        DeleteObject(my_b);
        DeleteDC(hDrawDC);
        DeleteObject(hDrawBM);
        //버튼 해제
        DeleteObject(palette_Bitmap);
        DeleteObject(free_Bitmap);
        DeleteObject(erase_Bitmap);
        DeleteObject(rect_Bitmap);
        DeleteObject(elli_Bitmap);
        DeleteObject(exit_Bitmap);
        DeleteObject(save_Bitmap);
        DeleteObject(size_Bitmap);
        DeleteObject(line_Bitmap);
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
            //색상 변경을 위한 다이얼로그 생성
        case IDB_PALETTE:
        {

            //다이얼로그 생성
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_COLORDLG), captured_Wnd, COLORDLG);
            ShowWindow(button_Wnd, SW_HIDE);
            btn_Opened = 0;

        }
        break;
        //그리기 종류 변경에 따라 실행
        case IDB_FREE:
        {
            //자유 곡선
            g_line_kind = 0;
            ShowWindow(button_Wnd, SW_HIDE);
            btn_Opened = 0;
        }
        break;
        case IDB_LINE:
        {
            //직선
            g_line_kind = 1;
            ShowWindow(button_Wnd, SW_HIDE);
            btn_Opened = 0;
        }
        break;
        case IDB_RECT:
        {
            //사각형
            g_line_kind = 2;
            ShowWindow(button_Wnd, SW_HIDE);
            btn_Opened = 0;
        }
        break;
        case IDB_ELLI:
        {
            //타원형
            g_line_kind = 3;
            ShowWindow(button_Wnd, SW_HIDE);
            btn_Opened = 0;
        }
        break;
        case IDB_ERASE:
        {
            g_line_kind = 4;
            ShowWindow(button_Wnd, SW_HIDE);
            btn_Opened = 0;
        }
        break;
        //펜 굵기 변경을 위한 다이얼로그 생성
        case IDB_SIZE:
        {
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SIZE_SLIDER), captured_Wnd, LINE_SIZE);
            ShowWindow(button_Wnd, SW_HIDE);
            btn_Opened = 0;
        }
        break;
        //필기한 이미지를 저장
        case IDB_SAVE:
        {
            screen_save(captured_Wnd);
            ShowWindow(button_Wnd, SW_HIDE);
            btn_Opened = 0;
        }
        break;
        //캡쳐 화면 종료
        case IDB_EXIT:
        {
            GetClientRect(captured_Wnd, &hCapWnd_Rect);
            //기존 캡쳐 화면에서 그린 정보를 제거
            FillRect(hDrawDC, &hCapWnd_Rect, CreateSolidBrush(0x00030502));
            MSG msg;
            while (PeekMessageW(&msg, captured_Wnd, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            //캡쳐 화면을 숨기고 메인 화면을 보여줌
            ShowWindow(captured_Wnd, SW_HIDE);
            ShowWindow(button_Wnd, SW_HIDE);
            ShowWindow(g_hWnd, SW_SHOW);
            erase_image.Destroy();
            remove("tmp.jpeg");
            btn_Opened = 0;
        }
        break;
        }

    }
    case WM_KEYDOWN:
    {
        switch (wParam)
        {

        }
    }
    break;
    case WM_CREATE:
    {

        //색상 선택 팔레트 버튼
        palette_Button = CreateWindowEx(WS_EX_TOPMOST, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_DEFPUSHBUTTON, 0, 0, 30, 30, hWnd, (HMENU)IDB_PALETTE, GetModuleHandle(NULL), NULL);
        palette_Bitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PALETTE));
        SendMessage(palette_Button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)palette_Bitmap);

        //굵기 조절 버튼
        size_Button = CreateWindowEx(WS_EX_TOPMOST, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_DEFPUSHBUTTON, 40, 0, 30, 30, hWnd, (HMENU)IDB_SIZE, GetModuleHandle(NULL), NULL);
        size_Bitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SIZE));
        SendMessage(size_Button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)size_Bitmap);

        //자유 곡선 버튼
        free_Button = CreateWindowEx(WS_EX_TOPMOST, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_DEFPUSHBUTTON, 80, 0, 30, 30, hWnd, (HMENU)IDB_FREE, GetModuleHandle(NULL), NULL);
        free_Bitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_FREE));
        SendMessage(free_Button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)free_Bitmap);

        //직선 버튼
        line_Button = CreateWindowEx(WS_EX_TOPMOST, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_DEFPUSHBUTTON, 120, 0, 30, 30, hWnd, (HMENU)IDB_LINE, GetModuleHandle(NULL), NULL);
        line_Bitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LINE));
        SendMessage(line_Button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)line_Bitmap);

        //사각형 버튼
        rect_Button = CreateWindowEx(WS_EX_TOPMOST, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_DEFPUSHBUTTON, 160, 0, 30, 30, hWnd, (HMENU)IDB_RECT, GetModuleHandle(NULL), NULL);
        rect_Bitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_RECT));
        SendMessage(rect_Button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)rect_Bitmap);

        //원형 버튼
        elli_Button = CreateWindowEx(WS_EX_TOPMOST, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_DEFPUSHBUTTON, 200, 0, 30, 30, hWnd, (HMENU)IDB_ELLI, GetModuleHandle(NULL), NULL);
        elli_Bitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_ELLI));
        SendMessage(elli_Button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)elli_Bitmap);

        //지우개 버튼
        erase_Button = CreateWindowEx(WS_EX_TOPMOST, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_DEFPUSHBUTTON, 240, 0, 30, 30, hWnd, (HMENU)IDB_ERASE, GetModuleHandle(NULL), NULL);
        erase_Bitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_ERASE));
        SendMessage(erase_Button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)erase_Bitmap);

        //저장 버튼
        save_Button = CreateWindowEx(WS_EX_TOPMOST, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_DEFPUSHBUTTON, 280, 0, 30, 30, hWnd, (HMENU)IDB_SAVE, GetModuleHandle(NULL), NULL);
        save_Bitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SAVE));
        SendMessage(save_Button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)save_Bitmap);

        //나가기 버튼
        exit_Button = CreateWindowEx(WS_EX_TOPMOST, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_DEFPUSHBUTTON, 320, 0, 30, 30, hWnd, (HMENU)IDB_EXIT, GetModuleHandle(NULL), NULL);
        exit_Bitmap = LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_EXIT));
        SendMessage(exit_Button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)exit_Bitmap);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hWnd, &ps);


        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


//알람 창 메시지 처리
LRESULT CALLBACK TimerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
    {
        switch (wParam)
        {

        }
    }
    break;
    case WM_CREATE:
    {
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hWnd, &ps);
        //폰트를 생성
        HFONT hFont = CreateFontW(

            100, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, JOHAB_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Sans");

        // 폰트를 선택
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        HBRUSH b_Brush = CreateSolidBrush(RGB(3, 3, 3)); // 배경색 브러시 생성
        HBRUSH b_OldBrush = (HBRUSH)SelectObject(hdc, b_Brush);
        HPEN b_Pen = CreatePen(PS_SOLID, 1, RGB(3, 3, 3));
        HPEN b_OldPen = (HPEN)SelectObject(hdc, b_Pen);

        SetTextColor(hdc, RGB(150, 0, 0));
        SetBkColor(hdc, RGB(3, 3, 3));


        RECT rect = { 0, 0, 875, 100 }; // 텍스트를 출력할 사각형의 좌표

        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom); // 사각형으로 덮어씌우기


        DrawText(hdc, alarm_Text, wcslen(alarm_Text), &rect, DT_LEFT | DT_NOCLIP);
        // 이전 폰트로 복원
        SelectObject(hdc, hOldFont);

        // 폰트 삭제
        DeleteObject(hFont);

        SelectObject(hdc, b_OldBrush);

        DeleteObject(b_Brush);

        SelectObject(hdc, b_OldPen);

        DeleteObject(b_Pen);


        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//모니터 정보를 얻는 함수
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdc, LPRECT lpRect, LPARAM lParam)
{
    //모니터 정보를 얻기 위한 변수
    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    //모니터 정보를 mi에 저장
    GetMonitorInfo(hMonitor, &mi);
    //mi의 정보를 g_mi에 저장
    g_mi.push_back(mi);
    //모니터 갯수를 증가
    g_monitor_count++;
    return TRUE;

};

//펜 굵기 설정 다이얼로그에 관한 코드
INT_PTR CALLBACK LINE_SIZE(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        hSlider = GetDlgItem(hDlg, IDC_SIZE_SLIDER);
        if (hSlider == NULL)
        {
            return (INT_PTR)true;
        }
        SendMessage(hSlider, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));
        SendMessage(hSlider, TBM_SETPOS, TRUE, g_line_size);
        SetDlgItemInt(hDlg, IDC_SIZE_LABEL, g_line_size, TRUE);
        return (INT_PTR)TRUE;
    }
    case WM_HSCROLL:
    {
        pos = SendDlgItemMessageW(hDlg, IDC_SIZE_SLIDER, TBM_GETPOS, 0, 0);
        SetDlgItemInt(hDlg, IDC_SIZE_LABEL, pos, TRUE);
    }
    break;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDOK)
        {
            g_line_size = pos;
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//색상 변경 다이얼로그에 관한 코드
INT_PTR CALLBACK COLORDLG(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        memset(&cc, 0, sizeof(CHOOSECOLOR));
        cc.lStructSize = sizeof(CHOOSECOLOR);
        cc.hwndOwner = hDlg;
        cc.lpCustColors = crTemp;
        cc.Flags = 0;
        //색상 변경시 선 색을 변경
        if (ChooseColorW(&cc) == TRUE) {
            g_line_color = cc.rgbResult;
            if (4 == g_line_kind)
            {
                g_line_kind = 0;
            }
            EndDialog(hDlg, LOWORD(wParam));
        }
        else
        {
            EndDialog(hDlg, LOWORD(wParam));

        }
        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        break;
    }
    return (INT_PTR)FALSE;
}
INT_PTR CALLBACK INFODLG(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {

    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_CLOSE:
        EndDialog(hDlg, 0);
        return (INT_PTR)TRUE;

    }
    return (INT_PTR)FALSE;
}


//모니터 캡쳐 버튼을 눌렀을 때 실행
//idx를 통해 넘어온 정수값으로 모니터 구분
void screen_change(int idx) {
    //지우개용 이미지를 불러온 적이 있다면 초기화
    if (erase_image != 0) {
        erase_image.Detach();
    }
    //메인 윈도우를 숨김
    ShowWindow(g_hWnd, SW_HIDE);
    Sleep(500);
    //출력을 위한 화면 보임
    ShowWindow(captured_Wnd, SW_SHOW);
    //지우개용 이미지
    CImage tmp_image;
    //화면 캡쳐 수행
    hScrDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
    BitBlt(hCapDC, 0, 0, 1920, 1080, hScrDC, g_mi[idx].rcMonitor.left, g_mi[idx].rcMonitor.top, SRCCOPY);

    //지우개용 이미지 설정
    int cx = 1920;
    int cy = 1080;
    int c_Color = ::GetDeviceCaps(hCapDC, BITSPIXEL);
    tmp_image.Create(cx, cy, c_Color, 0);

    ::BitBlt(tmp_image.GetDC(), 0, 0, cx, cy, hCapDC, 0, 0, SRCCOPY);

    //지우개용 이미지 저장
    tmp_image.Save(L"tmp.jpeg", Gdiplus::ImageFormatJPEG);

    hBitmap = CreateCompatibleBitmap(hMemDC, 1920, 1080);
    SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, 1920, 1080, hCapDC, 0, 0, SRCCOPY);
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    ShowWindow(g_hWnd, SW_SHOW);

    tmp_image.ReleaseDC();
    //지우개용 이미지 로드
    erase_image.Load(L"tmp.jpeg");
};


//필기한 이미지 저장
void screen_save(HWND hWnd)
{
    HDC c_Hdc = GetDC(hWnd);
    CImage image_save;
    RECT hWnd_Rect;
    GetClientRect(hWnd, &hWnd_Rect);

    time_t  t;
    time(&t);
    struct tm now;
    localtime_s(&now, &t);

    char buffer[80];
    strftime(buffer, 80, "%Y_%m_%d %H_%M_%S.jpeg", &now);

    int bufferSize = MultiByteToWideChar(CP_ACP, 0, buffer, -1, nullptr, 0);
    wchar_t* saved_Date_Time = new wchar_t[bufferSize];
    MultiByteToWideChar(CP_ACP, 0, buffer, -1, saved_Date_Time, bufferSize);
    LPCTSTR saved_Date_Time_Name = saved_Date_Time;


    int cx = hWnd_Rect.right - hWnd_Rect.left;
    int cy = hWnd_Rect.bottom - hWnd_Rect.top;
    int c_Color = ::GetDeviceCaps(c_Hdc, BITSPIXEL);
    image_save.Create(cx, cy, c_Color, 0);
    ::BitBlt(image_save.GetDC(), 0, 0, cx, cy, c_Hdc, 0, 0, SRCCOPY);
    image_save.Save(saved_Date_Time_Name, Gdiplus::ImageFormatJPEG);

    ReleaseDC(NULL, c_Hdc);
    image_save.ReleaseDC();

};



//시간마다 알림
void timer_alarm(int first, int second)
{
    while (true)
    {
        //알람 기능이 활성화된 상태에만 실행
        if (1 == is_Activated)
        {
            time_t alarm;
            struct tm alarm_Time;
            time(&alarm);
            localtime_s(&alarm_Time, &alarm);

            if (alarm_Time.tm_hour >= 0 && alarm_Time.tm_hour <= 23)
            {
                if (alarm_Time.tm_min >= 55 && alarm_Time.tm_min < 59 && (alarm_Time.tm_hour == first || alarm_Time.tm_hour == second))
                {
                    if (0 == is_Updated)
                    {
                        alarm_Text = L"!!!전자 출결 시간입니다!!!";
                        InvalidateRect(timer_Wnd, NULL, TRUE);
                        ShowWindow(timer_Wnd, SW_SHOW);
                        is_Updated = 1;
                    }

                }
                else if (alarm_Time.tm_min == 59)
                {
                    if (1 == is_Updated)
                    {
                        is_Updated = 0;
                    }
                }
                else if (alarm_Time.tm_min == 0)
                {
                    if (0 == is_Updated)
                    {
                        alarm_Text = L"!!!수업 시작 시간입니다!!!";
                        InvalidateRect(timer_Wnd, NULL, TRUE);
                        ShowWindow(timer_Wnd, SW_SHOW);
                        is_Updated = 1;
                    }
                }
                else if (alarm_Time.tm_min >= 50 && alarm_Time.tm_min < 54)
                {
                    if (0 == is_Updated)
                    {
                        alarm_Text = L"!!!쉬는 시간입니다!!!";
                        ShowWindow(timer_Wnd, SW_SHOW);
                        is_Updated = 1;
                    }
                }
                else if (alarm_Time.tm_min == 54)
                {
                    if (1 == is_Updated)
                    {
                        is_Updated = 0;
                    }
                }
                else if (alarm_Time.tm_min == 1)
                {
                    if (1 == is_Updated)
                    {
                        is_Updated = 0;
                    }
                    ShowWindow(timer_Wnd, SW_HIDE);
                }

            }
        }
        //메인 창이 종료되었다면 스레드 종료
        if (is_Exit == 1)
        {
            DestroyWindow(timer_Wnd);
            break;
        }
    }

    return;
};
