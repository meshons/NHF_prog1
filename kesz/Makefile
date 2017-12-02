ODIR=obj

_OBJ = charset.o text.o settings.o results.o file.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

release: clean nhf
linux: cleanlinux nhflinux

obj/%.o: src/%.c
	gcc -c -o $@ $< -std=c11 -Wall

nhflinux: $(OBJ)
	gcc -o bin/nhf.exe $^ -Wall `sdl2-config --cflags --libs` -lSDL2_ttf -pthread -std=c11 -lm

nhf: $(OBJ)
	gcc -o bin/nhf.exe $^ -Wall -lmingw32 -lSDL2main -lSDL2 -mwindows -lSDL2_ttf -std=c11 -pthread -lm

clean:
	rd /s /q "obj"
	mkdir "obj"

cleanlinux:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
