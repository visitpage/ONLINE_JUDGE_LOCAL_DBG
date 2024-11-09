## 本仓库头文件的`名字`，`功能`和`用法`说明：
## **"gen.h"** （generator.h）
#### 1. description：RandomTestcaseGenerating （随机测试用例生成）
#### 2. how to use: put under code inside main.cpp （Example #1）
```cpp
#ifdef LOCAL_DBG  
#include "dbg/gen.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/gen.h
struct GEN {  
  GEN() {  
    gen::testcases("../input.txt", 1, [&] (ofstream& file) {  
      auto list = gen::split(1e2);  
      file << list.size() << '\n';  
      for (int n : list) {  
        file << n << '\n';  
        for (int i = 0; i < n; ++i) {  
          file << gen::Ints(1, 100)(n) << '\n';  
        }  
        file << '\n';  
      }  
    });  
  }  
} GEN;  
#endif
```

## **"che.h"** （checker.h)
#### 1. description：SolutionCorrectnessCheck（解法正确性检查）
#### 2. how to use: put under code inside main.cpp（Example #1）
```cpp
#ifdef LOCAL_DBG  
#include "dbg/che.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/che.h 
struct CHE {  
  CHE() {  
    che::program("../input.txt", "../outputA.txt", solve).run();  
    che::program("../input.txt", "../outputB.txt", [&] () {  
    }).run();  
    assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));  
  }  
} CHE;  
#endif
```