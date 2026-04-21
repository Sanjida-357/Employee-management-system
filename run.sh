#!/bin/bash

echo "Cleaning previous processes..."
pkill server 2>/dev/null
pkill -f "http.server" 2>/dev/null

echo "Starting Backend..."
cd backend
g++ server.cpp -std=c++17 -o server
./server &

sleep 2

echo "Starting Frontend..."
cd ../frontend
python3 -m http.server 5500