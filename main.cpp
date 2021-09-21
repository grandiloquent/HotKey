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
#include "baidu.hpp"

using namespace std;

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
    DWORD
    vkCode = ((LPKBDLLHOOKSTRUCT)lParam)->vkCode;
    if (vkCode != 165) std::cout << vkCode << " ";
    switch (vkCode) {
      case 48:  // 0
       
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
 
  chdir("C:\\Users\\psycho\\Desktop\\Key");
  system("git add .");
  wstring git = L"git commit -m ";
  git += getString();
  system(gbk(git.c_str()));
  system("git push");
 
  MSG msg;

  SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, GetModuleHandle(NULL),0);  // Setup keyboard Hook

  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}