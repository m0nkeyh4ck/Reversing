#include "framework.h"
#include "reto1.h"
#include <string>

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// XOR string simple
std::wstring xorString(const std::wstring& input) {
    std::wstring output;
    for (wchar_t c : input) {
        output += c ^ 0x5A;
    }
    return output;
}

// ✅ Codificación correcta
std::wstring usuarioCodificado = L"\x3B\x3E\x37\x33\x34";       // "admin"
std::wstring contrasenaCodificada = L"\x6B\x68\x69\x6E";         // "1234"

bool verificarCredenciales(const std::wstring& usuario, const std::wstring& contrasena) {
    return xorString(usuarioCodificado) == usuario && xorString(contrasenaCodificada) == contrasena;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 👇 Solo cambiamos el título a mano
    wcscpy_s(szTitle, L"Panel de Autenticación");
    LoadStringW(hInstance, IDC_RETO1, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RETO1));  // 👈 Ícono restaurado
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(40, 40, 40));
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // 👈 También ícono pequeño

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, 0, 400, 240, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hUsuarioEdit, hContrasenaEdit, hBotonLogin, hMensaje;

    switch (message)
    {
    case WM_CREATE:
        CreateWindow(L"STATIC", L"Usuario: ", WS_VISIBLE | WS_CHILD, 20, 20, 80, 20, hWnd, nullptr, hInst, nullptr);
        hUsuarioEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 20, 200, 20, hWnd, nullptr, hInst, nullptr);

        CreateWindow(L"STATIC", L"Contraseña: ", WS_VISIBLE | WS_CHILD, 20, 60, 90, 20, hWnd, nullptr, hInst, nullptr);
        hContrasenaEdit = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 120, 60, 200, 20, hWnd, nullptr, hInst, nullptr);

        hBotonLogin = CreateWindow(L"BUTTON", L"Login", WS_VISIBLE | WS_CHILD, 100, 100, 200, 25, hWnd, (HMENU)1, hInst, nullptr);
        hMensaje = CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 20, 140, 320, 20, hWnd, nullptr, hInst, nullptr);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1)
        {
            wchar_t usuario[100], contrasena[100];
            GetWindowText(hUsuarioEdit, usuario, 100);
            GetWindowText(hContrasenaEdit, contrasena, 100);

            if (verificarCredenciales(usuario, contrasena))
                SetWindowText(hMensaje, L"¡Acceso concedido!");
            else
                SetWindowText(hMensaje, L"¡Acceso denegado!");
        }
        break;

    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORBTN:
    {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, RGB(220, 220, 220));
        SetBkColor(hdc, RGB(40, 40, 40));
        return (INT_PTR)CreateSolidBrush(RGB(40, 40, 40));
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
