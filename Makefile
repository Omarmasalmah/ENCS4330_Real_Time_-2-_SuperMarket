# the compiler: gcc for C program, define as g++ for C++
CC = gcc
USERINPUT = filename.txt
ITEMS = items.txt
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

UIFFLAGS = -Wall -lGL -lGLU -lglut -lm
all: supermarket customer customerArrival ShelvesTeam uif

supermarket: supermarket.c
	$(CC) $(CFLAGS) -o supermarket supermarket.c functions.c

customer: customer.c
	$(CC) $(CFLAGS) -o customer customer.c functions.c
ShelvesTeam: ShelvesTeam.c
	$(CC) $(CFLAGS) -o ShelvesTeam ShelvesTeam.c functions.c
customerArrival: customerArrival.c
	$(CC) $(CFLAGS) -o customerArrival customerArrival.c functions.c 
	
uif: opengl.c
	$(CC) opengl.c functions.c -o myprogram.elf $(UIFFLAGS)
run: supermarket
	./supermarket $(USERINPUT) $(ITEMS)
clean:
	$(RM) customerArrival customer supermarket ShelvesTeam uif
	$(RM) myprogram.elf
