all: program

program:
	gcc -o showFDTables main.c

clean:
	rm -f showFDTables