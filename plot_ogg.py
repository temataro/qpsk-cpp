#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.12"
# dependencies = [
#     "librosa",
#     "matplotlib",
#     "pyqt6",
# ]
# ///

# Adding the shebang allows you to not even think about the right uv command
# anymore!

# Adding dependencies in uv is as easy as saying uv add --script plot_ogg.py
# 'librosa' 'matplotlib' 'pyqt6' And you can then just run the thing with no
# care in the world with ./plot_ogg.py <input> without having to install a
# single dependency

import librosa
import librosa.display
import matplotlib.pyplot as plt
import sys

N = 100_000
def plot_ogg(filename):
    # Load audio (librosa automatically handles .ogg)
    y, sr = librosa.load(filename, sr=None)  # sr=None keeps original sampling rate

    # Create a time axis in seconds
    time = librosa.times_like(y, sr=sr)

    print(f"{len(y)=}")

    # Plot waveform
    plt.figure(figsize=(12, 4))
    plt.plot(time[:N], y[:N], label="Waveform")
    plt.xlabel("Time (s)")
    plt.ylabel("Amplitude")
    plt.title(f"Waveform of {filename}")
    plt.legend()
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python plot_ogg.py <file.ogg>")
    else:
        plot_ogg(sys.argv[1])

