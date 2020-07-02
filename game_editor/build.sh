#!/bin/bash
cc `pkg-config --cflags gtk+-3.0` -o main *.c `pkg-config --libs gtk+-3.0`

