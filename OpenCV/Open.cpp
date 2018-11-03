#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <windows.h>

int DoGetActiveWindow()
{
	char buf[1000];
	HWND hWnd;


	/* アクティブウィンドウの取得 */
	hWnd = GetForegroundWindow();
    
    GetWindowText(hWnd, buf, 1000);

    LPARAM lParam = (LPARAM)"ResearchProject2";
   
    if(strcmp(buf,(char*)lParam)==0){//名前が一致したら、

        std::cout << buf << std::endl;

        return 1; 

	}else{
        return 0;
    }
    
    if(hWnd == NULL){
         std::cout << "not handle" << std::endl;
         return 0;
    }
	
    return 0;
}

DWORD WINAPI call_system(LPVOID arg){
    system("C:\\Users\\mizuho\\Desktop\\sotuken\\ResearchProject2");
}

int main(){
    int c = 0;
    HANDLE hThread;
    DWORD dwThreadId;

    hThread = CreateThread(  //スレッドの起動
        NULL,
        0,
        call_system,
        NULL,
        0,
        &dwThreadId);

    Sleep(5000);

    int i = DoGetActiveWindow();


    if(i == 0){
        std::cout << "Not Active" << std::endl;
        DoGetActiveWindow();
    }else if(i == 1){
        std::cout << "OKOKOK" << std::endl;
        return 0;
    }

    return 0;
}
