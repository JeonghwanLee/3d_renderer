build:
	gcc -arch x86_64 -std=c99 -Wall ./src/*.c -o 3d_renderer -I/Library/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2
run:
	./3d_renderer

clean:
	rm 3d_renderer
