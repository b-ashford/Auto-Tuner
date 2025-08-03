import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

def plot_guitar_filter(iir_taps, num_stages, fs=8000):
    # Convert CMSIS format to scipy SOS
    sos = np.zeros((num_stages, 6))
    for i in range(num_stages):
        start_idx = i * 5
        b0, b1, b2, a1, a2 = iir_taps[start_idx:start_idx + 5]
        sos[i] = [b0, b1, b2, 1.0, a1, a2]
    
    # Calculate and plot frequency response
    w, h = signal.sosfreqz(sos, worN=2048, fs=fs)
    
    plt.figure(figsize=(10, 6))
    plt.semilogx(w, 20 * np.log10(np.maximum(abs(h), 1e-10)))
    plt.title('Filter Frequency Response')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude (dB)')
    plt.grid(True, alpha=0.3)
    
    # Mark guitar frequencies
    guitar_freqs = [82, 110, 147, 196, 247, 330]
    for freq in guitar_freqs:
        plt.axvline(freq, color='g', linestyle=':', alpha=0.7)
    
    plt.xlim(10, fs/2)
    plt.ylim(-60, 10)
    plt.show()


three_stage_taps = [
    0.9, -0.9, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0,  
    0.0, 0.0, 0.0, 0.0, 0.0 
]

# Test them:

plot_guitar_filter(three_stage_taps, 3)