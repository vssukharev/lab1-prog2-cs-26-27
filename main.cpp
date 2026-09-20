
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include "tormentor.h"
#include "io.h"

using namespace std;
using namespace std::filesystem;

const char* help = "Usage: main <file>\n"
                   "  file - input file name";

path parse_command_args(int argc, const char** argv) {
  if (argc < 2) throw runtime_error{help};
  if (argc > 2) throw runtime_error{help};

  path res = argv[1];

  if (!exists(res)) throw runtime_error{"File does not exists"};
  if (!is_regular_file(res)) throw runtime_error{"File is invalid"};

  return res;
}

void process(istream& is) {
  Tormentor torm = read_tormentor(is);
  Torture t;

  size_t i = 1;
  while (torm) {
    t = torm();
    cout << "Iteration " << i << ":\n";
    cout << t;
    cout << '\n';
    ++i;
  }
}

int main(int argc, const char** argv) {
  try {
    path filename = parse_command_args(argc, argv);
    ifstream ifs{filename};
    process(ifs);

  } catch (const exception& err) {
    cerr << err.what() << endl;
    return 1;
  }

  return 0;
}

