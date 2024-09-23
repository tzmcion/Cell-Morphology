g++ -o main.o main.cpp `pkg-config --cflags --libs opencv4`
./main.o
rm ./main.o