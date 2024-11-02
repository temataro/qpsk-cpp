# QAM/OFDM Implementation Cpp

According to jippity 4.0, this would be the best way to structure this project:

```
  ./qpsk_cpp/
  ├── include/
  │   ├── bit_stream_reader.h      // Header for BitStreamReader
  │   ├── symbol_mapper.h          // Header for QPSKSymbolMapper
  │   ├── iq_signal_generator.h    // Header for IQSignalGenerator
  │   ├── pulse_shaping_filter.h   // Header for PulseShapingFilter
  │   ├── output_handler.h         // Header for OutputHandler
  │   └── config.h                 // Header for configuration constants
  ├── src/
  │   ├── bit_stream_reader.cpp    // Implementation of BitStreamReader
  │   ├── symbol_mapper.cpp        // Implementation of QPSKSymbolMapper
  │   ├── iq_signal_generator.cpp  // Implementation of IQSignalGenerator
  │   ├── pulse_shaping_filter.cpp // Implementation of PulseShapingFilter
  │   ├── output_handler.cpp       // Implementation of OutputHandler
  │   └── main.cpp                 // Main entry point
  ├── tests/
  │   ├── test_bit_stream_reader.cpp      // Unit tests for BitStreamReader
  │   ├── test_symbol_mapper.cpp          // Unit tests for SymbolMapper
  │   ├── test_iq_signal_generator.cpp    // Unit tests for IQSignalGenerator
  │   ├── test_pulse_shaping_filter.cpp   // Unit tests for PulseShapingFilter
  │   └── test_output_handler.cpp         // Unit tests for OutputHandler
  ├── data/
  │   └── sample_output.dat        // Placeholder for generated output files
  └── Makefile                     // Makefile to build the project
```

--- 
This project will be broken down into the five following main sections:
1) Handling bitstream inputs ccc
2) Symbol to IQ contstellation mapping
3) Generation of IQ signals from constellation
4) Applying Tx intersymbol interference deterrents and channel effects
5) Writing/appending to a file buffer.
6) What does the data layer you transmit actually look like?
  Look into forward error correction; packets and what not.
7) OFDM with pilot symbols and the whole shebang.
 
The goal is to follow a tracer bullet approach and implement a bare bones version of each step before
moving on to making it prettier.

### Current progress
  **{ [ . ] = WIP; [x] = DONE}**

    [ . ] Stage 1

    [ . ] Stage 2

    [ . ] Stage 3

    [ . ] Stage 4

    [50%] Stage 5
    
    [ . ] Stage 6

    [ . ] Stage 7
