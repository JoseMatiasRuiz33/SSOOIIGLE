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

# He añadido $(DIROBJ)Cliente.o a la lista de dependencias
SSOOIIGLE: $(DIROBJ)SSOOIIGLE.o $(DIROBJ)Buscador.o $(DIROBJ)ResultadoBusqueda.o $(DIROBJ)Cliente.o
	$(CC) -o $(DIREXE)$@ $^ -pthread

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

# Actualizados los argumentos para que coincidan con tu nuevo main (ejemplo: 10 clientes, 2 réplicas)
debug: SSOOIIGLE
	./$(DIREXE)SSOOIIGLE 10 2 > debug.txt

solution: SSOOIIGLE
	./$(DIREXE)SSOOIIGLE 10 2
