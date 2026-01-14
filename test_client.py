#!/usr/bin/env python3
import socket
import json
import time

def test_identify():
    print("=== Testing IDENTIFY Protocol ===\n")

    # Connect to server
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('localhost', 8080))
    print("✓ Connected to server")

    # Send IDENTIFY message
    identify_msg = {"type": "IDENTIFY", "username": "Kimberly"}
    sock.send(json.dumps(identify_msg).encode() + b'\n')
    print(f"→ Sent: {json.dumps(identify_msg)}")

    # Receive response
    time.sleep(0.5)
    response = sock.recv(4096).decode()
    print(f"← Received: {response}")

    # Parse and display
    try:
        data = json.loads(response)
        print(f"\n✓ Response parsed successfully:")
        print(f"  Type: {data.get('type')}")
        print(f"  Operation: {data.get('operation')}")
        print(f"  Result: {data.get('result')}")
        print(f"  Extra: {data.get('extra')}")
    except:
        print("✗ Failed to parse response as JSON")

    sock.close()
    print("\n✓ Connection closed")

def test_duplicate_username():
    print("\n=== Testing Duplicate Username ===\n")

    # First client
    sock1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock1.connect(('localhost', 8080))
    identify_msg = {"type": "IDENTIFY", "username": "Alice"}
    sock1.send(json.dumps(identify_msg).encode() + b'\n')
    response1 = sock1.recv(4096).decode()
    print(f"Client 1 → {json.dumps(identify_msg)}")
    print(f"Client 1 ← {response1}")

    time.sleep(0.5)

    # Second client with same username
    sock2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock2.connect(('localhost', 8080))
    sock2.send(json.dumps(identify_msg).encode() + b'\n')
    response2 = sock2.recv(4096).decode()
    print(f"Client 2 → {json.dumps(identify_msg)}")
    print(f"Client 2 ← {response2}")

    sock1.close()
    sock2.close()
    print("\n✓ Both connections closed")

def test_new_user_broadcast():
    print("\n=== Testing NEW_USER Broadcast ===\n")

    # First client connects
    sock1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock1.connect(('localhost', 8080))
    msg1 = {"type": "IDENTIFY", "username": "Bob"}
    sock1.send(json.dumps(msg1).encode() + b'\n')
    response1 = sock1.recv(4096).decode()
    print(f"Bob → {json.dumps(msg1)}")
    print(f"Bob ← {response1}")

    time.sleep(0.5)

    # Second client connects
    sock2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock2.connect(('localhost', 8080))
    msg2 = {"type": "IDENTIFY", "username": "Carol"}
    sock2.send(json.dumps(msg2).encode() + b'\n')
    response2 = sock2.recv(4096).decode()
    print(f"\nCarol → {json.dumps(msg2)}")
    print(f"Carol ← {response2}")

    # Bob should receive NEW_USER broadcast
    time.sleep(0.5)
    broadcast = sock1.recv(4096).decode()
    print(f"\nBob ← Broadcast: {broadcast}")

    sock1.close()
    sock2.close()
    print("\n✓ Test completed")

if __name__ == "__main__":
    test_identify()
    time.sleep(1)
    test_duplicate_username()
    time.sleep(1)
    test_new_user_broadcast()
