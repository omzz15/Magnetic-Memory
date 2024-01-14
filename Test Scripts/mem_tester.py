import serial
import random
import time

PORT = "/dev/ttyACM2"
BAUD = 115200

con = serial.Serial(PORT, BAUD, timeout=1)

def send(cmd : str):
    con.write(cmd.encode())

def send_int(cmd : int):
    con.write(cmd.to_bytes(1, "little"))

def recive() -> int:
    return int.from_bytes(con.read(), 'little')

def enter_fast_mode():
    send('f')
    time.sleep(0.1)
    con.reset_input_buffer() # delete fast mode prompt
    send_int(3) # send command to verify fast mode (0b00000011)
    res = recive()
    if res != 10:
        raise Exception("Failed to enter fast mode.")
    print("Entered fast mode!")

def write(mad, data: int):
    cmd = 0b00000001 | (mad << 2) # combine base command and mad data
    send_int(cmd)
    send_int(data)
    res = recive()
    if res != 0:
        raise Exception("Failed to write.")

def read(mad):
    cmd = mad << 2 # base command is just 0b00000000
    send_int(cmd)
    res = recive()
    return res

def test(debug = False):
    mem = []
    for i in range(64):
        mem.append(random.randint(0, 255))

    print("Writing data...")
    for i in range(64):
        write(i, mem[i])
        if(debug): print(f"{i} -> {mem[i]}")

    misses = 0
    print("Reading data...")
    for i in range(64):
        res = read(i)
        if res != mem[i]:
            misses += 1
            if (debug): print(f"[-] {i} -> {res} (expected {mem[i]})")
        elif(debug):
            print(f"[+] {i} -> {res}")

    return misses

time.sleep(2) # Wait for arduino to boot
con.reset_input_buffer() # Clear buffer

print("Entering fast mode...")
enter_fast_mode()

print("Testing memory...")
for i in range(10):
    print(f"Test {i+1}")
    misses = test()
    print(f"Misses: {misses}")
    if misses > 0:
        break