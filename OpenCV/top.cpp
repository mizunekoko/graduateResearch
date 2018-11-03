#include <tchar.h>
#include <memory>
#include <Clipbrd.hpp>

int main(){
    
//プライマリモニタの画面全体の幅と高さ
const int screenX = Screen->PrimaryMonitor->Width;
const int screenY = Screen->PrimaryMonitor->Height;

//ビットマップの作成
std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
bmp->Width = screenX;
bmp->Height = screenY;

//デバイスコンテキストの作成
HDC screenDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
//ビットブロック転送
BitBlt(bmp->Canvas->Handle, 0, 0, screenX, screenY, screenDC, 0, 0, SRCCOPY);
//デバイスコンテキストの削除
DeleteDC(screenDC);

//ビットマップをクリップボード形式でコピー
unsigned int handle;
HPALETTE palette;
unsigned short format;
bmp->SaveToClipboardFormat(format, handle, palette);

//クリップボードに書き込み
Clipboard()->SetAsHandle(format, handle);


//キャプチャしたときに移り込まないように、最小化して非表示
Application->MainForm->WindowState = wsMinimized;
Application->MainForm->Visible = false;

//ウィンドウサイズを取得
RECT rect;
GetWindowRect(GetForegroundWindow(), &rect);
const int width = rect.right - rect.left;
const int height = rect.bottom - rect.top;

//ビットマップの作成
std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
bmp->Width = width;
bmp->Height = height;

//デバイスコンテキストの作成
HDC screenDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
//ビットブロック転送
BitBlt(bmp->Canvas->Handle, 0, 0, width, height, screenDC, rect.left, rect.top, SRCCOPY);
//デバイスコンテキストの削除
DeleteDC(screenDC);

//ビットマップをクリップボード形式でコピー
unsigned int handle;
HPALETTE palette;
unsigned short format;
bmp->SaveToClipboardFormat(format, handle, palette);
//クリップボードに書き込み
Clipboard()->SetAsHandle(format, handle);

//ウィンドウを元に戻す
Application->MainForm->Visible = true;
Application->MainForm->WindowState = wsNormal;
}
