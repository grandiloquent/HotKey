#ifndef TRANSLATES_H
#define TRANSLATES_H
// #include "translates.hpp"
#define SION_DISABLE_SSL
#include "sion.h"
void google(){
	 wstring value = getString();
  const wchar_t* wp = value.c_str();
  char szUTF8[32];
  int nMbyteLen = WideCharToMultiByte(CP_UTF8, 0, wp, -1, NULL, 0, NULL, NULL);
  WideCharToMultiByte(CP_UTF8, 0, wp, -1, szUTF8, nMbyteLen, NULL, NULL);
  string encode = szUTF8;

  string uri =
      "http://translate.google.com/translate_a/"
      "single?client=gtx&sl=auto&tl=en&dt=t&dt=bd&ie=UTF-8&oe=UTF-8&dj=1&"
      "source=icon&q=" +
      urlencode(encode);
      cout<<uri<<endl;
  auto res =
      sion::Request()
          .SetUrl(uri)
          .SetHttpMethod(sion::Method::Get)
          .SetHeader("Connection", "close")
          .SetHeader("Accept", "application/json, text/javascript, */*; q=0.01")
          .SetHeader("User-Agent", "Mozilla/4.0")
          .Send();
  cout << res.Body().size() << endl;
  cout << Utf8ToGbk("ALT+9 同步GitHub") << endl;
}
#endif
