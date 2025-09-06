class BitStreamReader {
public:
  BitStreamReader(std::istream &input) : inputStream(input) {}
  bool readBits(int &bits, int numBits);

private:
  std::istream &inputStream;
};

class QPSKSymbolMapper {
public:
  std::complex<float> mapBitsToSymbol(int bits);
};

class IQSignalGenerator {
public:
  std::vector<std::complex<float>> generateIQ(std::complex<float> symbol,
                                              int samplesPerSymbol);
};

class PulseShapingFilter {
public:
  PulseShapingFilter(float rollOffFactor, int sampleRate);
  std::vector<std::complex<float>>
  apply(const std::vector<std::complex<float>> &iqSamples);

private:
  std::vector<float> filterCoefficients;
};

class OutputHandler {
public:
  void writeToFile(const std::vector<std::complex<float>> &iqSamples);
};

int main() {
  BitStreamReader bitReader(std::cin);
  QPSKSymbolMapper symbolMapper;
  IQSignalGenerator iqGenerator;
  PulseShapingFilter rrcFilter(0.35,
                               1000000); // Example roll-off and sample rate
  OutputHandler outputHandler;

  while (/* Condition for reading */) {
    int bits;
    if (bitReader.readBits(bits, 2)) {
      auto symbol = symbolMapper.mapBitsToSymbol(bits);
      auto iqSamples = iqGenerator.generateIQ(symbol, 10);
      auto filteredSamples = rrcFilter.apply(iqSamples);
      outputHandler.writeToFile(filteredSamples);
    }
  }
  return 0;
}
