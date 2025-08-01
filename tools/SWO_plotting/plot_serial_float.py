import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import struct
from collections import deque
import time
import sys

def create_serial_connection():
    """Create or recreate serial connection"""
    try:
        return serial.Serial('/dev/tty.usbmodem8332403', 921600, timeout=0.001)
    except:
        return None

ser = create_serial_connection()

def plot_data():
    """Real-time plot with connection recovery"""
    global ser  # Fix: declare ser as global
    xlim = 1024
    data = deque(maxlen=xlim)
    fig, ax = plt.subplots()
    line, = ax.plot([], [])
    ax.set_title('STM32 ADC Data')
    ax.set_ylim(-5, 5)
    ax.set_xlim(0, xlim)
    ax.grid(True)
    
    last_data_time = time.time()
    reconnect_count = 0
    
    def animate(i):
        nonlocal last_data_time, reconnect_count  
        global ser  

        try:
            if ser is None:
                ser = create_serial_connection()
                if ser:
                    reconnect_count += 1
                    sys.stdout.write(f"\rReconnected {reconnect_count} times")
                    sys.stdout.flush()
                return line,
            
            # Check if we're getting data
            current_time = time.time()
            if ser.in_waiting >= 4:
                last_data_time = current_time
                
                # Read limited amount to prevent overflow
                bytes_to_read = min(ser.in_waiting, 400)  # Max 100 floats
                bytes_to_read = (bytes_to_read // 4) * 4
                raw_bytes = ser.read(bytes_to_read)
                
                if len(raw_bytes) >= 4:
                    floats = struct.unpack(f'<{len(raw_bytes)//4}f', raw_bytes)
                    for float_val in floats:
                        data.append(float_val)
                        
            # If no data for 2 seconds, reconnect
            elif current_time - last_data_time > 2.0:
                sys.stdout.write(f"\rNo data - reconnecting...")
                sys.stdout.flush()
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
        return line,
    
    ani = animation.FuncAnimation(fig, animate, interval=50, blit=True)
    plt.show()

if __name__ == "__main__":
    try:
        plot_data()
    except KeyboardInterrupt:
        print("\nStopped")
    finally:
        if ser:
            ser.close()