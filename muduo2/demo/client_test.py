#!/usr/bin/env python3
"""muduo2 demo 压力测试：并发 N 个连接，每个发送一行并回显。"""
import socket, threading, time, sys

HOST, PORT = "127.0.0.1", 10000
N = int(sys.argv[1]) if len(sys.argv) > 1 else 200

ok = 0
err = 0

def one(idx):
    global ok, err
    try:
        s = socket.create_connection((HOST, PORT), timeout=5)
        msg = f"hello-{idx}\n".encode()
        s.sendall(msg)
        data = b""
        while len(data) < len(msg):
            chunk = s.recv(1024)
            if not chunk:
                break
            data += chunk
        if data == msg:
            ok += 1
        else:
            print(f"[{idx}] MISMATCH: {data!r} != {msg!r}")
            err += 1
        s.close()
    except Exception as e:
        print(f"[{idx}] ERROR: {e}")
        err += 1

t0 = time.time()
threads = [threading.Thread(target=one, args=(i,)) for i in range(N)]
for t in threads: t.start()
for t in threads: t.join()
print(f"done in {time.time()-t0:.2f}s  N={N}  ok={ok}  err={err}")
