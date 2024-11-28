# Target voor het bouwen van het uitvoerbare bestand
output: main.o automaton.o
	g++ main.o automaton.o -o output

# Regel voor het compileren van main.cpp naar een objectbestand
main.o: main.cc automaton.h
	g++ -c main.cc

# Regel voor het compileren van Expression.cpp naar een objectbestand
expressie.o: automaton.cc automaton.h
	g++ -c automaton.cc

# Regel om de build op te schonen
clean:
	rm -f *.o output

# Nieuw 'run' target om het programma te compileren en uit te voeren
run: output
	./output