// --std=c++20 -lws2_32

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
#define SION_DISABLE_SSL
#include "sion.h"

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
        gitPush();
        break;
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main(int argc, char const* argv[]) {
  // setlocale(LC_ALL, "zh-CN");
  // createClangFiles("https");

  wstring value = getString();
  const wchar_t* wp = value.c_str();
  char szUTF8[32];
  int nMbyteLen = WideCharToMultiByte(CP_UTF8, 0, wp, -1, NULL, 0, NULL, NULL);
  WideCharToMultiByte(CP_UTF8, 0, wp, -1, szUTF8, nMbyteLen, NULL, NULL);
  string encode = szUTF8;

  string uri =
      "http://translate.google.cn/translate_a/"
      "single?client=gtx&sl=auto&tl=%s&dt=t&dt=bd&ie=UTF-8&oe=UTF-8&dj=1&"
      "source=icon&q=" +
      urlencode(encode);
  auto res =
      sion::Request()
          .SetUrl(uri)
          .SetHttpMethod(sion::Method::Get)
          .SetHeader("Connection", "close")
          .SetHeader("Accept", "application/json, text/javascript, */*; q=0.01")
          .SetHeader("User-Agent", "Mozilla/4.0")
          .Send();
  cout << res.Body() << endl;
  cout << Utf8ToGbk("ALT+9 同步GitHub") << endl;

  MSG msg;

  SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, GetModuleHandle(NULL),
                   0);  // Setup keyboard Hook

  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}