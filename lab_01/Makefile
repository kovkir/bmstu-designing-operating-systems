compilation:
	gcc -pthread main.c -o main.exe
compilation_v1:
	gcc -pthread main_v1.c -o main.exe
compilation_v2:
	gcc -pthread -mrtm -march=native main_v2.c -o main.exe

run: compilation
	./main.exe
run_v1: compilation_v1
	./main.exe
run_v2: compilation_v2
	./main.exe
