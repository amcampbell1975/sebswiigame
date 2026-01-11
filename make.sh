echo "Compiling Box2d"
cd box2d
gcc -c *.c
cd ..

echo "Compiling src"

gcc ./src/game.c ./x11/draw.c   ./box2d/*.o   -I./box2d/ -lm -lX11 -o game
