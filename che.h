//
// Created by sion on 2024/11/8.
//

#ifndef SOLUTION_CHE_H
#define SOLUTION_CHE_H

#include <iostream>
#include <functional>
#include <fstream>

using namespace std;

namespace che {
  struct program {
    ifstream in;
    ofstream out;
    function<void()> solve;
    program(const string& in, const string& out, const function<void()>& f): in(in), out(out), solve(f) {}
    void run() const {
      cout << "che::program.run" << endl;
      streambuf *oldCinBuf = cin.rdbuf(), *oldCoutBuf = cout.rdbuf();
      cin.rdbuf(in.rdbuf()); cout.rdbuf(out.rdbuf());
#ifdef TEST_SOLUTION_N_TIMES
      int N;
      cin >> N;
      for (int i = 0; i < N; i++)
#endif
        solve();
      cin.rdbuf(oldCinBuf); cout.rdbuf(oldCoutBuf);
    }
  };
  bool isContentConsistent(const string& filenameA, const string& filenameB) {
    cout << "che::isContentConsistent" << endl;
    std::ifstream fileA(filenameA), fileB(filenameB);
    for (std::string a, b; a == b && !fileA.eof() && !fileB.eof(); fileA >> a, fileB >> b) {}
    return fileA.eof() && fileB.eof();
  }
}

#endif //SOLUTION_CHE_H
