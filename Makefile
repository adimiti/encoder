PRJ = demo
OBJ = encode.o main.o

.PHONY: all
all: $(PRJ)

$(PRJ): $(OBJ)
	gcc -o $@ $^

main.o: encode.h

%.o: %.c
	gcc -c $< -Wall -O0 -g -pedantic


.PHONY: clean
clean:
	-rm $(OBJ) $(PRJ)
