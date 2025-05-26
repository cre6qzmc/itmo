# mt_gen.py
import random
import struct
import sys

random.seed()  # по умолчанию — текущее время

for _ in range(12500000):
    r = random.getrandbits(32)
    sys.stdout.buffer.write(struct.pack("I", r))
