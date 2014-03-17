CC=gcc
FLAGS=-I./src/ `pkg-config --cflags opencv` -fopenmp
TARGETS=fpex
LIBS=-lm `pkg-config --libs opencv` -fopenmp

main: $(TARGETS)


lib/%.o: src/%.c
        $(CC) $(FLAGS) -c $< -o $@

clean:
        rm -f $(TARGETS) lib/*.o

%:lib/%.o lib/fp.o
        $(CC) -o $@ $^ $(LIBS)