#include <filesystem>
#include <fstream>
#include <iostream>

#include "src/commons.h"

int main(int argc, char *argv[]) {
  std::filesystem::path rp(argv[0]);
  std::filesystem::path fp =
      std::filesystem::absolute(std::filesystem::relative(
          rp.parent_path(), std::filesystem::current_path()));
  fp.append("01.txt");

  std::ifstream file(fp);
  if (!file) {
    std::cerr << "Error: file at " << fp << " couldn't be opened" << std::endl;
  }
  // count character
  int characterCount = 0;
  int lineCount = 0;

  while (true) {
    char c = file.get();
    if (!file.good()) {
      break;
    }

    if (c == '\n') {
      lineCount++;
    }
    characterCount++;
  }
  std::cout << "Line = " << lineCount
            << ", Character(whitespace counted) = " << characterCount
            << std::endl;
  footer();
}
