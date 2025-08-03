def check_filter_stability(sos):import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

# Filter specifications
fs = 8000  # Sample rate (Hz)
f_low = 120   # Low cutoff (Hz)
f_high = 2000  # High cutoff (Hz)

print(f"Filter Design for Guitar Tuner")
print(f"Sample Rate: {fs} Hz")
print(f"Bandpass: {f_low} - {f_high} Hz")
print("="*50)

def design_bandpass_filter(fs, f_low, f_high, order=4):
    """Design bandpass filter and return coefficients"""
    nyquist = fs / 2
    low_norm = f_low / nyquist
    high_norm = f_high / nyquist
    
    # Design as Second-Order Sections (SOS) for better numerical stability
    sos = signal.butter(order, [low_norm, high_norm], btype='band', output='sos')
    
    # Convert to cascaded biquads format for CMSIS-DSP
    print(f"\n{order}th Order Bandpass Filter Coefficients:")
    print("Format: [b0, b1, b2, a1, a2] for each biquad stage")
    
    coeffs_flat = []
    for i, section in enumerate(sos):
        b0, b1, b2, a0, a1, a2 = section
        # CMSIS-DSP expects a0=1, so normalize
        b0_norm, b1_norm, b2_norm = b0/a0, b1/a0, b2/a0
        a1_norm, a2_norm = a1/a0, a2/a0
        
        print(f"\nStage {i+1}:")
        print(f"  {b0_norm:.15f}, {b1_norm:.15f}, {b2_norm:.15f}, {a1_norm:.15f}, {a2_norm:.15f}")
        
        coeffs_flat.extend([b0_norm, b1_norm, b2_norm, a1_norm, a2_norm])
    
    # Generate C array format
    print(f"\nC Array Format (for {len(sos)} stages):")
    print(f"#define NUM_IIR_STAGES {len(sos)}")
    print("static float iir_taps[] = {")
    for i in range(0, len(coeffs_flat), 5):
        stage_coeffs = coeffs_flat[i:i+5]
        coeffs_str = ", ".join([f"{c:.15f}" for c in stage_coeffs])
        print(f"    {coeffs_str}{',' if i < len(coeffs_flat)-5 else ''}")
    print("};")
    
    return sos

def plot_filter_response(sos, fs, title="Filter Response"):
    """Plot frequency response"""
    # Calculate frequency response
    w, h = signal.sosfreqz(sos, worN=2048, fs=fs)
    
    # Create the plot
    plt.figure(figsize=(12, 8))
    
    # Magnitude plot
    plt.subplot(2, 1, 1)
    plt.semilogx(w, 20 * np.log10(np.maximum(abs(h), 1e-10)))  # Avoid log(0)
    plt.title(f'{title} - Magnitude Response')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude (dB)')
    plt.grid(True, alpha=0.3)
    plt.axvline(f_low, color='r', linestyle='--', alpha=0.7, label=f'Low cutoff ({f_low} Hz)')
    plt.axvline(f_high, color='r', linestyle='--', alpha=0.7, label=f'High cutoff ({f_high} Hz)')
    plt.axvline(82, color='g', linestyle=':', alpha=0.7, label='Low E (82 Hz)')
    plt.axvline(330, color='g', linestyle=':', alpha=0.7, label='High E (330 Hz)')
    plt.legend()
    plt.xlim(10, fs/2)
    plt.ylim(-80, 10)
    
    # Phase plot
    plt.subplot(2, 1, 2)
    plt.semilogx(w, np.angle(h) * 180/np.pi)
    plt.title(f'{title} - Phase Response')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Phase (degrees)')
    plt.grid(True, alpha=0.3)
    plt.xlim(10, fs/2)
    
    plt.tight_layout()
    plt.show()
    
    return w, h
    """Check filter stability without plotting"""
    poles = []
    for section in sos:
        b, a = section[:3], section[3:]
        section_poles = np.roots(a)
        poles.extend(section_poles)
    
    max_pole_mag = max(abs(pole) for pole in poles)
    print(f"Stability Analysis:")
    print(f"Maximum pole magnitude: {max_pole_mag:.6f}")
    print(f"Filter is {'STABLE' if max_pole_mag < 1.0 else 'UNSTABLE'}")
    
    # Test with impulse to verify
    impulse_response = signal.sosfilt(sos, signal.unit_impulse(50))
    max_impulse = max(abs(impulse_response))
    print(f"Max impulse response: {max_impulse:.6f}")
    if max_impulse > 100:
        print("WARNING: Filter may be unstable!")
    
    return max_pole_mag < 1.0

def test_simple_filters():
    """Test simpler filter options"""
    print("\n" + "="*50)
    print("SIMPLE FILTER OPTIONS FOR COMPARISON")
    print("="*50)
    
    # Option 1: Simple highpass (DC removal only)
    print("\nOption 1: Simple Highpass (50 Hz cutoff)")
    fc_hp = 50
    sos_hp = signal.butter(1, fc_hp/(fs/2), btype='high', output='sos')
    b0, b1, b2, a0, a1, a2 = sos_hp[0]
    print(f"Coefficients: [{b0/a0:.6f}, {b1/a0:.6f}, {b2/a0:.6f}, {a1/a0:.6f}, {a2/a0:.6f}]")
    
    # Option 2: Very gentle bandpass 
    print("\nOption 2: Gentle 1-Stage Bandpass (wide band)")
    f_low_gentle = 60
    f_high_gentle = 3500
    try:
        sos_gentle = signal.butter(1, [f_low_gentle/(fs/2), f_high_gentle/(fs/2)], btype='band', output='sos')
        b0, b1, b2, a0, a1, a2 = sos_gentle[0]
        print(f"Coefficients: [{b0/a0:.6f}, {b1/a0:.6f}, {b2/a0:.6f}, {a1/a0:.6f}, {a2/a0:.6f}]")
    except:
        print("Cannot create 1-stage bandpass with these specs")
        
    # Option 3: 2-stage bandpass
    print("\nOption 3: 2-Stage Bandpass")
    try:
        sos_2stage_simple = signal.butter(2, [f_low/(fs/2), f_high/(fs/2)], btype='band', output='sos')
        for i, section in enumerate(sos_2stage_simple):
            b0, b1, b2, a0, a1, a2 = section
            print(f"Stage {i+1}: [{b0/a0:.6f}, {b1/a0:.6f}, {b2/a0:.6f}, {a1/a0:.6f}, {a2/a0:.6f}]")
    except Exception as e:
        print(f"Error creating 2-stage bandpass: {e}")

# Design and analyze different filter orders
print("Testing different filter complexities...\n")

# Test 1-stage (simple highpass)
print("1-STAGE HIGHPASS (DC removal only):")
fc_hp = 50
sos_1stage = signal.butter(1, fc_hp/(fs/2), btype='high', output='sos')

# Print highpass coefficients manually
b0, b1, b2, a0, a1, a2 = sos_1stage[0]
b0_norm, b1_norm, b2_norm = b0/a0, b1/a0, b2/a0
a1_norm, a2_norm = a1/a0, a2/a0

print(f"1st Order Highpass Filter Coefficients ({fc_hp} Hz cutoff):")
print(f"[{b0_norm:.15f}, {b1_norm:.15f}, {b2_norm:.15f}, {a1_norm:.15f}, {a2_norm:.15f}]")

print(f"\nC Array Format:")
print(f"#define NUM_IIR_STAGES 1")
print("static float iir_taps[] = {")
print(f"    {b0_norm:.15f}, {b1_norm:.15f}, {b2_norm:.15f}, {a1_norm:.15f}, {a2_norm:.15f}")
print("};")


print("\n" + "="*70)

# Test 2-stage bandpass
print("2-STAGE BANDPASS (recommended for real-time):")
sos_2stage = design_bandpass_filter(fs, f_low, f_high, order=2)

print("\n" + "="*70)

# Test 4-stage bandpass
print("4-STAGE BANDPASS (better filtering, more CPU):")
sos_4stage = design_bandpass_filter(fs, f_low, f_high, order=4)

# Plot frequency responses for comparison
print("\nPlotting frequency responses...")

# Plot 1-stage highpass
plot_filter_response(sos_1stage, fs, "1-Stage Highpass (50 Hz)")

# Plot 2-stage bandpass  
plot_filter_response(sos_2stage, fs, "2-Stage Bandpass (80-3000 Hz)")

# Plot 4-stage bandpass
plot_filter_response(sos_4stage, fs, "4-Stage Bandpass (80-3000 Hz)")

# Check stability of the 2-stage filter (recommended)
print("\nChecking 2-stage filter stability...")
check_filter_stability(sos_2stage)

# Show simple alternatives
test_simple_filters()

print(f"\n{'='*70}")
print("RECOMMENDATION FOR REAL-TIME GUITAR TUNING:")
print("Start with 1-stage highpass, then upgrade to 2-stage if needed")
print("4+ stages are probably overkill and may impact real-time performance")