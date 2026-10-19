#include <iostream>
#include <stack>
#include <string>

using namespace std;

class Solution {
public:
  string removeDuplicates(string s) {
    stack<char> stk;
    for (char c : s) {
      if (stk.empty() || stk.top() != c) {
        stk.push(c);
      } else {
        stk.pop();
      }
    }
    string result = "";
    while (!stk.empty()) {
      result = stk.top() + result;
      stk.pop();
    }
    return result;
  }
};
