#!/usr/bin/env python3
"""临时测试客户端：连上 10000 端口，发一条消息，打印收到的回显"""
import socket

def main():
    s = socket.create_connection(("127.0.0.1", 10000), timeout=3)
    msg = b"hello from client\n"
    s.sendall(msg)
    data = s.recv(4096)
    print("server echoed:", data)
    s.close()

if __name__ == "__main__":
    main()
