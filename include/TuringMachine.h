#ifndef PROJECT_TURINGMACHINE_H
#define PROJECT_TURINGMACHINE_H


#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <list>
#include <stack>

#include "../include/json.hpp"
using json = nlohmann::json;


/**
 * Enum to display the direction of the turing machine
 */
enum Direction {stay, left, right};

class TapeSymbol;
class Transition;

/**
 * Class which represents the state of the Turing machine
 */
class State{
public:
    /**
     * Default constructor for a State
     */
    State() = default;

    /**
     * Constructor for state which takes name of state as argument
     * @param name Name of the state
     */
    explicit State(std::string name);

    /**
     * Constructor for the state which takes the states name and its transitions as arguments
     * @param name Name of the state
     * @param transitions Transitions of the state
     */
    State(std::string name, std::vector<Transition>&& transitions);

    /**
     * Default destructor of the State
     */
    virtual ~State() = default;

    /**
     * Adds a transition to the state
     * @param transition Transition to add to the state
     */
    void add_transition(Transition&& transition);

    /**
     * Getter for the name of the state
     * @return Name of the state
     */
    std::string get_name() const;

    /**
     * Getter for the transitions of the state
     * @return Transitions of the state
     */
    const std::vector<Transition > get_transitions() const;

    /**
     * This functions searches for the transition with a certain input
     * If none is found, this function returns a nullptr
     * @param input Input to search for
     * @return Pointer to transitions
     */
    const Transition* find_transition(std::vector<TapeSymbol* >& input) const;

    /**
     * Equality operator for State
     * @param other State to compare with
     * @return True if this State is equal to the other
     */
    bool operator==(const State& other) const;

private:
    std::string name;
    std::vector<Transition > transitions;
};

/**
 *  This class represents a tape symbol of a Turing machine
 */
class TapeSymbol{
public:
    /**
     * Default constructor of Tape Symbol
     */
    TapeSymbol() = default;

    /**
     * Constructor for tape symbol which takes its value as input
     * @param value Value of the tape symbol
     */
    explicit TapeSymbol(std::string& value);

    /**
     * Default destructor of the Tape Symbol
     */
    ~TapeSymbol() = default;

    /**
     * Function that returns the value of the tape symbol.
     * @return the value as a string.
     */
    std::string get_value_string() const;

    /**
     * Equality operator for TapeSymbol
     * @param other Other tape symbol
     * @return True if this Tape Symbol is equal to the other one
     */
    bool operator==(TapeSymbol& other) const;

private:
    std::string value;
};


/**
 *  Class which represents a Turing machine transition
 */
class Transition {
public:
    /**
     * Constructor for transition
     * @param from state from where the finite control starts
     * @param to state to where the finite control goes
     * @param input inputsymbol to trigger this transition
     * @param write tapesymbol to write on the tape
     * @param direction direction of the head movement for this transition
     */
    Transition(State* to, std::vector<TapeSymbol* >&& input, std::vector<TapeSymbol* >&& write, std::vector<Direction >&& directions);

    /**
     * Getter for input symbols
     * @return Vector of input symbols
     */
    const std::vector<TapeSymbol* > get_input_symbols() const;

    /**
     * Getter for the next state
     * @return Pointer to the next state
     */
    const State* get_next_state() const;

    /**
     * Getter for the write symbols
     * @return Vector of write symbols
     */
    std::vector<TapeSymbol*> get_write_symbols() const;

    /**
     * Getter for the direction of the Transition
     * @return Directions of the Transition
     */
    std::vector<Direction > get_direction() const;
private:
    State* to;
    std::vector<TapeSymbol* > input_symbol;
    std::vector<TapeSymbol* > write;
    std::vector<Direction > directions;
};

/**
 *  This class represents a Turing machine
 */
class TuringMachine {
public:
    /**
     * Default constructor of Turing machine
     */
    TuringMachine() = default;

    /**
     * Constructor of Turing machine
     * @param states States of Turing machine
     * @param input_symbols Input symbols op Turing machine
     * @param tape_symbols Tape symbols of Turing machine
     * @param start_state Start state of Turing machine
     * @param blank Blank symbol of Turing machine
     * @param final_states Final states of Turing machine
     * @param amount_of_tapes The amount of tapes of a Turing machine (multi tape support)
     */
    TuringMachine(std::vector<State* >& states, std::vector<std::string >& input_symbols, std::vector<TapeSymbol* >&
                    tape_symbols, State* start_state, TapeSymbol* blank, std::vector<State* >& final_states,
                    int amount_of_tapes);

    /**
     * Constructor of Turing machine created from file
     * @param input_file Input file in json
     */
    explicit TuringMachine(char input_file[]);

    /**
     * Destructor for Turing machine
     */
    ~TuringMachine();

    /**
     * Evaluates a string using the Turing machine
     * Important: the tapes need to be initialized first (setup_tapes)
     * @return True if the string is evaluated by the Turing machine, false if it is not
     */
    bool evaluate_string();

    /**
     * This function does a calculation with the Turing machine
     * Important:the tapes need to be initialized first (setup_tapes)
     * @return Vector of strings where the calculation is made on
     */
    std::vector<std::string > calculate();

    /**
     * Setup the tapes with a certain input
     * @param input Input the setup the tape with, each string represents a tape
     */
    void setup_tapes_with_string(std::vector<std::string >& input);

    /**
     * Resets the tapes of the Turing machine
     */
    void reset_tapes();

    /**
     * Adds a subroutine to the Turing machine
     * @param subroutine Subroutine to add
     * @param stateFrom From state input
     * @param getIn_read Read symbols input
     * @param getIn_write Write symbols input
     * @param getIn_dir Directions input
     * @param stateTo To state
     * @param getOut_read Read symbols output
     * @param getOut_write Write symbols output
     * @param getOut_dir Directions output
     */
    void add_subroutine(TuringMachine&& subroutine, State* stateFrom, std::vector<TapeSymbol *>&& getIn_read, std::vector<TapeSymbol *>&& getIn_write,
                        std::vector<Direction >&& getIn_dir ,State* stateTo, std::vector<TapeSymbol *>&& getOut_read, std::vector<TapeSymbol *>&& getOut_write,
                        std::vector<Direction >&& getOut_dir);

    /**
     * Generate a dot file of the Turing machine
     * @param os Stream to generate dot file to
     */
    void to_dot(std::ostream& os);

    /**
     * Getter for start State
     * @return start State of Turing machine
     */
    const State* get_start_state();

    /**
     * Getter for the final states of the Turing machine
     * @return final states of the Turing machine
     */
    const std::vector<State *>& get_final_states();

private:
    /**
     * Simulates a run
     * @return True if Turing machine accepted
     */
    bool simulate_run();

    std::vector<State* > states;
    std::vector<std::string > input_symbols;
    std::vector<TapeSymbol* > tape_symbols;
    State* start_state;
    TapeSymbol* blank;
    std::vector<State* > final_states;
    std::vector<std::list<TapeSymbol* >* > tapes;
    std::vector<std::list<TapeSymbol* >::iterator > tape_head_ptrs;
};


#endif //PROJECT_TURINGMACHINE_H
