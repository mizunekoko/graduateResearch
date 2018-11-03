#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace Gdiplus;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT num, size;
    GetImageEncodersSize(&num, &size);
    ImageCodecInfo* pci = (ImageCodecInfo*)(malloc(size));
    GetImageEncoders(num, size, pci);
    
    for (int i = 0; i < num; i++) {
        if (wcscmp(pci[i].MimeType, format) == 0 ) {
            *pClsid = pci[i].Clsid;
            free(pci);
            return 0;
        }
    }
    free(pci);
    return 1;
}

int SaveBitmapImageByJpeg(HBITMAP hbmp, char* fileName) {
    GdiplusStartupInput gsi;
    ULONG token;
    GdiplusStartup(&token, &gsi, NULL);
    
    wchar_t wFileName[1<<16];
    mbstowcs(wFileName, fileName, strlen(fileName));
    
    Bitmap* pbmp = Bitmap::FromHBITMAP(hbmp, NULL);
    CLSID clsid;
    GetEncoderClsid(L"image/jpeg", &clsid);
    pbmp->Save(wFileName, &clsid, NULL);
    delete pbmp;
    
    GdiplusShutdown(token);
    
    return 0;
}

int SaveImage(HWND hwnd, HDC hdc, char* fileName) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    UINT width = rect.right - rect.left;
    UINT height = rect.bottom - rect.top;
    
    HDC hmdc = CreateCompatibleDC(hdc);
    HBITMAP hbmp = CreateCompatibleBitmap(hdc, width, height);
    
    HBITMAP hbmpOld = (HBITMAP)SelectObject(hmdc, hbmp);
    BitBlt(hmdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
    SelectObject(hmdc, hbmpOld);
    
    SaveBitmapImageByJpeg(hbmp, fileName);
    
    DeleteObject(hbmp);
    DeleteDC(hmdc);
    return 0;
}


int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
                   LPSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    char fileName[1<<16];
    HWND hwnd;
    HDC hdc;
    int n = 0;
    
    // Scroll Lock キーでデスクトップ全体をキャプチャ
    RegisterHotKey(NULL,  1, 0, VK_SCROLL);
    // Ctrl+Scroll Lockでフォアグランドウィンドウのみをキャプチャ
    RegisterHotKey(NULL,  2, MOD_ALT, VK_SCROLL);
    // Ctrl+Alt+Scroll Lockでプログラム終了
    RegisterHotKey(NULL, 99, MOD_ALT | MOD_CONTROL, VK_SCROLL);
    
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            switch (msg.wParam) {
                case  1:
                    hwnd = GetDesktopWindow();
                    hdc = GetDC(NULL);
                    sprintf(fileName, "%s%03d.jpg", lpsCmdLine, n);
                    n++;
                    SaveImage(hwnd, hdc, fileName);
                    ReleaseDC(NULL, hdc);
                    break;
                case  2:
                    hwnd = GetForegroundWindow();
                    hdc = GetWindowDC(hwnd);
                    sprintf(fileName, "%s%03d.jpg", lpsCmdLine, n);
                    n++;
                    SaveImage(hwnd, hdc, fileName);
                    ReleaseDC(hwnd, hdc);
                    break;
                case 99:
                    PostQuitMessage(0);
            }
        }
    }
    return msg.wParam;
}
