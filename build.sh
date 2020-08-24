#!/bin/bash
cd game_editor
./build.sh
cd ..

if [ ! -d ./bin ]; then
	mkdir ./bin
fi
if [ ! -e ./game_editor/main ]; then
	echo "Game editor binary wasn't built!"
	exit
fi
cp -r ./data_devel ./bin
# temp fix until I add script references to the editor
cp ./game_editor/main ./bin/editor
cc *.c  -g -lm -o main
if [ ! -e ./main ]; then
	echo "Main game binary wasn't built!"
	exit
fi
mv main bin/game
