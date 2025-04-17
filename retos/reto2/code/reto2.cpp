#include "framework.h"
#include "reto2.h"
#include <string>

#define MAX_LOADSTRING 100

// Global Variables
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

// UI Elements
HWND hBtnNormal, hBtnPro, hBtnBuy, hEditLicense, hBtnActivate;

// Forward declarations
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
bool                validateLicense(std::wstring serial);

// Función de validación de licencia
bool validateLicense(std::wstring serial) {
    if (serial.rfind(L"PRO-", 0) != 0) return false;
    int sum = 0;
    for (size_t i = 4; i < serial.length(); ++i) {
        sum += serial[i];
    }
    return (sum % 7 == 0);
}

// Punto de entrada
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RETO2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow)) return FALSE;

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

// Registrar clase de ventana
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RETO2));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(20, 20, 20)); // Fondo oscuro tipo "gaming"
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.lpszMenuName = nullptr; // Sin menú

    return RegisterClassExW(&wcex);
}

// Inicializar ventana y controles
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, L"Mini Juego - Modo Gratis/Pro", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 420, 420, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HFONT hFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

    hBtnNormal = CreateWindowW(L"BUTTON", L"🎮 Modo normal (gratis)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        60, 50, 280, 40, hWnd, (HMENU)1, hInst, NULL);
    SendMessage(hBtnNormal, WM_SETFONT, (WPARAM)hFont, TRUE);

    hBtnPro = CreateWindowW(L"BUTTON", L"🔒 Modo PRO (bloqueado)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_DISABLED,
        60, 100, 280, 40, hWnd, (HMENU)2, hInst, NULL);
    SendMessage(hBtnPro, WM_SETFONT, (WPARAM)hFont, TRUE);

    hBtnBuy = CreateWindowW(L"BUTTON", L"💰 Comprar $49.99", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        60, 150, 280, 40, hWnd, (HMENU)3, hInst, NULL);
    SendMessage(hBtnBuy, WM_SETFONT, (WPARAM)hFont, TRUE);

    hEditLicense = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
        60, 210, 280, 30, hWnd, (HMENU)4, hInst, NULL);
    SendMessage(hEditLicense, WM_SETFONT, (WPARAM)hFont, TRUE);

    hBtnActivate = CreateWindowW(L"BUTTON", L"🔓 Activar licencia", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        60, 260, 280, 40, hWnd, (HMENU)5, hInst, NULL);
    SendMessage(hBtnActivate, WM_SETFONT, (WPARAM)hFont, TRUE);

    return TRUE;
}

// Lógica principal de la ventana
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case 1: // Modo normal
            MessageBox(hWnd, L"Entraste al modo normal. ¡A jugar!", L"Modo Normal", MB_OK);
            break;
        case 2: // Modo PRO
            MessageBox(hWnd, L"Modo PRO activado. ¡Bienvenido al lado pro!", L"Modo PRO", MB_OK);
            break;
        case 3: // Comprar
            MessageBox(hWnd, L"No disponible aún. ¡Esperamos tu dinero pronto! 💸", L"Comprar", MB_OK | MB_ICONINFORMATION);
            break;
        case 5: // Activar licencia
        {
            wchar_t serial[256];
            GetWindowText(hEditLicense, serial, 256);
            if (validateLicense(serial)) {
                EnableWindow(hBtnPro, TRUE);
                SetWindowText(hBtnPro, L"✅ Modo PRO (activado)");
                EnableWindow(hBtnActivate, FALSE);
                SetWindowText(hBtnActivate, L"✅ Activado");
                MessageBox(hWnd, L"¡Licencia válida! Has desbloqueado el modo PRO.", L"¡Éxito!", MB_OK | MB_ICONINFORMATION);
            }
            else {
                MessageBox(hWnd, L"Licencia inválida. Intentá de nuevo o pagá como la gente 😅", L"Error", MB_OK | MB_ICONERROR);
            }
        }
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

        // Fondo decorativo (opcional)
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, 100, 10, L"🎯 Mini Juego: Gratis vs Pro", 27);

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
