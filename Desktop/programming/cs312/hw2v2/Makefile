ifeq ($(shell uname -s), Darwin)
LDFLAGS=-L/usr/local/Cellar/glfw3/3.2.1/lib -framework OpenGL -lglfw3 -lm
else
LDFLAGS=-lX11 -lGL -lGLU -lglfw -lGLEW -lm
endif
CC = gcc
CFLAGS=-g -Wall -I/usr/local/Cellar/glfw3/3.2.1/include
SOURCES=main.c list.c ship.c
OBJECTS=$(SOURCES:.c=.o)
DEPS=list.h
EXECUTABLE=main list ship

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

clean:
	rm *o $(EXECUTABLE)
