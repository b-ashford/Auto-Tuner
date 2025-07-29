import serial
import matplotlib.pyplot as plt
from collections import deque

# find the port stlink is outputting too: ls /dev/tty.usb*
ser = serial.Serial('/dev/tty.usbmodem8332403', 921600, timeout=0.001)

def raw_data():
    """Print raw data to terminal"""
    print("Raw data mode - Press Ctrl+C to stop")
    try:
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode().strip()
                if line:
                    print(line)
    except KeyboardInterrupt:
        print("\nStopped")

def plot_data():
    """Fast real-time plot using animation"""
    import matplotlib.animation as animation
    xlim = 500
    data = deque(maxlen=xlim)
    fig, ax = plt.subplots()
    line, = ax.plot([], [])
    ax.set_title('STM32 Fast Data')
    ax.set_ylim(-150, 150)
    ax.set_xlim(0, xlim)
    ax.grid(True)
    
    def animate(i):
        # Process multiple samples per frame for speed
        for _ in range(10):
            if ser.in_waiting > 0:
                try:
                    line_data = ser.readline().decode().strip()
                    if ',' in line_data:
                        timestamp, value = line_data.split(',')
                        data.append(int(value))
                except:
                    pass
        
        if data:
            line.set_data(range(len(data)), data)
        return line,
    
    # Fast animation - 10ms interval
    ani = animation.FuncAnimation(fig, animate, interval=10, blit=True)
    plt.show()

if __name__ == "__main__":
    # Comment out what you don't want:
    
    # raw_data()     # Raw terminal output
    plot_data()      # Fast plotting
    
    ser.close()