#include <stack>
#include "TuringMachine.h"

/**
 * Constructor for the state which takes the states name and its transitions as arguments
 * @param name Name of the state
 * @param transitions Transitions of the state
 */
State::State(std::string name, std::vector<Transition>&& transitions)
        :name(name), transitions(transitions)
{

}

/**
 * Constructor for state which takes name of state as argument
 * @param name Name of the state
 */
State::State(std::string name)
        :name(name)
{
}

/**
 * Adds a transition to the state
 * @param transition Transition to add to the state
 */
void State::add_transition(Transition&& transition)
{
    this->transitions.push_back(transition);
}

/**
 * Getter for the name of the state
 * @return Name of the state
 */
std::string State::get_name() const
{
    return name;
}

/**
 * Getter for the transitions of the state
 * @return Transitions of the state
 */
const std::vector<Transition> State::get_transitions() const
{
    return transitions;
}

/**
 * This functions searches for the transition with a certain input
 * If none is found, this function returns a nullptr
 * @param input Input to search for
 * @return Pointer to transitions
 */
const Transition* State::find_transition(std::vector<TapeSymbol*>& input) const
{

    for (const auto& iter:transitions) {
        if (iter.get_input_symbols()==input) {
            return &iter;
        }
    }

    return nullptr;
}

/**
 * Equality operator for State
 * @param other State to compare with
 * @return True if this State is equal to the other
 */
bool State::operator==(const State& other) const
{
    return this->get_name()==other.get_name();
}

/**
 * Equality operator for TapeSymbol
 * @param other Other tape symbol
 * @return True if this Tape Symbol is equal to the other one
 */
bool TapeSymbol::operator==(TapeSymbol& other) const
{
    return this->get_value_string()==other.get_value_string();
}

/**
 * Constructor for tape symbol which takes its value as input
 * @param value Value of the tape symbol
 */
TapeSymbol::TapeSymbol(std::string& value)
        :value(value)
{
}

/**
 * Function that returns the value of the tape symbol.
 * @return the value as a string.
 */
std::string TapeSymbol::get_value_string() const
{
    return value;
}

/**
 * Constructor for transition
 * @param from state from where the finite control starts
 * @param to state to where the finite control goes
 * @param input inputsymbol to trigger this transition
 * @param write tapesymbol to write on the tape
 * @param direction direction of the head movement for this transition
 */
Transition::Transition(State* to, std::vector<TapeSymbol*>&& input, std::vector<TapeSymbol*>&& write,
        std::vector<Direction>&& directions)
        :to(to), input_symbol(input), write(write), directions(directions)
{
    if (directions.size()==1) {
        for (auto direction:directions) {
            if (direction==stay) {
                throw std::runtime_error("Transitie geconstrueerd met slechts een tape maar stationiare beweging");
            }
        }
    }

}

/**
 * Getter for input symbols
 * @return Vector of input symbols
 */
const std::vector<TapeSymbol*> Transition::get_input_symbols() const
{
    return input_symbol;
}

/**
 * Getter for the next state
 * @return Pointer to the next state
 */
const State* Transition::get_next_state() const
{
    return to;
}

/**
 * Getter for the write symbols
 * @return Vector of write symbols
 */
std::vector<TapeSymbol*> Transition::get_write_symbols() const
{
    return write;
}

/**
 * Getter for the direction of the Transition
 * @return Directions of the Transition
 */
std::vector<Direction> Transition::get_direction() const
{
    return directions;
}

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
TuringMachine::TuringMachine(std::vector<State*>& states, std::vector<std::string>& input_symbols,
        std::vector<TapeSymbol*>& tape_symbols, State* start_state, TapeSymbol* blank,
        std::vector<State*>& final_states, int amount_of_tapes)
        :
        states(states), input_symbols(input_symbols), tape_symbols(tape_symbols), start_state(start_state),
        blank(blank),
        final_states(final_states)
{

    for (int iter = 0; iter<amount_of_tapes; iter++) {
        auto newTape = new std::list<TapeSymbol*>;
        this->tapes.push_back(newTape);
    }

}

/**
 * Evaluates a string using the Turing machine
 * Important: the tapes need to be initialized first (setup_tapes)
 * @return True if the string is evaluated by the Turing machine, false if it is not
 */
bool TuringMachine::evaluate_string()
{

    if (tapes.size()==0) {
        return false;
    }

    return simulate_run();

}

/**
 * This function does a calculation with the Turing machine
 * Important:the tapes need to be initialized first (setup_tapes)
 * @return Vector of strings where the calculation is made on
 */
std::vector<std::string> TuringMachine::calculate()
{

    std::vector<std::string> result;

    // setup de tapes
    if (tapes.size()==0) {
        return result;
    }

    simulate_run();

    for (auto iter_tape:tapes) {
        std::string result_as_string;
        for (auto iter_string:*iter_tape) {
            if (iter_string==blank) {
                continue;
            }
            result_as_string += iter_string->get_value_string();
        }

        result.push_back(result_as_string);
    }

    return result;
}

/**
 * Setup the tapes with a certain input
 * @param input Input the setup the tape with, each string represents a tape
 */
void TuringMachine::setup_tapes_with_string(std::vector<std::string>& input)
{
    // construeer de ID's voor de tapes.

    int max_string_length = 0;
    try {
        int iter = 0;
        for (auto input_string:input) {
            this->tapes.at(iter)->push_back(this->blank);

            if (input_string.size()>max_string_length) {
                max_string_length = input_string.size();
            }

            std::vector<std::string> split(input_string.size());
            std::copy(input_string.begin(), input_string.end(), split.begin());

            for (auto character:split) {

                auto place = std::find_if(tape_symbols.begin(), tape_symbols.end(),
                        [&character](TapeSymbol* ptr_symbol) { return ptr_symbol->get_value_string()==character; });

                if (place==tape_symbols.end()) {
                    throw std::runtime_error(
                            "een karakter uit de input string is niet te vinden in het alphabet voor tapesymbolen");
                }

                this->tapes.at(iter)->push_back(*place);
            }

            this->tapes.at(iter)->push_back(this->blank);

            //zet de tape head pointer aan het begin van de string
            this->tape_head_ptrs.at(iter) = ++(this->tapes.at(iter)->begin());

            iter++;
        }

    }
    catch (std::out_of_range& e) {
        std::cerr << "de vector van strings meegegeven met functie evaluate bevat meer strings dan tapes";
        return;
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << "\n";
        return;
    }

    for (auto iter_string:this->tapes) {
        while (iter_string->size()<max_string_length+2) {
            iter_string->push_back(this->blank);
        }
    }
}

/**
 * Resets the tapes of the Turing machine
 */
void TuringMachine::reset_tapes()
{
    for (auto& tape:tapes) {
        tape->clear();
    }
}

/**
 * Simulates a run
 * @return True if Turing machine accepted
 */
bool TuringMachine::simulate_run()
{
    // we starten met simuleren vanuit de start state
    const State* current_state = this->start_state;

    // het zou vreemd zijn maar we controleren of de huidige state in de accepterende states zit
    auto place = std::find(this->final_states.begin(), this->final_states.end(), current_state);

    // zolang we niet in accepterende staat zijn
    while (place==this->final_states.end()) {
        std::vector<TapeSymbol*> read_input;
        for (auto tape_ptr:tape_head_ptrs) {
            read_input.push_back(*tape_ptr);
        }

        const Transition* transitie = current_state->find_transition(read_input);

        //indien er geen mogelijke transitie is dan zijn we klaar
        if (!transitie) {
            return false;
        }

        //voor elke tape passen we de situatie aan
        unsigned int iter = 0;
        for (auto& tape_ptr:tape_head_ptrs) {

            std::list<TapeSymbol*>* current_tape = tapes.at(iter);

            //overschrijf de tapesymbolen
            auto temp_ptr = current_tape->emplace(tape_ptr, transitie->get_write_symbols().at(iter));
            auto next_right_point = current_tape->erase(++temp_ptr);

            //beweeg de head pointer
            tape_ptr = next_right_point;
            switch (transitie->get_direction().at(iter)) {
            case right:
                if (tape_ptr==--current_tape->end()) {
                    current_tape->push_back(this->blank);
                }
                break;
            case left:
                --tape_ptr; //we decrement the tape pointer to get it back pointing to emplaced cell
                --tape_ptr;
                if (tape_ptr==current_tape->begin()) {
                    current_tape->emplace_front(this->blank);
                }
                break;
            case stay:
                --tape_ptr; //we decrement the tape pointer to get it back pointing to emplaced cell
                break;
            }

            iter++;
        }

        //update de huidige state
        current_state = transitie->get_next_state();
        place = std::find(this->final_states.begin(), this->final_states.end(), current_state);
    }

    return true;
}

/**
     * Constructor of Turing machine created from file
     * @param input_file Input file in json
     */
TuringMachine::TuringMachine(char* input_file)
{
    std::ifstream json_file(input_file);

    json json_complete;
    json_file >> json_complete;

    try {
        for (const auto& iter_state: json_complete["States"].items()) {
            State* new_state = new State(iter_state.value());
            states.push_back(new_state);
        }
    }
    catch (json::parse_error& e) {
        std::cerr << "\"States\" was niet correct geformatteerd in json file\n";
    }

    try {
        for (const auto& iter_input: json_complete["InputAlphabet"].items()) {
            input_symbols.push_back(iter_input.value());
        }
    }
    catch (json::parse_error& e) {
        std::cerr << "\"InputAlphabet\" was niet correct geformatteerd in json file\n";
    }

    try {
        for (const auto& iter_tape: json_complete["TapeAlphabet"].items()) {

            std::string temp = iter_tape.value();
            TapeSymbol* new_tape_symbol = new TapeSymbol(temp);
            tape_symbols.push_back((new_tape_symbol));

        }

    }
    catch (json::parse_error& e) {
        std::cerr << "\"TapeAlphabet\" was niet correct geformatteerd in json file\n";
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << "\n";
    }

    try {
        std::string name_blank = json_complete["Blank"];
        auto symbol_blank = std::find_if(tape_symbols.begin(), tape_symbols.end(),
                [&name_blank](TapeSymbol* ptr_symbol) { return ptr_symbol->get_value_string()==name_blank; });
        blank = *symbol_blank;
    }
    catch (json::parse_error& e) {
        std::cerr << "\"Blank\" was niet correct geformatteerd in json file\n";
    }

    try {
        int amount_of_tapes;
        if (json_complete["Tapes"].is_null()) {
            amount_of_tapes = 1;
        }
        else {
            amount_of_tapes = json_complete["Tapes"];
        }

        for (int iter = 0; iter<amount_of_tapes; iter++) {
            std::list<TapeSymbol*>* new_tape = new std::list<TapeSymbol*>();
            tapes.push_back(new_tape);
            tape_head_ptrs.push_back(new_tape->begin());
        }
    }
    catch (json::parse_error& e) {
        std::cerr << "\"Tapes\" was niet correct geformatteerd in json file\n";
    }

    try {
        std::string startState_name = json_complete["StartState"];
        State temp_state(startState_name);
        auto place_start_state = std::find_if(states.begin(), states.end(),
                [&temp_state](State* ptr_state) { return *ptr_state==temp_state; });

        if (place_start_state==states.end()) {
            throw std::runtime_error("startstate is geen bestaande state in json");
        }

        start_state = *place_start_state;

    }
    catch (json::parse_error& e) {
        std::cerr << "\"StartState\" was niet correct geformatteerd in json file\n";
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << "\n";
    }

    try {
        for (const auto& iter_final_state:json_complete["FinalStates"].items()) {
            std::string final_state_name = iter_final_state.value();
            State temp_state(final_state_name);
            auto place_final_state = std::find_if(states.begin(), states.end(),
                    [&temp_state](State* ptr_state) { return *ptr_state==temp_state; });

            if (place_final_state==states.end()) {
                throw std::runtime_error("er bestaat een final state in json dat niet bestaat in de states");
            }

            final_states.push_back(*place_final_state);
        }

    }
    catch (json::parse_error& e) {
        std::cerr << "\"FinalStates\" was niet correct geformatteerd in json file\n";
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << "\n";
    }

    try {
        for (auto& iter_transition:json_complete["Transitions"].items()) {
            std::string name_from_state = iter_transition.value()["from"];

            auto place_from_state = std::find_if(states.begin(), states.end(),
                    [&name_from_state](State* ptr_state) { return (ptr_state->get_name()==name_from_state); });

            if (place_from_state==states.end()) {
                throw std::runtime_error(
                        "er bestaat een transitie met from state in json waarvan de to state niet bestaat in de states");
            }

            State* from_state = *place_from_state;

            std::string name_to_state = iter_transition.value()["to"];

            auto place_to_state = std::find_if(states.begin(), states.end(),
                    [&name_to_state](State* ptr_state) { return (ptr_state->get_name()==name_to_state); });

            if (place_to_state==states.end()) {
                throw std::runtime_error(
                        "er bestaat een transitie met to state in json waarvan de from state niet bestaat in de states");
            }

            State* to_state = *place_to_state;

            std::vector<TapeSymbol*> read_tape;

            for (auto& iter_read:iter_transition.value()["read"].items()) {
                std::string string_value_read = iter_read.value();
                auto place_tape_symbol = std::find_if(tape_symbols.begin(), tape_symbols.end(),
                        [&string_value_read](TapeSymbol* ptr_symbol) {
                            return (string_value_read==ptr_symbol->get_value_string());
                        });

                if (place_tape_symbol==tape_symbols.end()) {
                    throw std::runtime_error(
                            "er bestaat een transitie met een input dat niet in tapesymbolen voorkomt");
                }

                read_tape.push_back(*place_tape_symbol);
            }

            std::vector<TapeSymbol*> write_tape;

            for (auto& iter_write:iter_transition.value()["write"].items()) {
                std::string string_value_write = iter_write.value();
                auto place_write_symbol = std::find_if(tape_symbols.begin(), tape_symbols.end(),
                        [&string_value_write](TapeSymbol* ptr_symbol) {
                            return ptr_symbol->get_value_string()==string_value_write;
                        });

                if (place_write_symbol==tape_symbols.end()) {
                    throw std::runtime_error(
                            "er bestaat een transitie met een write symbol zonder dat het een geldig tapesymbool is");
                }

                write_tape.push_back(*place_write_symbol);
            }

            std::vector<Direction> directions;

            for (auto& iter_direction:iter_transition.value()["direction"].items()) {
                std::string direction_name = iter_direction.value();

                Direction direction;
                if (direction_name=="left") {
                    direction = Direction::left;
                }
                else if (direction_name=="right") {
                    direction = Direction::right;
                }
                else if (direction_name=="stay") {
                    direction = Direction::stay;
                }
                else {
                    throw std::runtime_error("direction is not an existing direction");
                }

                directions.push_back(direction);
            }

            Transition new_transition(to_state, std::move(read_tape), std::move(write_tape), std::move(directions));
            from_state->add_transition(std::move(new_transition));
        }
    }
    catch (json::parse_error& e) {
        std::cerr << "\"Transitions\" was niet correct geformatteerd in json file\n";
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << "\n";
    }

    if (json_complete.find("Subroutines")!=json_complete.end()) {

        try {

            for (auto& iter_subroutine:json_complete["Subroutines"].items()) {
                std::string from_state_str = iter_subroutine.value()["from"];
                State* from_state = (*std::find_if(states.begin(), states.end(),
                        [&from_state_str](State* ptr) { return ptr->get_name()==from_state_str; }));

                //from into subroutine
                std::vector<TapeSymbol*> read_tape_from;

                for (auto& iter_read:iter_subroutine.value()["readFrom"].items()) {
                    std::string string_value_read = iter_read.value();
                    auto place_tape_symbol = std::find_if(tape_symbols.begin(), tape_symbols.end(),
                            [&string_value_read](TapeSymbol* ptr_symbol) {
                                return (string_value_read==ptr_symbol->get_value_string());
                            });

                    if (place_tape_symbol==tape_symbols.end()) {
                        throw std::runtime_error(
                                "er bestaat een transitie met een input dat niet in tapesymbolen voorkomt");
                    }

                    read_tape_from.push_back(*place_tape_symbol);
                }

                std::vector<TapeSymbol*> write_tape_from;

                for (auto& iter_write:iter_subroutine.value()["writeFrom"].items()) {
                    std::string string_value_write = iter_write.value();
                    auto place_write_symbol = std::find_if(tape_symbols.begin(), tape_symbols.end(),
                            [&string_value_write](TapeSymbol* ptr_symbol) {
                                return ptr_symbol->get_value_string()==string_value_write;
                            });

                    if (place_write_symbol==tape_symbols.end()) {
                        throw std::runtime_error(
                                "er bestaat een transitie met een write symbol zonder dat het een geldig tapesymbool is");
                    }

                    write_tape_from.push_back(*place_write_symbol);
                }

                std::vector<Direction> directions_from;

                for (auto& iter_direction:iter_subroutine.value()["directionFrom"].items()) {
                    std::string direction_name = iter_direction.value();

                    Direction direction;
                    if (direction_name=="left") {
                        direction = Direction::left;
                    }
                    else if (direction_name=="right") {
                        direction = Direction::right;
                    }
                    else if (direction_name=="stay") {
                        direction = Direction::stay;
                    }
                    else {
                        throw std::runtime_error("direction is not an existing direction");
                    }

                    directions_from.push_back(direction);
                }

                //to after subroutine
                std::string to_state_str = iter_subroutine.value()["to"];
                State* to_state = (*std::find_if(states.begin(), states.end(),
                        [&to_state_str](State* ptr) { return ptr->get_name()==to_state_str; }));

                std::vector<TapeSymbol*> read_tape_to;

                for (auto& iter_read:iter_subroutine.value()["readTo"].items()) {
                    std::string string_value_read = iter_read.value();
                    auto place_tape_symbol = std::find_if(tape_symbols.begin(), tape_symbols.end(),
                            [&string_value_read](TapeSymbol* ptr_symbol) {
                                return (string_value_read==ptr_symbol->get_value_string());
                            });

                    if (place_tape_symbol==tape_symbols.end()) {
                        throw std::runtime_error(
                                "er bestaat een transitie met een input dat niet in tapesymbolen voorkomt");
                    }

                    read_tape_to.push_back(*place_tape_symbol);
                }

                std::vector<TapeSymbol*> write_tape_to;

                for (auto& iter_write:iter_subroutine.value()["writeTo"].items()) {
                    std::string string_value_write = iter_write.value();
                    auto place_write_symbol = std::find_if(tape_symbols.begin(), tape_symbols.end(),
                            [&string_value_write](TapeSymbol* ptr_symbol) {
                                return ptr_symbol->get_value_string()==string_value_write;
                            });

                    if (place_write_symbol==tape_symbols.end()) {
                        throw std::runtime_error(
                                "er bestaat een transitie met een write symbol zonder dat het een geldig tapesymbool is");
                    }

                    write_tape_to.push_back(*place_write_symbol);
                }

                std::vector<Direction> directions_to;

                for (auto& iter_direction:iter_subroutine.value()["directionTo"].items()) {
                    std::string direction_name = iter_direction.value();

                    Direction direction;
                    if (direction_name=="left") {
                        direction = Direction::left;
                    }
                    else if (direction_name=="right") {
                        direction = Direction::right;
                    }
                    else if (direction_name=="stay") {
                        direction = Direction::stay;
                    }
                    else {
                        throw std::runtime_error("direction is not an existing direction");
                    }

                    directions_to.push_back(direction);
                }

                std::string subroutine_machine_str = iter_subroutine.value()["subroutine_file"];

                TuringMachine subroutine_machine(const_cast<char*>(subroutine_machine_str.c_str()));

                add_subroutine(std::move(subroutine_machine), from_state, std::move(read_tape_from),
                        std::move(write_tape_from), std::move(directions_from),
                        to_state, std::move(read_tape_to), std::move(write_tape_to), std::move(directions_to));

            }


        }
        catch (json::parse_error& e) {
            std::cerr << "\"Subroutines\" was niet correct geformatteerd in json file";
        }

    }

    json_file.close();
}

/**
 * Destructor for Turing machine
 */
TuringMachine::~TuringMachine()
{
    start_state = nullptr;

    for (auto final_state:states) {
        final_state = nullptr;
    }
    final_states.clear();

    for (auto state:states) {
        delete state;
    }
    states.clear();

    input_symbols.clear();

    blank = nullptr;

    tape_head_ptrs.clear();

    for (auto tape:tapes) {
        for (auto tape_symbol:*tape) {
            tape_symbol = nullptr;
        }
    }
    for (auto tape:tapes) {
        delete tape;
    }
    tapes.clear();

    for (auto tape_symbol:tape_symbols) {
        delete tape_symbol;
    }
    tape_symbols.clear();

}

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
void TuringMachine::add_subroutine(TuringMachine&& subroutine, State* stateFrom, std::vector<TapeSymbol*>&& getIn_read,
        std::vector<TapeSymbol*>&& getIn_write,
        std::vector<Direction>&& getIn_dir, State* stateTo, std::vector<TapeSymbol*>&& getOut_read,
        std::vector<TapeSymbol*>&& getOut_write,
        std::vector<Direction>&& getOut_dir)
{

    std::stack<const State*> states_to_process;

    const State* subroutine_start_state = subroutine.get_start_state();


    //voeg de starttoestand van de subroutine toe
    std::vector<State*>::iterator loc = std::find_if(states.begin(), states.end(),
            [&subroutine_start_state](State* ptr) {
                return ptr->get_name()==subroutine_start_state->get_name();
            });
    State* new_start_state;
    if (loc==states.end()) {
        new_start_state = new State(subroutine_start_state->get_name());


        //plaats de nieuwe toestand in de vector van bestaande toestanden
        states.push_back(new_start_state);

    }
    else {
        new_start_state = *loc;
    }

    const State* subroutine_state = subroutine_start_state;
    states_to_process.push(subroutine_start_state);

    //we gebruiken de getIn om de machine met de subTuringMachine te verbinden.
    Transition getIn(new_start_state, std::move(getIn_read), std::move(getIn_write), std::move(getIn_dir));
    stateFrom->add_transition(std::move(getIn));

    while (!states_to_process.empty()) {

        states_to_process.pop();

        const std::vector<Transition> subroutine_transitions = subroutine_state->get_transitions();

        //voeg de toestand van de subroutine toe
        std::vector<State*>::iterator loc = std::find_if(states.begin(), states.end(),
                [&subroutine_state](State* ptr) {
                    return ptr->get_name()==subroutine_state->get_name();
                });
        //de toestanden worden al toegevoegd wanneer we ze ontdekken via de transities
        State* new_from_state = *loc;


        //voeg de transities toe
        for (int iter_transition = 0; iter_transition<subroutine_transitions.size(); ++iter_transition) {

            std::vector<State*>::iterator loc = std::find_if(states.begin(), states.end(),
                    [&](State* ptr) {
                        return ptr->get_name()==subroutine_transitions.at(
                                iter_transition).get_next_state()->get_name();
                    });

            State* new_state;
            if (loc==states.end()) {
                new_state = new State(subroutine_transitions.at(iter_transition).get_next_state()->get_name());

                //als hij niet gevonden wordt staat de toestand nog niet in de stack anders wel.
                states_to_process.push(subroutine_transitions.at(iter_transition).get_next_state());

                //plaats de nieuwe toestand in de vector van bestaande toestanden
                states.push_back(new_state);
            }
            else {
                new_state = *loc;
            }

            std::vector<TapeSymbol*> new_input_symbols = subroutine_transitions.at(
                    iter_transition).get_input_symbols();

            //zet de verwijzingen correct
            std::vector<TapeSymbol*> input_symbols;
            for (auto& symbol:new_input_symbols) {

                std::vector<TapeSymbol*>::iterator loc_tapesymbol_in_turing = std::find_if(tape_symbols.begin(),
                        tape_symbols.end(),
                        [&](TapeSymbol* ptr) { return ptr->get_value_string()==symbol->get_value_string(); });

                if (loc_tapesymbol_in_turing==tape_symbols.end()) {
                    std::string temp = symbol->get_value_string();
                    TapeSymbol* new_symbol = new TapeSymbol(temp);

                    tape_symbols.push_back(new_symbol);
                    input_symbols.push_back(new_symbol);
                }
                else {
                    input_symbols.push_back(*loc_tapesymbol_in_turing);
                }
            }

            std::vector<TapeSymbol*> new_write_symbols = subroutine_transitions.at(
                    iter_transition).get_write_symbols();

            //zet de verwijzingen correct
            std::vector<TapeSymbol*> write_symbols;
            for (auto& symbol:new_write_symbols) {

                std::vector<TapeSymbol*>::iterator loc_tapesymbol_in_turing = std::find_if(tape_symbols.begin(),
                        tape_symbols.end(),
                        [&](TapeSymbol* ptr) { return ptr->get_value_string()==symbol->get_value_string(); });

                if (loc_tapesymbol_in_turing==tape_symbols.end()) {
                    std::string temp = symbol->get_value_string();
                    TapeSymbol* new_symbol = new TapeSymbol(temp);

                    tape_symbols.push_back(new_symbol);
                    write_symbols.push_back(new_symbol);
                }
                else {
                    write_symbols.push_back(*loc_tapesymbol_in_turing);
                }
            }

            std::vector<Direction> new_directions = subroutine_transitions.at(iter_transition).get_direction();

            Transition new_transition(new_state, std::move(input_symbols), std::move(write_symbols),
                    std::move(new_directions));

            new_from_state->add_transition(std::move(new_transition));

        }

        if (!states_to_process.empty()) {
            subroutine_state = states_to_process.top();
        }
    }

    //voor impleteermogelijke redenen neem ik hier aan dat er een finale toestand is in de subroutine
    const State* final = subroutine.get_final_states().at(0);

    loc = std::find_if(states.begin(), states.end(),
            [final](State* ptr) {
                return ptr->get_name()==final->get_name();
            });

    Transition getOut(stateTo, std::move(getOut_read), std::move(getOut_write), std::move(getOut_dir));

    (*loc)->add_transition(std::move(getOut));

}

/**
 * Generate a dot file of the Turing machine
 * @param os Stream to generate dot file to
 */
void TuringMachine::to_dot(std::ostream& os)
{
    os << "digraph G{\n";
    os << "rankdir = LR\n"; //maak de dot van links naar rechts

    int iter = 0;
    for (auto state: states) {
        os << state->get_name();
        auto loc = std::find_if(final_states.begin(), final_states.end(),
                [&state](State* ptr) { return (*ptr==*state); });
        if (loc!=final_states.end()) {
            os << "[shape=doublecircle]\n";
        }
        os << "\n";
        ++iter;
    }

    for (auto state:states) {
        for (auto& transitie:state->get_transitions()) {
            std::string label;
            for (int iter = 0; iter<transitie.get_input_symbols().size(); ++iter) {
                label += transitie.get_input_symbols().at(iter)->get_value_string()+",";
                label += transitie.get_write_symbols().at(iter)->get_value_string()+",";

                switch (transitie.get_direction().at(iter)) {
                case right:
                    label += "R\\n";
                    break;
                case left:
                    label += "L\\n";
                    break;
                case stay:
                    label += "S\\n";
                    break;
                }
            }

            os << state->get_name() << "->" << transitie.get_next_state()->get_name() << "[label=\"" << label
               << "\"]\n";
        }

    }

    os << "}";
}

/**
 * Getter for start State
 * @return start State of Turing machine
 */
const State* TuringMachine::get_start_state()
{
    return start_state;
}

/**
 * Getter for the final states of the Turing machine
 * @return final states of the Turing machine
 */
const std::vector<State*>& TuringMachine::get_final_states()
{
    return final_states;
}
