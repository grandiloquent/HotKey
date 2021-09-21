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
          std::wstring arg=L"BaiduPCS-Go.exe mkdir \"/书籍/书籍/";
          arg+=dir.stem().wstring();
          exec(arg, [](auto& str) { std::wcout << str << std::endl; });
          std::vector<std::wstring> v = {L".epub", L".azw3", L".pdf", L".mobi"};
          for (auto& i : v) {
            std::filesystem::path file = dir.parent_path();
             file.append(dir.stem().wstring()+i);
             if (std::filesystem::exists(file)) { 
               std::wstring ag = L"BaiduPCS-Go.exe upload \"";
               ag += file.stem().wstring();
               ag += L"\" \"/书籍/书籍/" + file.filename().wstring() + L"\"";
               exec(ag);
             }
          }
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

  //std::filesystem::path dir{u8""};

  // std::thread t([&] {

  // });
  
  MSG msg;

  SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, GetModuleHandle(NULL),0);  // Setup keyboard Hook

  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}