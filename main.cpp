/*
 * main.cpp - temporary playground until things get complicated and we move to
 * src/ https://github.com/temataro/qpsk-cpp Author: temataro 2024-11-01
 *
 * Copyright 2024. SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstdio>
#include <format>
#include <iostream>
#include <string>
#include <vector>

#define PI 3.14159265359f
#define SPS ceil((float)config::SAMP_RATE / config::BAUD_RATE)
/*
 *  Write iq signals into a binary file in complex float 32 format.
 */

// clang-format off
namespace config{
constexpr size_t       M                  =                                2;
constexpr size_t       numTxSym           =                              300;
constexpr size_t       numSymbols         =              0x0001 << config::M;
constexpr float        fc                 =                          2440.0f; // Carrier frequency
constexpr unsigned int SAMP_RATE          =                        1'000'000;
constexpr unsigned int BAUD_RATE          =                              940;
constexpr float        dt                 =      (float) 1/config::SAMP_RATE;
constexpr unsigned int BIT_RATE           =            M * config::BAUD_RATE;
// constexpr unsigned int samplesPerSymbol=                              SPS;

// compile time functions
constexpr float        RAD2DEG(float radians)         { return (float) radians * 180 / PI; }
constexpr float        DEG2RAD(float degrees)         { return (float) degrees * PI / 180; }
constexpr size_t       fVEC_SIZE(std::vector<std::complex<float>> vec){ return (size_t) 2 * vec.size() * sizeof(float); }
}
// clang-format on

// *** === Prototypes === ***
int writeIQToFile(const std::string &filename,
                  const std::vector<std::complex<float>> &iq_data);
void mapSymToIQ(std::vector<std::vector<std::complex<float>>> &symbolMap,
                const std::vector<std::complex<float>> &symbols);
int iqGenerator(const std::vector<std::complex<float>> &symbols,
                const std::vector<std::vector<std::complex<float>>> &symbolMap);
// *** ===            === ***

int main(void) {
  std::vector<std::complex<float>> symbols(config::numSymbols);
  float d_angleStep = (float)360 / (config::numSymbols);
  float d_initAngle = 45;

  // Just put all the constellation in one cirle for now...
  for (int e = 0; e < symbols.size();
       e++) { // .size() returns number of elements; .capacity() allocated size.
    symbols[e] =
        std::polar(1.0f, config::DEG2RAD(d_initAngle + e * d_angleStep));
  }

  for (std::complex<float> sym : symbols) {
    printf("%02.2f + 1j*%02.2f  \t<==>  \t", sym.real(), sym.imag());
    printf("(r, theta) = (%.2f, %.2f)\n", std::abs(sym),
           config::RAD2DEG(std::arg(sym)));
  }

  // Initialize our symbolMap vector of vectors with zeros
  // Shape of array: (numSymbols, samplesPerSymbol)
  std::vector<std::vector<std::complex<float>>> symbolMap(
      config::numSymbols,                     // =4
      std::vector<std::complex<float>>(SPS)); // =1064

  mapSymToIQ(symbolMap, symbols);

  // Main loop
  char byteInput = 97;
  size_t bitsAccum = 0;
  size_t rxBit, idx = 0x00; // For now let's just say the bit value of the
  // symbol is its index in the symbols vector. Keep this an explicit decision
  // with idx.
  std::vector<std::complex<float>> iqSym(SPS);

  while (std::cin.get(byteInput)) {
    bool dataBit = byteInput & 0x01;
    // Accumulate bits until you get to the size you want,
    // then get the index of the sym and proceed with the processing...
    rxBit = ((rxBit << 1) ^ dataBit) & ((0x1 << config::M) - 1);
    bitsAccum++;
    if (bitsAccum == config::M) {
      idx = rxBit;
      std::cout << "Read a bit! It's " << idx << std::endl;
      iqSym = symbolMap[idx];
      bitsAccum = 0;
      rxBit = 0;
      idx = 0;
      writeIQToFile("./data/qpsk.iq", iqSym);
    }
  }

  return 0;
}

int writeIQToFile(const std::string &filename,
                  const std::vector<std::complex<float>> &iq_data) {
  FILE *fd;

  fd = fopen(filename.c_str(), "a+b"); // We want to accumulate a large binary
  if (!fd) {
    fprintf(stderr, "%s could not be opened!", filename.c_str());
    return 1;
  }

  // Write to file
  size_t written_bytes = 0;
  for (const std::complex<float> &samp : iq_data) {
    float i = samp.real();
    float q = samp.imag();
    fwrite(&i, sizeof(float), 1, fd);
    fwrite(&q, sizeof(float), 1, fd);
    // Writing to a cf32 format means spending 4 bytes (32 bits) on I and 4
    // bytes on Q So we spend 8 bytes per sample
    written_bytes += 2 * sizeof(float);
  }

  if (written_bytes != config::fVEC_SIZE(iq_data)) {
    fprintf(stderr,
            "Not all samples written to file %s. (Only %zu / %zu written.)\n",
            filename.c_str(), written_bytes, config::fVEC_SIZE(iq_data));
    return 1;
  }
  printf("%d bytes written.\n", (int)config::fVEC_SIZE(iq_data));

  fclose(fd);

  return 0;
}

void mapSymToIQ(std::vector<std::vector<std::complex<float>>> &symbolMap,
                const std::vector<std::complex<float>> &symbols) {
  /*
   * One time only, make a symbol to IQ map with SPS
   * samples of each symbol.
   */

  printf("[NOTE] Requested baud rate: %d for a samplerate of: %d\n"
         "Can only do baud rate : % .3f.\n ",
         config::BAUD_RATE, config::SAMP_RATE, (float)config::SAMP_RATE / SPS);

  float mag = 1;
  float phi = config::DEG2RAD(0);

  // Step 1: Make a template sine wave to manipulate
  std::vector<std::complex<float>> sineTemplate(SPS);

  for (int i = 0; i < SPS; i++) {
    sineTemplate[i] =
        std::polar(mag, (float)2 * PI * config::fc * i * config::dt + phi);
  }

  assert(symbolMap.size() == symbols.size() &&
         "[DEBUG] The number of symbols is not equal to the number of IQ maps. "
         "BAD!");

  printf("[DEBUG] Good. The number of symbols you have is equal to the number "
         "of IQ maps you have to make.\n\n");

  for (int s = 0; s < config::numSymbols; s++) {
    mag = std::abs(symbols[s]);
    phi = std::arg(symbols[s]);
    // Transform the values in the vector symbolMap[s] with the values in
    // sineTemplate with our scaling (implemented as a lambda) applied to each
    // of them. Clarity over consciseness so I can debug this in the future!
    //
    // NOTE: DSP-wise, multiplying each element of the signal vector with a
    // phase and magnitude term will shift the magnitude and angle of the sine
    // wave by that amount.

    auto lambda_ScaleAndShift =
        [=](std::complex<float> &sample) -> std::complex<float> {
      return sample * std::polar(mag, phi);
    };

    // std::transform(input_vector.begin(), input_vector.end(),
    // output_vector.begin(), lambda_func_to_apply);
    std::transform(sineTemplate.begin(), sineTemplate.end(),
                   symbolMap[s].begin(), lambda_ScaleAndShift);

    // This is a horrible way of doing string formatting, I'll be using C++20
    // instead (it has std::format) std::ostringstream outputFilename;
    // outputFilename << "./data/sine_sym_" << s << ".iq";
    // TEST: Using GNU Radio to see if the signals form the right constellation
    std::string outputFilename = std::format("./data/{}-ary_Map/sine_{:02d}.iq",
                                             (int)pow(2, config::M), s);
    printf("Writing output to file: %s\n", outputFilename.c_str());
    writeIQToFile(outputFilename, symbolMap[s]); // Just for testing
  }
}

int iqGenerator(
    const std::vector<std::complex<float>> &symbols,
    const std::vector<std::vector<std::complex<float>>> &symbolMap) {
  /*
   * Will use the sample rate and baud rate from the config to determine how
   * many samples each symbol would need to be represented in IQ.
   * A large preallocated vector will hold these IQ values mapped from symbols.
   */

  for (const std::complex<float> &sym : symbols) {
    printf("NotImplementedError %f", std::abs(sym));
  }

  return 0;
}
