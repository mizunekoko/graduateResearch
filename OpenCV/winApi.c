#include<windows.h>

void __fastcall TForm1::KeyItemClick(TObject *Sender)
{
    keybd_event(VK_SNAPSHOT, 0, 0, 0); // フルスクリーン 

    //クリップボードにコピーされるのを待つ
    Sleep(500);
    Application->ProcessMessages();
    
    //Clipboardの画像をImage1へ
    Image1->Picture->Assign(Clipboard());
}

void __fastcall TForm1::APIItemClick(TObject *Sender)
{    
    //スクリーンの大きさに合わせる
    Image1->Width  = Screen->Width;
    Image1->Height = Screen->Height;

    HDC dc = GetWindowDC(0);//HDC を取得
    Graphics::TCanvas *canvas = new Graphics::TCanvas;//Cavas 作成
    canvas->Handle = dc;//Canvas に取得した HDC を代入

    TRect rect = Rect(0, 0, Screen->Width, Screen->Height);
    Image1->Canvas->CopyRect(rect, canvas, rect);

    //Cavas を破棄 
    delete canvas;

    //デバイスコンテキストを開放する
    ReleaseDC(0, dc);
}