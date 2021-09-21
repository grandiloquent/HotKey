#ifndef BAIDU_H
#define BAIDU_H
// #include "baidu.hpp"

void uploadBooks() {
  std::vector<std::wstring> files = getCopyingFiles();
  if (!files.empty()) {
    std::filesystem::path dir = files[0];
    std::wstring n = dir.stem().wstring();
    std::wstring arg = L"BaiduPCS-Go.exe mkdir \"/书籍/书籍/";
    arg += n;
    exec(arg);
    std::vector<std::wstring> v = {L".epub", L".azw3", L".pdf", L".mobi"};
    for (auto& i : v) {
      std::filesystem::path file = dir.parent_path();
      file.append(n + i);
      if (std::filesystem::exists(file)) {
        std::wstring ag = L"BaiduPCS-Go.exe upload \"";
        ag = ag + dir.wstring();

        ag = ag + L"\" \"/书籍/书籍/" + n + L"\"";

        exec(ag);
      }
    }
    std::wstring share = L"BaiduPCS-Go.exe share set \"/书籍/书籍/";
    share += dir.stem().wstring();
    exec(share, [](std::wstring& str) {
      if (str.find(L"https:/pan.baidu.com") != std::string::npos) {
        setString(str);
      }
    });
  }
}
#endif
