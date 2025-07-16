import serial
import time

# Thông số UART của module
SERIAL_PORT = 'COM9'      # Thay bằng cổng COM của bạn
BAUDRATE = 115200

def send_at(ser, cmd, wait=1):
    ser.write((cmd + '\r\n').encode())
    time.sleep(wait)
    resp = ser.read_all().decode(errors='ignore')
    print(resp)
    return resp

def upload_file(ser, filename, module_filename):
    with open(filename, 'rb') as f:
        data = f.read()
    size = len(data)
    print(f"Uploading {filename} as {module_filename}, size: {size} bytes")

    # Gửi lệnh AT chuẩn bị ghi file
    send_at(ser, f'AT+CFSWFILE="{module_filename}",{size},60,0', wait=2)
    send_at(ser, 'AT+CFSGFLS', wait=2)
    time.sleep(1)
    # Gửi nội dung file
    ser.write(data)
    time.sleep(2)
    resp = ser.read_all().decode(errors='ignore')
    print(resp)

if __name__ == "__main__":
    ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=2)
    time.sleep(2)

    # Upload CA
    upload_file(ser, 'AmazonRootCA1.pem', 'AmazonRootCA1.pem')
    # Upload device certificate
    #upload_file(ser, '44739dc45599855a25eb65c33dae7ce742b7555096d25387333fbda946a437be-certificate.pem.crt', 'device-cert.pem')
    upload_file(ser, 'device-cert.pem', 'device-cert.pem')
    # Upload private key
    #upload_file(ser, 'p44739dc45599855a25eb65c33dae7ce742b7555096d25387333fbda946a437be-private.pem.key', 'private-key.pem')
    upload_file(ser, 'private-key.pem', 'private-key.pem')

    ser.close()