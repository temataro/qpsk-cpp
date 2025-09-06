#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

int main() {
  /*
  How does one make an FIR filter again...?
  You take the frequency response you want,
  then you inverseFFT to find the sinc parameters
  that correspond to it.
  Your main problem now is the fact that
  your filter has to be causal...
  and finite.
  So you have to truncate the filter to
  N taps, (make sure N is an odd number
  because your sinc has to be symmetric
  about the y-axis.) and shift your
  filter N//2 + 1 samples to the right.
  Finally, we multipy the sinc with a
  windowing function.
  */

  printf("Hello, World!");
  return 0;
}
