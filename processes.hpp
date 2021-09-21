#ifndef PROCESS_H
#define PROCESS_H
// #include "process.hpp"
void displayError(LPCTSTR errorDesc, DWORD errorCode) {
  TCHAR errorMessage[1024] = TEXT("");

  DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
                FORMAT_MESSAGE_MAX_WIDTH_MASK;

  FormatMessage(flags, nullptr, errorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorMessage,
                sizeof(errorMessage) / sizeof(TCHAR), nullptr);

#ifdef _UNICODE
  std::wcerr << L"Error : " << errorDesc << std::endl;
  std::wcerr << L"Code    = " << errorCode << std::endl;
  std::wcerr << L"Message = " << errorMessage << std::endl;
#else
  std::cerr << "Error : " << errorDesc << std::endl;
  std::cerr << "Code    = " << errorCode << std::endl;
  std::cerr << "Message = " << errorMessage << std::endl;
#endif
}
void createIoPipe(HANDLE& hRead, HANDLE& hWrite) {
  SECURITY_ATTRIBUTES saAttr;

  // Set the bInheritHandle flag so pipe handles are inherited.
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = nullptr;

  // Create a pipe for the child process's IO
  if (!CreatePipe(&hRead, &hWrite, &saAttr, 0)) return;

  // Ensure the read handle to the pipe for IO is not inherited.
  if (!SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0)) return;
}


 
void readFromPipe(HANDLE hProcess, HANDLE hRead,
                  std::function<void(std::wstring&)> handle) {
  DWORD dwAvail, dwRead;//, dwWritten;
  const auto size=4096;
  auto chBuf= std::make_unique<char[]>(size);
  std::fill(chBuf.get(),chBuf.get()+size,'\0');
  BOOL bSuccess = FALSE;
  //HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
 
  std::string v;
  for (;;) {
    DWORD exit_code;
    GetExitCodeProcess(hProcess, &exit_code);  // while the process is running
    if (exit_code != STILL_ACTIVE) break;

    PeekNamedPipe(hRead, chBuf.get(), size-1, &dwRead, &dwAvail, nullptr);
    if (dwAvail == 0) continue;

    bSuccess = ReadFile(hRead, chBuf.get(),size-1, &dwRead, nullptr);
   
    if (!bSuccess || dwRead == 0) break;

    v.append(chBuf.get(), dwRead);
    std::fill(chBuf.get(),chBuf.get()+size,'\0');
    //bSuccess = WriteFile(hParentStdOut, chBuf, dwRead, &dwWritten, nullptr);
    //if (!bSuccess) break;
  }

  auto result = string2wstring(v);

  if (handle != nullptr) handle(result);
}
int exec(std::wstring& cmd,std::function<void(  std::wstring&)> handle=nullptr) {
  STARTUPINFOW si;
  PROCESS_INFORMATION pi;

  // Create the IO pipe
  HANDLE hWrite, hRead;
  createIoPipe(hRead, hWrite);

  // Initialize memory
  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
  si.hStdOutput = hWrite;  // GetStdHandle(STD_OUTPUT_HANDLE);
  si.hStdError = hWrite;   // GetStdHandle(STD_ERROR_HANDLE);
  si.dwFlags |= STARTF_USESTDHANDLES;

  // Construct the command line
  /*
  const char* prg, const char* params = nullptr
  const char* separator = " ";
  const char* terminator = "\0";
  std::vector<char> cmd;
  cmd.reserve(strlen(prg) + (params ? strlen(params) : 0) + 2);

  std::copy(prg, prg + strlen(prg), std::back_inserter(cmd));

  if (params) {
    std::copy(separator, separator + 1, std::back_inserter(cmd));
    std::copy(params, params + strlen(params), std::back_inserter(cmd));
  }
  std::copy(terminator, terminator + 1, std::back_inserter(cmd));
*/
  if (CreateProcessW(
          nullptr,     //_In_opt_     LPCTSTR lpApplicationName,
          cmd.data(),  //_Inout_opt_  LPTSTR lpCommandLine,
          nullptr,     //_In_opt_     LPSECURITY_ATTRIBUTES lpProcessAttributes,
          nullptr,     //_In_opt_     LPSECURITY_ATTRIBUTES lpThreadAttributes,
          TRUE,        //_In_         BOOL bInheritHandles,
          0,           //_In_         DWORD dwCreationFlags,
          nullptr,     //_In_opt_     LPVOID lpEnvironment,
          nullptr,     //_In_opt_     LPCTSTR lpCurrentDirectory,
          &si,         //_In_         LPSTARTUPINFO lpStartupInfo,
          &pi          //_Out_        LPPROCESS_INFORMATION lpProcessInformation
          ) == FALSE) {
    DWORD err = GetLastError();
    displayError(TEXT("Unable to execute."), err);

    return -1;
  }

  // Read all the output
  readFromPipe(pi.hProcess, hRead,handle);

  // Successfully created the process.  Wait for it to finish.
  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exit_code;
  GetExitCodeProcess(pi.hProcess, &exit_code);

  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);
  CloseHandle(hRead);
  CloseHandle(hWrite);

  std::flush(std::cout);

  return exit_code;
}
#endif
