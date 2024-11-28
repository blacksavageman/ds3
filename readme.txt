## Auteurs
Mohammad Hammad Arif s3668630 & Sami Qalyoen s3709868

## Automaat Representatie
Gekozen representatie: Adjacency list met vector<vector<Transition>>
Voordelen:
- Geheugen efficiënt: Slaat alleen bestaande transities op
- Snelle lookup van transities vanuit een toestand
- Eenvoudige implementatie van Thompson constructie
- Makkelijk nieuwe transities toevoegen tijdens constructie

## Gebruik
1. Compileren:
   make

2. Programma starten:
   ./output

3. Beschikbare commando's:
   - exp <regex>    : Verwerk een nieuwe reguliere expressie
                      Bijvoorbeeld: exp a(b|cd)*ef
   - dot <bestand>  : Schrijf huidige automaat naar DOT bestand
                      Bijvoorbeeld: dot output.dot
   - end            : Programma afsluiten

## Test cases:
1. a(b|cd)*ef           # Basis expressie met herhaling en keuze
2. (a*|b)*a            # Geneste ster operator
3. (a|b)*(aa|bb)(a|b)* # Meerdere keuzes en herhalingen
4. ((ab)*|c*)*d        # Complex geneste expressie