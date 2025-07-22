import serial
import time
import os
import argparse

def read_until(ser, expected_tokens, timeout=10):
    """
    Đọc dữ liệu từ serial đến khi nhận được một trong các từ khoá (OK, ERROR, >), hoặc hết timeout.
    """
    response = b''
    start = time.time()
    while time.time() - start < timeout:
        if ser.in_waiting:
            response += ser.read(ser.in_waiting)
            for token in expected_tokens:
                if token in response:
                    return response
        time.sleep(1)
    return response

def send_cert(ser, alias, path):
    if not os.path.isfile(path):
        print(f"[❌] File không tồn tại: {path}")
        return False

    with open(path, 'rb') as f:
        data = f.read()

    print(f"[→] Gửi {alias} ({len(data)} bytes)...")

    # Gửi AT+CCERTADD
    cmd = f'AT+CCERTDOWN="{alias}",{len(data)}\r\n'
    ser.write(cmd.encode())

    # Đợi phản hồi '>' (prompt), 'OK' hoặc 'ERROR'
    resp = read_until(ser, [b'>', b'OK', b'ERROR'], timeout=5)
    print(f"[⇐] Phản hồi: {resp.decode(errors='ignore')}")

    if b'ERROR' in resp or b'>' not in resp:
        return False

    # Gửi nội dung file
    ser.write(data)

    # Gửi Ctrl+Z
    ser.write(b'\x1A')

    # Đợi phản hồi cuối
    response = read_until(ser, [b'OK', b'ERROR'], timeout=10)
    print(f"[⇐] Phản hồi cuối: {response.decode(errors='ignore')}")

    return b'OK' in response

def list_certs(ser):
    print("📄 Danh sách cert hiện tại:")
    ser.write(b'AT+CCERTLIST\r\n')
    resp = read_until(ser, [b'OK', b'ERROR'], timeout=5)
    print(resp.decode(errors='ignore'))

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

    list_certs(ser)

    if args.ca:
        if not send_cert(ser, "CA", args.ca):
            print("[✘] Gửi CA thất bại, dừng lại.")
            ser.close()
            return

    if args.cert:
        if not send_cert(ser, "CLIENT", args.cert):
            print("[✘] Gửi CLIENT thất bại, dừng lại.")
            ser.close()
            return

    if args.key:
        if not send_cert(ser, "KEY", args.key):
            print("[✘] Gửi KEY thất bại, dừng lại.")
            ser.close()
            return

    time.sleep(5)
    list_certs(ser)
    time.sleep(5)
    ser.close()
    print("[✓] Hoàn tất.")

if __name__ == '__main__':
    main()
