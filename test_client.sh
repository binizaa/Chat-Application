#!/bin/bash

# Test script for the chat server protocol

echo "=== Testing IDENTIFY Protocol ==="
echo ""

echo "Sending: {\"type\": \"IDENTIFY\", \"username\": \"Kimberly\"}"
(
    echo '{"type": "IDENTIFY", "username": "Kimberly"}'
    sleep 3
) | nc localhost 8080

echo ""
echo "Done!"
