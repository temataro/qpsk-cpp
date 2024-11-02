#include <cmath>
#include <complex>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#define PI 3.14159265359f

/*
 *  Write iq signals into a binary file in complex float 32 format.
 */

// clang-format off
namespace config{
constexpr int M                                 =                           2;
constexpr unsigned int SAMP_RATE                =                   1'000'000;
constexpr unsigned int BAUD_RATE                =                     128'000;
constexpr float dt                              = (float) 1/config::SAMP_RATE;
constexpr unsigned int BIT_RATE                 =       M * config::BAUD_RATE;
constexpr float RAD2DEG(float radians){
  return (float) radians * 180 / PI;
}
constexpr float DEG2RAD(float degrees){
  return (float) degrees * PI / 180;
}
constexpr size_t fVEC_SIZE(std::vector<std::complex<float>> vec){
        return (size_t) 2 * vec.size() * sizeof(float);
    }

}
// clang-format on

// *** === Prototypes === ***
int writeIQToFile(const std::string &filename,
                  const std::vector<std::complex<float>> &iq_data);
// *** ===            === ***

int main(void) {
  std::string outfile = "sine.iq";
  size_t num_samples = 1LL << 22;
  float sig_magnitude = 5.0f;

  /*
   * Let's decide on a symbol mapping
   * We'll go clock wise from the first symbol, 00 in one hot encoding
   *          00 :  1 + 1j
   *          01 : -1 + 1j
   *          11 : -1 - 1j
   *          10 : -1 + 1j
   *  but normalized to magnitude 1.
   */
  size_t numSymbols = pow(2, config::M);
  std::vector<std::complex<float>> symbols(numSymbols);
  float d_angleStep = (float)360 / (numSymbols);
  float d_initAngle = 45;

  for (int e = 0; e < symbols.size();
       e++) { // .size() returns number of elements; .capacity() allocated size.
    symbols[e] =
        std::polar(1.0f, config::DEG2RAD(d_initAngle + e * d_angleStep));
  }

  for (std::complex<float> sym : symbols) {
    printf("%02.2f + 1j*%02.2f\n", sym.real(), sym.imag());
    printf("(r, theta) = (%.2f, %.2f)\n", std::abs(sym),
           config::RAD2DEG(std::arg(sym)));
  }

  // printf("\n\nTesting file output to %s.", outfile);

  // Make a sine wave
  float f = 100.0f;
  std::vector<std::complex<float>> signal(num_samples);
  for (int i = 0; i < num_samples; i++) {
    signal[i] = std::polar(sig_magnitude, (float)2 * PI * f * i * config::dt);
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

  if (written_bytes != config::fVEC_SIZE(iq_data)) {
    fprintf(stderr,
            "Not all samples written to file %s. (Only %d / %d written.)",
            filename, config::fVEC_SIZE(iq_data));
    return 1;
  }
  printf("%x bytes written.", config::fVEC_SIZE(iq_data));

  fclose(fd);

  return 0;
}
