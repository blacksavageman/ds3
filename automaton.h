#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <string>
#include <vector>

class Automaton {
private:
    struct Transition {
        int target;          
        char label;          
    };

    struct AutomatonFragment {
        int start;          
        int end;           
    };
    
    std::vector<std::vector<Transition>> transitions;
    int start_state;        
    int final_state;        
    std::string current_expression;  
    int current_pos;        

    //Helper functies
    int get_next_char();
    void match_char(char c);
    int new_state();
    void add_transition(int from, int to, char label);
    
    //Thompson constructies
    AutomatonFragment create_basic(char c);
    AutomatonFragment create_union(const AutomatonFragment& f1, const AutomatonFragment& f2);
    AutomatonFragment create_concatenation(const AutomatonFragment& f1, const AutomatonFragment& f2);
    AutomatonFragment create_star(const AutomatonFragment& f);
    
    //Parser functies
    AutomatonFragment parse_expr();   
    AutomatonFragment parse_term();   
    AutomatonFragment parse_fact();   

public:
    Automaton();
    void parse_expression(const std::string& expr);
    void write_dot(const std::string& filename);
    bool match_string(const std::string& str);

};

#endif