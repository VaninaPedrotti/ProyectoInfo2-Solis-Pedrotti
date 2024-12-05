all: programa 

programa: main.o dispenser.o sensor.o memoria_SD.o
	gcc -o programa main.o
	gcc -o programa dispenser.o 
	gcc -o programa sensor.o
	gcc -o programa memoria_SD.o

main.o: src/main.cpp 
	gcc -c src/main.cpp

dispenser.o: src/dispenser.cpp include/dispenser.h
	gcc -c src/dispenser.cpp

sensor.o: src/sensor.cpp include/sensor.h
	gcc -c src/sensor.cpp

memoria_SD.o: src/memoria_SD.cpp include/memoria_SD.h
	gcc -c src/memoria_SD.cpp

clean:
	rm -f programa *.o

