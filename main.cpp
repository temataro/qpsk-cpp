#include <cmath>
#include <complex>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

// clang-format off
#define M                                                      2
#define dt                                   (float) 1/SAMP_RATE
#define PI                                        3.14159265359f
#define SAMP_RATE                                      1'000'000
#define BAUD_RATE                                        128'000
#define BIT_RATE                                   M * BAUD_RATE
#define fARRAY_SIZE(vec) (size_t) 2 * vec.size() * sizeof(float)
// clang-format on

int writeIQToFile(const std::string &filename,
                  const std::vector<std::complex<float>> &iq_data);

int main(void) {
  std::string outfile = "sine.iq";
  size_t num_samples = 1LL << 22;
  float sig_magnitude = 5.0f;

  std::vector<std::complex<float>> symbols = {
      1.f + 1.f * 1j,
      -1.f + 1.f * 1j,
      -1.f - 1.f * 1j,
      1.f - 1.f * 1j,
      std::polar(1.f, 45.f * 180 / PI),
  };

  // for (std::complex<float> sym : symbols) {
  //   printf("%02.2f + 1j*%02.2f\n", sym.real(), sym.imag());
  //   printf("(r, theta) = (%.2f, %.2f)\n", std::abs(sym), (float) 180  *
  //   std::arg(sym) / PI);
  // }

  // printf("\n\nTesting file output to %s.", outfile);

  // Make a sine wave
  float f = 100.f;
  std::vector<std::complex<float>> signal(num_samples);
  for (int i = 0; i < num_samples; i++) {
    signal[i] = std::polar(sig_magnitude, (float) 2 * PI * f * i * dt);
  }

  writeIQToFile("sine.iq", signal);

  return 0;
}

int writeIQToFile(const std::string &filename,
                  const std::vector<std::complex<float>> &iq_data) {
  FILE *fd;

  fd = fopen(filename.c_str(), "wb");
  if (!fd) {
    fprintf(stderr, "%s could not be opened!", filename);
    return 1;
  }

  // Write to file
  size_t written_bytes = 0;
  for (const std::complex<float> &samp : iq_data) {
    float i = samp.real();
    float q = samp.imag();
    fwrite(&i, sizeof(float), 1, fd);
    fwrite(&q, sizeof(float), 1, fd);
    written_bytes += 2 * sizeof(float);
  }


  if (written_bytes != fARRAY_SIZE(iq_data)) {
    fprintf(stderr, "Not all samples written to file %s. (Only %d / %d written.)",
            filename, fARRAY_SIZE(iq_data));
    return 1;
  }
  printf("%x bytes written.", fARRAY_SIZE(iq_data));

  fclose(fd);

  return 0;
}
