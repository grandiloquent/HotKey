// --std=c++20  

#include <Windows.h>

#include <algorithm>
 
#include <vector>
#include <iostream>
#include <fstream> 
#include <sys/stat.h>
#include <filesystem> 
#include <thread>
#include "strings.hpp"
#include "scope.hpp"
#include "clipboard.hpp"
#include "processes.hpp"
#include "files.hpp"

using namespace std;


LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
    DWORD
    vkCode = ((LPKBDLLHOOKSTRUCT)lParam)->vkCode;
    if (vkCode != 165) std::cout << vkCode << " ";
    switch (vkCode) {
      case 48:  // 0
        std::vector<std::wstring> files = getCopyingFiles();
        if(!files.empty()){
          std::filesystem::path dir = files[0];
          std::wstring n=dir.stem().wstring(); 
          std::wstring arg=L"BaiduPCS-Go.exe mkdir \"/书籍/书籍/";
          arg+=n;
          exec(arg);
          std::vector<std::wstring> v = {L".epub", L".azw3", L".pdf", L".mobi"};
          for (auto& i : v) {
            std::filesystem::path file = dir.parent_path();
             file.append(n+i); 
             if (std::filesystem::exists(file)) {
               std::wstring ag=L"BaiduPCS-Go.exe upload \"";
               ag=ag+ dir.wstring();

               ag = ag + L"\" \"/书籍/书籍/" + n +  L"\"";

               exec(ag);
             }
          }
          std::wstring share=L"BaiduPCS-Go.exe share set \"/书籍/书籍/";
          share+=dir.stem().wstring();
          exec(share, [](std::wstring& str) {
            if (str.find(L"https:/pan.baidu.com") != std::string::npos) {
              setString(str);
            }
          });
        }
        break;

      // case VK_F11:
      //   break;

      // case VK_F12:
      //   break;
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main(int argc, char const* argv[]) {
  //setlocale(LC_ALL, "zh-CN");

 
  
  MSG msg;

  SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, GetModuleHandle(NULL),0);  // Setup keyboard Hook

  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}