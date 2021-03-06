#ifndef STRINGS_H
#define STRINGS_H
// #include "strings.hpp"

using namespace std;
wstring string2wstring(string str) {
  wstring result;
  //获取缓冲区大小，并申请空间，缓冲区大小按字符计算
  int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0);
  wchar_t *buffer = new wchar_t[len + 1];
  //多字节编码转换成宽字节编码
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), buffer, len);
  buffer[len] = '\0';  //添加字符串结尾
  //删除缓冲区并返回值
  result.append(buffer);
  delete[] buffer;
  return result;
}
string GbkToUtf8(const char *src_str) {
  int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
  wchar_t *wstr = new wchar_t[len + 1];
  memset(wstr, 0, len + 1);
  MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
  len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
  char *str = new char[len + 1];
  memset(str, 0, len + 1);
  WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
  string strTemp = str;
  if (wstr) delete[] wstr;
  if (str) delete[] str;
  return strTemp;
}

string Utf8ToGbk(const char *src_str) {
  int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
  wchar_t *wszGBK = new wchar_t[len + 1];
  memset(wszGBK, 0, len * 2 + 2);
  MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
  len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
  char *szGBK = new char[len + 1];
  memset(szGBK, 0, len + 1);
  WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
  string strTemp(szGBK);
  if (wszGBK) delete[] wszGBK;
  if (szGBK) delete[] szGBK;
  return strTemp;
}
char *gbk(const wchar_t *wp) {
  int nMbyteLen = WideCharToMultiByte(CP_ACP, 0, wp, -1, NULL, 0, NULL, NULL);
  char *buffer = (char *)malloc(nMbyteLen + 1);
  WideCharToMultiByte(CP_ACP, 0, wp, -1, buffer, nMbyteLen, NULL, NULL);
  return buffer;
}
#endif
