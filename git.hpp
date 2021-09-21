#ifndef GIT_H
#define GIT_H
// #include "git.hpp"

void gitPush() {
  chdir("C:\\Users\\psycho\\Desktop\\Key");
  system("git add .");
  wstring git = L"git commit -m ";
  git += getString();
  system(gbk(git.c_str()));
  system("git push");
}
#endif