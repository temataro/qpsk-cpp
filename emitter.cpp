/*
 * emitter.cpp - Generate an infinite bitstream out to stdout at a set rate.
 * src/ https://github.com/temataro/qpsk-cpp Author: temataro 2024-11-02
 *
 * Copyright 2024. SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

// In nanosecond precision (=> max possible speed = 1GBps), emit
// a byte to stdout representing the bit we want to send.
// 0xff is 1 and 0x00 is 0.
//
// Our emit rate should be equal to our baud rate input for ./main
// Which means it will be R_baud = bytesPerSecond / M (=bits per symbol)
// clang-format off
#define M                                 2
#define BAUD_RATE                       940
#define EMIT_RATE             BAUD_RATE / M
#define DELAY_NS      (int) 1e9 / EMIT_RATE
// clang-format on

// We'll be outputting `EMIT_RATE` number of bytes to stdout every second.

int main(void) {
  char counter = 0;
  std::cout << "[NOTE] Desired baud rate    : " << BAUD_RATE << std::endl
            << "Actual obtainable baud rate : " << (float)1e9 / (DELAY_NS)
            << std::endl;

  // TODO: Make a cli argument on what pattern of bitstream to generate (all
  // ones, alternating, random...), also one to control the emit rate.
  for (;;) {
    std::this_thread::sleep_for(std::chrono::nanoseconds(DELAY_NS));
    // std::cout << counter;
    if (counter % 2)
      std::cout << 0x00;
    else
      std::cout << 0xff;

    // TODO: Write bytes themselves and not chars, though is there really a
    // difference? char bit = 0x00;
    // // std::cout.write(&bit, 1);
    // fwrite(&bit, 1, 1, stdout);
    // fflush(stdout);
    counter++;
  }
  return 0;
}
