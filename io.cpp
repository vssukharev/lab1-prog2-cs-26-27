
#include <sstream>
#include "io.h"

using namespace std;

ostream& operator<<(ostream& out, const Torture& tort) {
  for (auto [w, l] : tort) {
    out << w << " -> " << l << '\n';
  }
  return out;
}

class TormentorParser {
private:
  string line;
  istringstream iss;
  Tormentor res;
  int w;
  int l;

  bool skip() const;
  void parse_line();

public:
  Tormentor parse(istream&);
};

Tormentor TormentorParser::parse(istream& is) {
  // Retrive number of workers and labours
  getline(is, line);
  while (skip()) { getline(is, line); }
  iss.clear();
  iss.str(line);
  
  iss >> w;
  if (!iss) throw runtime_error{"Invalid number of workers"};
  if (w <= 0) throw runtime_error{"Number of workers must be positive"};

  iss >> l;
  if (!iss) throw runtime_error{"Invalid number of labours"};
  if (l <= 0) throw runtime_error{"Number of labours must be positive"};

  res = Tormentor(w, l);

  // The rest of adj list
  while (getline(is, line)) {
    parse_line();
  }

  if (!is.eof()) throw runtime_error{"Invalid input"};
  return std::move(res);
}

bool TormentorParser::skip() const {
  if (line.starts_with('#')) return true;
  for (char c : line) if (!std::isspace(c)) return false;
  return true;
}

void TormentorParser::parse_line() {
  if (skip()) return;

  iss.clear();
  iss.str(line);
  iss >> w;
  if (!iss) throw runtime_error{"Invalid worker"};
  if (w < 0) throw runtime_error{"Negative worker"};

  bool ok = false;
  while (iss >> l) {
    if (l < 0) throw runtime_error{"Negative labour"};
    ok = true;
    res.add_work(w, l);
  }

  if (!ok || !iss.eof()) throw runtime_error{"Invalid labour"};
}

Tormentor read_tormentor(istream& in) {
  TormentorParser reader;
  return reader.parse(in);
}

