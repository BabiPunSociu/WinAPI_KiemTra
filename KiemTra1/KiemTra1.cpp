// KiemTra1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "KiemTra1.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HDC hdc;
HPEN hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_KIEMTRA1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KIEMTRA1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KIEMTRA1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_KIEMTRA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, L"Connect-the-Points Mouse Demo", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINT point[1000];
    static int index = 0;
    switch (message)
    {
    case WM_MOUSEMOVE:  // Cau 5: Di chuot
        if (wParam && MK_LBUTTON == TRUE)
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            hdc = GetDC(hWnd);
            SelectObject(hdc, hpen);
            SetPixel(hdc, x, y, RGB(0, 0, 0));

            ReleaseDC(hWnd, hdc);
        }
        break;
    case WM_LBUTTONDOWN:    // Cau 2: Ve diem tai vi tri click
        point[index].x = LOWORD(lParam);
        point[index].y = HIWORD(lParam);

        hdc = GetDC(hWnd);
        SelectObject(hdc, hpen);
        SetPixel(hdc, point[index].x, point[index].y, RGB(0, 0, 0));
        ReleaseDC(hWnd, hdc);
        index++;
        break;
    case WM_RBUTTONDOWN:    // Cau 3: xoa mien thao tac
        // xóa dữ liệu trong array point
        index = 0;  // point là mảng tĩnh nên không xóa được
        // Xóa miền thao tác đã vẽ
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_LBUTTONDBLCLK:  // Cau 4: Noi cac diem thanh duong gap khuc
        hdc = GetDC(hWnd);
        SelectObject(hdc, hpen);
        Polyline(hdc, point, index);
        ReleaseDC(hWnd, hdc);
        break;
        
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_Color:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_Mau), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_CLOSE:
        EndDialog(hDlg, TRUE);
        break;

    case WM_COMMAND:
        int id = LOWORD(wParam);
        switch (id)
        {
        case IDC_btnDo:
            hpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            EndDialog(hDlg, TRUE);
            break;
        case IDC_btnXanh:
            hpen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
            EndDialog(hDlg, TRUE);
            break;
        case IDC_btnDen:
            hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
            EndDialog(hDlg, TRUE);
            break;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
