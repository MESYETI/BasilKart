SRC   = $(wildcard source/*.c) $(wildcard source/**/*.c)
DEPS  = $(wildcard source/*.h) $(wildcard source/**/*.h)
OBJ   = $(addsuffix .o,$(subst source/,bin/,$(basename ${SRC})))
LIBS  = -lSDL2 -lm
FLAGS = -std=c99 -Wall -Wextra -pedantic -g -I./lib
OUT   = BasilKart

ifeq ($(BUILD), release)
	FLAGS += -O3
endif

compile: ./bin ./bin/gfx ./bin/ui $(OBJ) $(SRC) $(DEPS)
	$(CC) $(OBJ) $(LIBS) -o $(OUT)

./bin/gfx:
	mkdir -p bin/gfx

./bin/ui:
	mkdir -p bin/ui

./bin:
	mkdir -p bin

bin/%.o: source/%.c $(DEPS)
	$(CC) -c $< $(FLAGS) -o $@

clean:
	rm -r bin/* $(OUT)

