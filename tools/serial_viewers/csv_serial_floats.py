import serial
import struct
import csv
import time

ser = serial.Serial('/dev/tty.usbmodem8332403', 921600, timeout=0.001)

def raw_data_to_csv(filename="output.csv"):
    """Read binary float data from serial and save to CSV"""
    print(f"Logging to {filename} - Press Ctrl+C to stop")

    with open(filename, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        try:
            while True:
                if ser.in_waiting >= 4:  # at least 1 float
                    bytes_to_read = (ser.in_waiting // 4) * 4
                    raw_bytes = ser.read(bytes_to_read)
                    
                    floats = struct.unpack(f"<{len(raw_bytes)//4}f", raw_bytes)
                    
                    # write each group of floats as one row
                    writer.writerow(floats)
                    
        except KeyboardInterrupt:
            print("\nStopped logging")

if __name__ == "__main__":
    raw_data_to_csv("serial_log.csv")
    ser.close()