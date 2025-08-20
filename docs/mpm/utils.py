import math
import matplotlib.pyplot as plt
import numpy as np
from typing import Optional, Tuple, Sequence
import time

def create_sinewave(fs, freq, length, amplitude=1, decay_rate=50):
    time = [t * 1/fs for t in range(length)]
    sinewave = [amplitude * math.exp(-decay_rate * t) * math.sin(t*2*math.pi*freq) 
                for t in time]
    return sinewave, time

def create_guitar_signal(fs, freq, length, amplitude=1, decay_rate=20.0):
    time = [t * 1/fs for t in range(length)]
    signal = []
    
    for t in time:
        envelope = amplitude * math.exp(-decay_rate * t)
        harmonic_signal = (
            #1.0 * math.sin(2 * math.pi * freq * t) +        # Fundamental (strongest)
            1 * math.sin(2 * math.pi * freq * 2 * t) +       # 2nd harmonic (prominent)
            0.35 * math.sin(2 * math.pi * freq * 3 * t) +    # 3rd harmonic (reduced)
            0.25 * math.sin(2 * math.pi * freq * 4 * t) +    # 4th harmonic
            0.15 * math.sin(2 * math.pi * freq * 5 * t) +      # 5th harmonic
            0.12 * math.sin(2 * math.pi * freq * 6 * t)        # 6th harmonic (minimal)
        )
        signal.append(envelope * harmonic_signal)
    
    max_val = max(abs(min(signal)), abs(max(signal)))
    if max_val > 0:
        signal = [s / max_val * amplitude for s in signal]
    
    return signal, time

def get_frequency(signal, fs):
    N = len(signal)
    spectrum = np.fft.rfft(signal)
    mags = np.abs(spectrum)
    freqs = np.fft.rfftfreq(N, 1/fs)
    peak_idx = np.argmax(mags)
    return freqs[peak_idx]

def plot_signal(
    signal: Sequence[float],
    time: Sequence[float],
    title: Optional[str] = None,
    xlabel: str = "Time (s)",
    ylabel: str = "Amplitude",
    figsize: Optional[Tuple[float, float]] = None
):
    if figsize:
        plt.figure(figsize=figsize)
    else:
        plt.figure()
    plt.plot(time, signal)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    if title:
        plt.title(title)
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def plot_signal2(
    signal1: Sequence[float],
    signal2: Sequence[float],
    time1: Sequence[float],
    time2: Sequence[float],
    title1: Optional[str] = None,
    title2: Optional[str] = None,
    xlabel: str = "Time (s)",
    ylabel: str = "Amplitude",
    figsize: Optional[Tuple[float, float]] = (12, 4)
):
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=figsize)
    
    # Plot first signal
    ax1.plot(time1, signal1)
    ax1.set_xlabel(xlabel)
    ax1.set_ylabel(ylabel)
    if title1:
        ax1.set_title(title1)
    ax1.grid(True)
    
    # Plot second signal
    ax2.plot(time2, signal2)
    ax2.set_xlabel(xlabel)
    ax2.set_ylabel(ylabel)
    if title2:
        ax2.set_title(title2)
    ax2.grid(True)
    
    plt.tight_layout()
    plt.show()


def NSDF_direct(signal):
   signal = np.array(signal)
   W = len(signal)
   lags = np.arange(W)
   nsdf = np.zeros(W)
   
   for tau in range(W):
       acf_sum = 0
       sosf_sum = 0
       for j in range(W - tau):
           x_j = signal[j]
           x_jτ = signal[j+tau]
           acf_sum += x_j * x_jτ
           sosf_sum += (x_j*x_j +  x_jτ*x_jτ)
       nsdf[tau] = 2*acf_sum / sosf_sum
   return nsdf, lags

def ACF_optimised(signal):
    
    signal = np.array(signal)
    W = len(signal)
    corr = np.zeros(W)

    for tau in range(W):
        a = signal[tau:W]
        b = signal[0: W - tau]
        corr[tau] = np.dot(a, b)

    return corr

def parabolic_interpolation(x_pos, a, b, c):
   a = 20 * np.log10(a)
   b = 20 * np.log10(b)
   c = 20 * np.log10(c)
   
   delta_pos = 0.5 * (a - c) / (a - 2.0 * b + c)
   
   return x_pos + delta_pos

def compare_execution_times(func1, func2, signal, num_runs=10):
   # Time function 1
   times_func1 = []
   for _ in range(num_runs):
       start_time = time.time()
       result1 = func1(signal)
       end_time = time.time()
       times_func1.append(end_time - start_time)
   
   # Time function 2
   times_func2 = []
   for _ in range(num_runs):
       start_time = time.time()
       result2 = func2(signal)
       end_time = time.time()
       times_func2.append(end_time - start_time)
   
   # Calculate averages
   avg_func1 = np.mean(times_func1)
   avg_func2 = np.mean(times_func2)
   
   print(f"{func1.__name__} average time: {avg_func1:.6f} seconds")
   print(f"{func2.__name__} average time: {avg_func2:.6f} seconds")
   print(f"Speedup: {avg_func1 / avg_func2:.2f}x")
   
   return avg_func1, avg_func2

def peak_picking_optimised(signal, threshold, stop_search=15):
    max_peak_val = 0
    max_peak_lag = 0
    zero_crossed = False
    
    for tau in range(stop_search):
        val = signal[tau]
        if not zero_crossed and val < 0:
            zero_crossed = True

        if zero_crossed and val > threshold and val > max_peak_val:
            max_peak_val = val
            max_peak_lag = tau
    return max_peak_lag

