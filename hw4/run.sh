gcc main.c -o main
gcc -c -fPIC ff.c -o ff.o
gcc ff.o -shared -o ff.so
gcc -c -fPIC bf.c -o bf.o
gcc bf.o -shared -o bf.so
LD_PRELOAD=./bf.so ./main
LD_PRELOAD=./ff.so ./main