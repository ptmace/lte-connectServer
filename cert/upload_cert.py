import serial
import time
import os
import argparse

def send_cert(ser, alias, path):
    if not os.path.isfile(path):
        print(f"[❌] File không tồn tại: {path}")
        return False

    with open(path, 'rb') as f:
        data = f.read()

    print(f"[→] Gửi {alias} ({len(data)} bytes)...")

    # 1. Gửi lệnh AT+CCERTADD
    cmd = f'AT+CCERTADD="{alias}",{len(data)}\r\n'
    ser.write(cmd.encode())
    time.sleep(1)

    # 2. Gửi nội dung file PEM
    ser.write(data)
    time.sleep(0.2)

    # 3. Gửi ký tự kết thúc (Ctrl+Z)
    ser.write(b'\x1A')

    # 4. Đọc phản hồi từ module
    response = b''
    start = time.time()
    while time.time() - start < 10:
        if ser.in_waiting:
            response += ser.read(ser.in_waiting)
            if b'OK' in response or b'ERROR' in response:
                break

    print(response.decode(errors='ignore'))
    return b'OK' in response

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', required=True, help='Serial port (COMx hoặc /dev/ttyUSBx)')
    parser.add_argument('--baud', type=int, default=115200)
    parser.add_argument('--ca')
    parser.add_argument('--cert')
    parser.add_argument('--key')
    args = parser.parse_args()

    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
        time.sleep(2)
    except Exception as e:
        print(f"[!] Không mở được cổng: {e}")
        return

    if args.ca:
        send_cert(ser, "CA", args.ca)
    if args.cert:
        send_cert(ser, "CLIENT", args.cert)
    if args.key:
        send_cert(ser, "KEY", args.key)

    ser.close()
    print("[✓] Hoàn tất.")

if __name__ == '__main__':
    main()