.PHONY: all clean

filename := main

all: myprogram run

myprogram:
	g++ -o $(filename).exe $(filename).cpp -Isrc/include -Lsrc/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

run:
	./$(filename)

clean:
	rm -f $(filename)