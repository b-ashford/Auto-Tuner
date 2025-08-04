import serial
import struct
import sys
import time

ser = serial.Serial('/dev/tty.usbmodem8332403', 921600, timeout=0.001)

def raw_data():
    """Print raw binary float data"""
    print("Binary float mode - Press Ctrl+C to stop")
    try:
        while True:
            if ser.in_waiting >= 4:  # Need at least 4 bytes for one float
                # Read multiples of 4 bytes
                bytes_to_read = (ser.in_waiting // 4) * 4
                raw_data = ser.read(bytes_to_read)
                
                # Unpack as little-endian floats
                floats = struct.unpack(f'<{len(raw_data)//4}f', raw_data)
                
                # Print the float values
                #line = "\t".join(f"{v:7.1f}" for v in floats)
                #print(line, end='\r')
                #time.sleep(0.3)
                #sys.stdout.flush()
                for val in floats:
                    print(f"{val:.3f}")
                    
    except KeyboardInterrupt:
        print("\nStopped")

if __name__ == "__main__":
    raw_data()     
    ser.close()