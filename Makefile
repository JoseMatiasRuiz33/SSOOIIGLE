DIROBJ := obj/
DIREXE := exec/
DIRSRC := src/

CFLAGS := -std=c++20 -pthread -Iinc -Wall
CC := g++

all : dirs SSOOIIGLE

clean: 
	rm -rf $(DIROBJ)
	rm -rf $(DIREXE)
	rm -rf debug.txt

dirs:
	mkdir -p $(DIROBJ) $(DIREXE) 

SSOOIIGLE: $(DIROBJ)SSOOIIGLE.o 
	$(CC) -o $(DIREXE)$@ $^ -pthread


$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

debug: SSOOIIGLE
	./$(DIREXE)SSOOIIGLE 17_LEYES_DEL_TRABAJO_EN_EQUIPO.txt la 5 > debug.txt

solution: SSOOIIGLE
	./$(DIREXE)SSOOIIGLE 17_LEYES_DEL_TRABAJO_EN_EQUIPO.txt la 5

