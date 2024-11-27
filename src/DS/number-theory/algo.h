namespace Algo
{
  int gcdEuclidean(int x, int y) {
   // std::cout << "(x, y) = (" << x << ", " << y << ")" << std::endl;
    if (y == 0) {
      return x;
    }

    return gcdEuclidean(y, x % y);
  }

  int gcdExEuclidean(int x, int y, int& a, int& b) {
    int a2 = 1, a1 = 0;
    int b2 = 0, b1 = 1;

    while (y > 0) {
      int q = x / y;
      int r = x % y;

      a = a2 - q * a1;
      b = b2 - q * b1;

      x = y;
      y = r;

      a2 = a1, a1 = a;

      b2 = b1, b1 = b;
    }

    a = a2;
    b = b2;
    return x;
  }

  int mulInverse(int a, int m) {
    int x, y;
    const bool isGCD = gcdExEuclidean(a, m, x, y) == 1;

    x = (x + m) % m;
    y = (y + m) % m;

    if (!isGCD) {
      return -1;
    }

    return x;
  }

}