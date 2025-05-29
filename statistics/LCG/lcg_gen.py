import sys

def generate_lcg(seed, a, c, m, total_bytes):
    x = seed
    for _ in range(total_bytes // 4):
        x = (a * x + c) % m
        sys.stdout.buffer.write(x.to_bytes(4, byteorder='little'))
    remaining = total_bytes % 4
    if remaining:
        x = (a * x + c) % m
        sys.stdout.buffer.write(x.to_bytes(4, byteorder='little')[:remaining])

# Параметры LCG
seed = 42
a = 1664525
c = 1013904223
m = 2**32

# 50 МБ
total_bytes = 50 * 1024 * 1024

generate_lcg(seed, a, c, m, total_bytes)
