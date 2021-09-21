#ifndef FILES_H
#define FILES_H
// #include "files.hpp"


using namespace std;

void clearDesktop() {
  const std::filesystem::path sandbox{"C:\\Users\\psycho\\Desktop"};
  for (auto const& dir_entry : std::filesystem::directory_iterator{sandbox}) {
    if (dir_entry.is_regular_file()) {
      string extension = dir_entry.path().extension().string();
      std::transform(extension.begin(), extension.end(), extension.begin(),
                     ::toupper);
      std::filesystem::path destination{u8"C:\\Users\\psycho\\Desktop\\资源"};
      destination.append(extension);
      if (!std::filesystem::exists(destination)) {
        std::filesystem::create_directory(destination);
      }
      destination.append(dir_entry.path().filename().c_str());
      std::filesystem::rename(dir_entry, destination);
    }
  }
}

void createClangFiles(string fileName){
  std::ofstream source;
   
  struct stat s;
  if (stat((fileName + ".hpp").c_str(), &s) != 0 || !S_ISREG(s.st_mode)) {
    source.open(fileName + ".hpp");
    string str = fileName;
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    source << "#ifndef " << str << "_H" << std::endl;
    source << "#define " << str << "_H" << std::endl;
    source << "// #include \"" << fileName << ".hpp\"" << std::endl;
    source << "#endif" << std::endl;
    source.close();
  }
}
#endif
