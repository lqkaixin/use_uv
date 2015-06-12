#include <iostream>
#include <cstdio>
using namespace std;

class A
{
  public:
  A(){printf("created\n");}
  A(const A &a){printf("created A\n");}
  A& operator = (const A&a){printf("created b\n");}
  ~A(){printf("destory\n");}
};

#define foo(a) ({A b = a; b;})

inline A fo(A a){A b=a;return b;};

int main()
{
  A a;
  A b = foo(a);
  printf("fo\n");
  A c = fo(a);
  return 0;
}
