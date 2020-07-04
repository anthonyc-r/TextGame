#!/bin/bash
glib-compile-resources editor.gresources.xml --target=resources.c --generate-source
cc `pkg-config --cflags gtk+-3.0` -o main *.c `pkg-config --libs gtk+-3.0`

