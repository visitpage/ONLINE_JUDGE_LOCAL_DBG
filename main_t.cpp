//#include <bits/stdc++.h>
//using namespace std;
//template<class T> ostream& operator<<(ostream& os, const vector<T>& a) { for (const T& x : a) { os << x << ' '; } return os; }
//
//void solve() {
//}
//
//int32_t main() {
//  ios::sync_with_stdio(false); cin.tie(nullptr);
//#ifdef TEST_SOLUTION_N_TIMES
//  freopen("../input.txt", "r", stdin);
//  int N;
//  cin >> N;
//  for (int i = 0; i < N; i++)
//#endif
//    solve();
//  return 0;
//}
//
//#ifdef LOCAL_DBG
//#include "dbg/che.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/che.h
//#include "dbg/gen.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/gen.h
//struct DBG {
//  DBG() {
//    while (true) {
//      gen::testcases("../input.txt", 1, [&] (ofstream& file) {
//        // generating testcase...
//      });
//      che::program("../input.txt", "../outputA.txt", solve).run();
//      che::program("../input.txt", "../outputB.txt", [&] () {
//        // brute force...
//      }
//      }).run();
//      assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));
//    }
//  }
//} DBG;
//#endif