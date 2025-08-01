import serial

ser = serial.Serial('/dev/tty.usbmodem8332403', 921600, timeout=0.001)

def print_ascii():
   """Print ASCII text data"""
   print("ASCII text mode - Press Ctrl+C to stop")
   try:
       while True:
           if ser.in_waiting > 0:
               # Read available text data
               text_data = ser.read(ser.in_waiting).decode('utf-8', errors='ignore')
               

               print(text_data, end='')  
                   
   except KeyboardInterrupt:
       print("\nStopped")

if __name__ == "__main__":
   print_ascii()     
   ser.close()