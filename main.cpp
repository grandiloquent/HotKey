//   --std=c++20 -lws2_32

#include <Windows.h>
#include <sys/stat.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include "scope.hpp"
#include "strings.hpp"
#include "clipboard.hpp"
#include "processes.hpp"
#include "baidu.hpp"
#include "files.hpp"
#include "git.hpp"
#include "https.hpp"
//#include "rapidjson/document.h"


using namespace std;

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
    DWORD
    vkCode = ((LPKBDLLHOOKSTRUCT)lParam)->vkCode;
    if (vkCode != 165) std::cout << vkCode << " ";
    switch (vkCode) {
      case 48:  // 0

        break;

      case 57:  // 9
        thread t([] { gitPush(); });
        break;
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main(int argc, char const* argv[]) {
  // setlocale(LC_ALL, "zh-CN");
   createClangFiles("translates");

 

  MSG msg;

  SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, GetModuleHandle(NULL),
                   0);  // Setup keyboard Hook

  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}