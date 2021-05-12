assembler: ./src/main.cpp  ./src/linker.cpp
	g++ -o linker ./src/main.cpp ./src/linker.cpp -std=gnu++1z