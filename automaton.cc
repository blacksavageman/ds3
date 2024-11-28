#include "automaton.h"
#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <queue>

//Constructor
Automaton::Automaton() : start_state(0), final_state(0), current_pos(0) {}

//Verwerk regular expression
void Automaton::parse_expression(const std::string& expr) {
    current_expression = expr;
    current_pos = 0;
    transitions.clear();
    
    //Begin recursieve parsing
    AutomatonFragment result = parse_expr();
    start_state = result.start;
    final_state = result.end;
}

//Haal volgende karakter uit expressie
int Automaton::get_next_char() {
    if (current_pos >= current_expression.length()) {
        return -1;
    }
    return current_expression[current_pos++];
}

//Controleer of karakter overeenkomt met verwacht karakter
void Automaton::match_char(char expected) {
    char c = get_next_char();
    if (c != expected) {
        throw std::runtime_error("Onverwachte karakter");
    }
}

//Maak nieuwe staat aan
int Automaton::new_state() {
    transitions.push_back(std::vector<Transition>());
    return transitions.size() - 1;
}

//Voeg transitie toe
void Automaton::add_transition(int from, int to, char label) {
    transitions[from].push_back({to, label});
}

//Thompson constructies
Automaton::AutomatonFragment Automaton::create_basic(char c) {
    int start = new_state();
    int end = new_state();
    add_transition(start, end, c);
    return {start, end};
}

//Union
Automaton::AutomatonFragment Automaton::create_union(const AutomatonFragment& f1, const AutomatonFragment& f2) {
    int start = new_state();
    int end = new_state();
    
    add_transition(start, f1.start, '-');  
    add_transition(start, f2.start, '-');
    add_transition(f1.end, end, '-');
    add_transition(f2.end, end, '-');
    
    return {start, end};
}

//Concatenation
Automaton::AutomatonFragment Automaton::create_concatenation(const AutomatonFragment& f1, const AutomatonFragment& f2) {
    add_transition(f1.end, f2.start, '-');
    return {f1.start, f2.end};
}

//Star
Automaton::AutomatonFragment Automaton::create_star(const AutomatonFragment& f) {
    int start = new_state();
    int end = new_state();
    
    add_transition(start, end, '-');
    add_transition(start, f.start, '-');
    add_transition(f.end, end, '-');
    add_transition(f.end, f.start, '-');
    
    return {start, end};
}

//Parser functies
Automaton::AutomatonFragment Automaton::parse_expr() {
    AutomatonFragment term = parse_term();
    
    int c = get_next_char();
    if (c == '|') {
        AutomatonFragment expr = parse_expr();
        return create_union(term, expr);
    } else {
        current_pos--; 
        return term;
    }
}

//Parser functie voor termen
Automaton::AutomatonFragment Automaton::parse_term() {
    AutomatonFragment fact = parse_fact();
    
    if (current_pos < current_expression.length()) {
        char c = current_expression[current_pos];
        if (c == '(' || (c >= 'a' && c <= 'z')) {
            AutomatonFragment term = parse_term();
            return create_concatenation(fact, term);
        }
    }
    return fact;
}

//Parser functie voor basis, haakjes en ster
Automaton::AutomatonFragment Automaton::parse_fact() {
    char c = get_next_char();
    AutomatonFragment result({0, 0});
    
    if (c == '(') {
        result = parse_expr();
        match_char(')');
    } else if (c >= 'a' && c <= 'z') {
        result = create_basic(c);
    } else {
        throw std::runtime_error("Onjuiste karakter in expressie");
    }
    
    //Check voor star
    if (current_pos < current_expression.length() && 
        current_expression[current_pos] == '*') {
        current_pos++;
        result = create_star(result);
    }
    
    return result;
}
//Schrijf automaat naar dot bestand
void Automaton::write_dot(const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Kan file niet openen");
    }
    
    out << "digraph G {\n";
    out << "rankdir=\"LR\"\n";
    
    //Schrijf transities
    for (size_t from = 0; from < transitions.size(); from++) {
        for (const auto& trans : transitions[from]) {
            out << from << " -> " << trans.target;
            if (trans.label != '-') {
                out << " [label=\"" << trans.label << "\"]";
            }
            out << "\n";
        }
    }
    
    out << "}\n";
    out.close();
}


bool Automaton::match_string(const std::string& str) {
    if (transitions.empty()) {
        throw std::runtime_error("Automaat is leeg, parse eerst een expressie.");
    }

    // Hulpfunctie om de ε-afsluiting te berekenen
    auto epsilon_closure = [&](const std::unordered_set<int>& states) -> std::unordered_set<int> {
        std::unordered_set<int> closure = states;
        std::queue<int> to_process;
        for (int state : states) {
            to_process.push(state);
        }

        while (!to_process.empty()) {
            int current = to_process.front();
            to_process.pop();

            for (const auto& trans : transitions[current]) {
                if (trans.label == '-' && closure.find(trans.target) == closure.end()) {
                    closure.insert(trans.target);
                    to_process.push(trans.target);
                }
            }
        }

        return closure;
    };

    // Initialiseer de huidige toestanden met de ε-afsluiting van de starttoestand
    std::unordered_set<int> current_states = epsilon_closure({start_state});

    // Verwerk elke letter in de string
    for (char c : str) {
        std::unordered_set<int> next_states;

        for (int state : current_states) {
            for (const auto& trans : transitions[state]) {
                if (trans.label == c) {
                    next_states.insert(trans.target);
                }
            }
        }

        // Bereken de ε-afsluiting van de volgende toestanden
        current_states = epsilon_closure(next_states);

        // Als er geen toestanden meer over zijn, is er geen match mogelijk
        if (current_states.empty()) {
            return false;
        }
    }

    // Controleer of een van de huidige toestanden de eindtoestand is
    return current_states.find(final_state) != current_states.end();
}
