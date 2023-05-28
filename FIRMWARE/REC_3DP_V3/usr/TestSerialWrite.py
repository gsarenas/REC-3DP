import serial
import time

# Define the serial port and baud rate (ensure it matches the Arduino settings)
port = 'COM5'  # Replace with your Arduino's serial port
baud_rate = 115200

# Initialize the serial connection
ser = serial.Serial(port, baud_rate, timeout=1)
time.sleep(2)
print(ser.readline().decode('utf-8'))

while True:
    i = input('Input: ').strip()
    if i == 'Done':
        ser.close()
        time.sleep(1)
        break
    ser.write(i.encode())
    time.sleep(1)
    print(ser.readline().decode('utf-8'))
