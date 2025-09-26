#include <iostream>
using namespace std;

int main() {
  int dividend[] = {10, -10, 10, -10, 0};
  int divisor[] = {3, 3, -3, -3, 3};

  for (int i = 0; i < 5; ++i) {
    cout << dividend[i] << " / " << divisor[i] << " = "
         << dividend[i] / divisor[i] << " remainder "
         << dividend[i] % divisor[i] << endl;
  }
}
