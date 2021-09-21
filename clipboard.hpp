#ifndef CLIPBOARD_H
#define CLIPBOARD_H
// #include "clipboard.hpp"

std::vector<std::wstring> getCopyingFiles() {
  std::vector<std::wstring> filePaths{};

  using std::experimental::make_unique_resource_checked;
  auto cbOpen = make_unique_resource_checked(::OpenClipboard(nullptr), FALSE,
                                             [](BOOL) { ::CloseClipboard(); });

  if (not cbOpen.get()) return filePaths;
  {
    auto cbData = make_unique_resource_checked(::GetClipboardData(CF_HDROP),
                                               nullptr, [](auto) {});

    if (not cbData.get()) return filePaths;
    {
      auto hDrop = make_unique_resource_checked(
          ::GlobalLock(cbData.get()), nullptr,
          [hData = cbData.get()](auto) { ::GlobalUnlock(hData); });

      if (not hDrop.get()) return filePaths;
      {
        // query files from clipboard
        auto numFiles = ::DragQueryFileW(static_cast<HDROP>(hDrop.get()),
                                         0xFFFFFFFF, nullptr, 0);
        if (not numFiles) return filePaths;

        //++ IMPORTANT: NEVER EVER FORGET RESERVE
        filePaths.reserve(numFiles);

        wchar_t paths[MAX_PATH] = {0};
        for (UINT i = 0; i < numFiles; ++i) {
          auto len = ::DragQueryFileW(static_cast<HDROP>(hDrop.get()), i, paths,
                                      MAX_PATH);
          if (0 == len) continue;
          filePaths.emplace_back(paths);
        }
      }
    }
  }
  return filePaths;
}

std::wstring getString() {
  using std::experimental::make_unique_resource_checked;
  auto cbOpen = make_unique_resource_checked(::OpenClipboard(nullptr), FALSE,
                                             [](BOOL) { ::CloseClipboard(); });

  if (not cbOpen.get()) return std::wstring();
  {
    auto cbData = make_unique_resource_checked(
        ::GetClipboardData(CF_UNICODETEXT), nullptr, [](auto) {});

    if (not cbData.get()) return std::wstring();
    {
      auto hDrop = make_unique_resource_checked(
          ::GlobalLock(cbData.get()), nullptr,
          [hData = cbData.get()](auto) { ::GlobalUnlock(hData); });

      if (not hDrop.get()) return std::wstring();
      {
        std::wstring ws = reinterpret_cast<wchar_t*>(hDrop.get());
        return ws;
      }
    }
  }
  return std::wstring();
}

void setString(std::wstring& str) {
  if (OpenClipboard(NULL)) {
    HGLOBAL hMem =
        GlobalAlloc(GHND | GMEM_DDESHARE, (str.size() + 1) * sizeof(WCHAR));
    if (hMem) {
      wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
      memcpy(pMem, str.data(), str.size() * sizeof(wchar_t));
      pMem[str.size()] = 0;
      GlobalUnlock(hMem);

      EmptyClipboard();
      SetClipboardData(CF_UNICODETEXT, hMem);
    }

    CloseClipboard();
    GlobalFree(hMem);
  }
}
 

#endif
