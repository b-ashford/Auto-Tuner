import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import struct
from collections import deque
import time
import sys
import numpy as np

def create_serial_connection():
   """Create or recreate serial connection"""
   try:
       return serial.Serial('/dev/tty.usbmodem8332403', 921600, timeout=0.001)
   except:
       return None

ser = create_serial_connection()

def plot_data():
   """Real-time plot for uint16 ADC data with FFT frequency display"""
   global ser
   xlim = 2 * 1024
   data = deque(maxlen=xlim)
   fig, ax = plt.subplots()
   line, = ax.plot([], [])
   ax.set_title('STM32 ADC Data - uint16 (Press R to restart)')
   ax.set_ylim(-500, 4500)  # 12-bit ADC range
   ax.set_xlim(0, xlim)
   ax.grid(True)
   
   # Add text for dominant frequency display
   freq_text = ax.text(0.02, 0.98, '', transform=ax.transAxes, 
                      fontsize=12, verticalalignment='top',
                      bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))
   
   last_data_time = time.time()
   reconnect_count = 0
   paused = False
   sample_rate = 8000  # 8kHz sampling rate
   
   def on_key(event):
       nonlocal paused, data, last_data_time
       global ser
       
       if event.key == 'r':
           # HARD RESET: clear everything aggressively
           data.clear()
           if ser:
               ser.close()
           
           # Wait a moment for port to fully close
           time.sleep(0.1)
           
           # Create new connection
           ser = create_serial_connection()
           
           if ser:
               # Flush all buffers aggressively
               ser.reset_input_buffer()
               ser.reset_output_buffer()
               
               # Read and discard any remaining data
               time.sleep(0.05)  # Let any buffered data arrive
               if ser.in_waiting > 0:
                   ser.read(ser.in_waiting)  # Discard it all
               
               # Flush again to be sure
               ser.reset_input_buffer()
           
           last_data_time = time.time()
           paused = False
           print("HARD RESET - all buffers flushed")
           
       elif event.key == ' ':
           # Pause/unpause
           paused = not paused
           print("Paused" if paused else "Resumed")
   
   fig.canvas.mpl_connect('key_press_event', on_key)
   
   def animate(i):
       nonlocal last_data_time, reconnect_count
       global ser
       
       if paused:
           return line,

       try:
           if ser is None:
               ser = create_serial_connection()
               if ser:
                   reconnect_count += 1
                   sys.stdout.write(f"\rReconnected {reconnect_count} times")
                   sys.stdout.flush()
               return line,
           
           current_time = time.time()
           if ser.in_waiting >= 2:  # Need at least 2 bytes for one uint16
               last_data_time = current_time
               
               bytes_to_read = min(ser.in_waiting, 400)
               bytes_to_read = (bytes_to_read // 2) * 2  # Ensure even number of bytes
               raw_bytes = ser.read(bytes_to_read)
               
               if len(raw_bytes) >= 2:
                   uint16s = struct.unpack(f'<{len(raw_bytes)//2}H', raw_bytes)  # 'H' = uint16
                   for uint16_val in uint16s:
                       data.append(uint16_val)
                       
           elif current_time - last_data_time > 2.0:
               sys.stdout.write(f"\rNo data - reconnecting...")
               sys.stdout.flush()
               if ser:
                   ser.close()
               ser = None
                       
       except Exception as e:
           sys.stdout.write(f"\rError: {e} - reconnecting...")
           sys.stdout.flush()
           if ser:
               ser.close()
           ser = None
       
       if data:
           line.set_data(range(len(data)), data)
           
           # Calculate dominant frequency when we have enough data
           if len(data) >= 512:  # Need enough samples for good FFT
               try:
                   # Remove DC offset and apply window
                   signal = np.array(data) - np.mean(data)
                   windowed = signal * np.hanning(len(signal))
                   
                   # Calculate FFT
                   fft = np.fft.fft(windowed)
                   freqs = np.fft.fftfreq(len(fft), 1/sample_rate)
                   
                   # Only look at positive frequencies up to Nyquist
                   positive_freqs = freqs[:len(freqs)//2]
                   magnitude = np.abs(fft[:len(fft)//2])
                   
                   # Find dominant frequency (ignore DC component)
                   if len(magnitude) > 1:
                       dominant_idx = np.argmax(magnitude[1:]) + 1
                       dominant_freq = positive_freqs[dominant_idx]
                       freq_text.set_text(f'Freq: {dominant_freq:.1f} Hz\nSamples: {len(data)}')
                   else:
                       freq_text.set_text('Calculating...')
               except Exception as e:
                   freq_text.set_text(f'FFT Error: {str(e)}')
           else:
               freq_text.set_text(f'Collecting data... ({len(data)}/512)')
               
       return line, freq_text
   
   ani = animation.FuncAnimation(fig, animate, interval=50, blit=False)
   plt.show()

if __name__ == "__main__":
   try:
       plot_data()
   except KeyboardInterrupt:
       print("\nStopped")
   finally:
       if ser:
           ser.close()