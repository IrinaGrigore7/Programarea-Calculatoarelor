cc = gcc

build: main.o command.o text.o
	$(cc) -Wall main.o command.o text.o -o format_text
run:
	./format_text
clean:
	rm -f *.o format_text

main.o: main.c Text.h Command.h
	$(cc) -c main.c -o main.o
command.o: Command.c Command.h
	$(cc) -c Command.c -o command.o
text.o: Text.c Text.h
	$(cc) -c Text.c -o text.o

