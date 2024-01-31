#include <iostream>

int hi = 0;
int zarray[1];
int array[5] = {1, 2, 3, 4, 5};
int array2[5] = {6, 7, 8, 9, 10};
int arrayField = array[2];

struct myStructType {
  int i;
  char const* s;
  char const* sa[4];
} mystruct = {3, "house", {"how", "now", "brown", "cow"}};

int count(int i);

int zomg = 2;

int main() {

  int i = 0;
  int r = 0;
  while(i < 10) {
    i = count(i);
    ++hi;
    r = hi * 10;
    (void)r;
  }
}

void hello() {}

int count(int i) {
  if(i % 2 == 0) {
    hello();
  }
  array[i % 5]++;
  return i + 1;
}

int count(int i, char* n) {
  printf("%s", n);
  return i + 1;
}

int printfWrapper(char* s) {
  printf("%s", s);
  return 1;
}

int count(char* s) {
  printf("%s\n", s);
  return 1;
}
