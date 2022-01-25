#!/usr/bin/env python3
import serial
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("device", help="serial device to use")
parser.add_argument("-b", "--baud-rate", type=int,
                    default=115200, help="baud rate")
args = parser.parse_args()


def read_assert(expected: str, ser: serial.Serial) -> bool:
    resp = str(ser.readline(), "utf-8").strip()
    if resp != expected.strip():
        print("read_assert failed:")
        print(f"\texpected: {expected}")
        print(f"\tactual: {resp}")
        return False
    return True


def force_write(message: bytes, ser: serial.Serial):
    while len(message) != 0:
        written = ser.write(message)
        message = message[written:]
    ser.flush()


def load(file: str, ser: serial.Serial) -> bool:
    with open(file, "rb") as f:
        data = f.read()

    message = bytes(f"l {len(data)}\n", "utf-8") + data
    force_write(message, ser)
    return read_assert("l ok", ser)


def run(ser: serial.Serial) -> bool:
    message = b'r'
    force_write(message, ser)
    return read_assert("r ok", ser)


def handle(cmd: list[str], ser: serial.Serial) -> bool:
    res = True

    try:
        if cmd[0] == "load" and len(cmd) == 2:
            res = load(cmd[1], ser)
        elif cmd[0] == "run" and len(cmd) == 1:
            res = run(ser)
        elif cmd[0] == "exit" and len(cmd) == 1:
            return True
        else:
            print(f"invalid command: {cmd}")
    except Exception as e:
        print(e)
        res = False

    x = False
    if not res:
        print("Somethig went wrong. Exit? (y/N)")
        resp = input()
        if len(resp) > 0:
            x = resp[0].lower() == 'y'

    return x


with serial.Serial(args.device, args.baud_rate) as ser:
    print("> ", end="", flush=True)
    while line := input():
        if handle(line.split(), ser):
            break
        print("> ", end="", flush=True)
