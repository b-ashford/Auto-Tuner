#!/usr/bin/env python3
import serial, struct, time, csv
import matplotlib.pyplot as plt
from datetime import datetime
from pathlib import Path

PORT = '/dev/tty.usbmodem8332403'
BAUD = 921600
TIMEOUT = 0.001

def collect_and_plot(duration_sec=60, out_dir=Path(".")):
    out_dir = Path(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    stamp = datetime.now().strftime("%Y%m%d-%H%M%S")
    csv_file = out_dir / f"freq_response_{stamp}.csv"
    png_file = out_dir / f"freq_response_{stamp}.png"

    ser = serial.Serial(PORT, BAUD, timeout=TIMEOUT)
    buf = bytearray()
    samples = []  # list of (freq, amp)

    t0 = time.time()
    try:
        while time.time() - t0 < duration_sec:
            n = ser.in_waiting
            if n:
                buf += ser.read(n)
                usable = (len(buf) // 8) * 8  # 2 floats per sample
                if usable:
                    chunk = buf[:usable]
                    del buf[:usable]
                    floats = struct.unpack('<' + 'f'*(usable//4), chunk)
                    it = iter(floats)
                    for f, a in zip(it, it):
                        samples.append((float(f), float(a)))
            else:
                time.sleep(0.001)
    finally:
        try: ser.close()
        except: pass

    if not samples:
        print("No data captured.")
        return

    samples.sort(key=lambda x: x[0])
    freqs = [f for f, _ in samples]
    amps  = [a for _, a in samples]

    # Write CSV
    with csv_file.open("w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["frequency_hz", "amplitude"])
        w.writerows(samples)

    # Plot and save
    plt.figure()
    plt.plot(freqs, amps, marker='.', linestyle='-')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Amplitude')
    plt.title(f'Frequency Response ({len(samples)} samples, {duration_sec}s)')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(png_file, dpi=150)
    plt.show()

    print(f"Saved: {csv_file}")
    print(f"Saved: {png_file}")

if __name__ == "__main__":
    collect_and_plot(60)