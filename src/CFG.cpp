#include <fstream>
#include <CFG.h>
#include "DesignByContract.h"
#include "json.hpp"

using json = nlohmann::json;

/**
 * Getter for the variables
 * @return a vector of string representing the variables
 */
const std::vector<std::string>& CFG::getVariables() const
{
    return m_variables;
}

/**
 * Setter for the variables
 * @param variables a vector of strings
 */
void CFG::setVariables(const std::vector<std::string>& variables)
{
    CFG::m_variables = variables;
}

/**
 * Getter for the terminals.
 * @return a vector of strings representing the terminals.
 */
const std::vector<std::string>& CFG::getTerminals() const
{
    return m_terminals;
}

/**
 * Setter for the terminals
 * @param terminals a vector of strings
 */
void CFG::setTerminals(const std::vector<std::string>& terminals)
{
    CFG::m_terminals = terminals;
}

/**
 * Getter for productions.
 * @return a vector of CFG productions
 */
const std::vector<CFGProduction>& CFG::getProductions() const
{
    return m_productions;
}

/**
 * Setter for productions
 * @param productions a vector of CFG productions
 */
void CFG::setProductions(const std::vector<CFGProduction>& productions)
{
    CFG::m_productions = productions;
}

/**
 * Getter for the start symbol
 * @return a string representing the start symbol
 */
const std::string& CFG::getStart() const
{
    return m_start;
}

/**
 * Setter for the start symbol
 * @param start a string that will become the start symbol
 */
void CFG::setStart(const std::string& start)
{
    CFG::m_start = start;
}

/**
 * Constructor for CFG
 * @param variables a vector of strings representing the variables
 * @param terminals a vector of string representing the terminals
 * @param productions a vector of CFG productions
 * @param start a string representing the starting variable
 */
CFG::CFG(const std::vector<std::string>& variables,
        const std::vector<std::string>& terminals, const std::vector<CFGProduction>& productions,
        const std::string& start)
        :m_variables(variables), m_terminals(terminals),
         m_productions(productions), m_start(start) { }

/**
 * Default constructor
 */
CFG::CFG() { }

/**
 * Destructor for cfg
 */
CFG::~CFG() { }

/**
 * Function that adds a variable to the vector
 * @param toAdd a string representing a variable
 */
void CFG::addVariable(const std::string& toAdd)
{
    m_variables.push_back(toAdd);
}

/**
 * Function that adds a terminal to the vector
 * @param toAdd String representing a terminal
 */
void CFG::addTerminal(const std::string& toAdd)
{
    m_terminals.push_back(toAdd);
}

/**
 * Function that adds a production the the vector
 * @param toAdd a cfgproduction that will be added
 */
void CFG::addProduction(const CFGProduction& toAdd)
{
    m_productions.push_back(toAdd);
}

/**
 * Function that deletes a variable out of the vector
 * @param toDelete a string that will be deleted
 */
void CFG::deleteVariable(const std::string& toDelete)
{
    for (unsigned int i = 0; i<m_variables.size(); i++) {
        if (m_variables[i]==toDelete) {
            m_variables.erase(m_variables.begin()+i);
        }
    }
    for (auto del : m_variables) {
        REQUIRE(toDelete!=del, "The variable wasn't deleted correctly");
    }
}

/**
 * Function that deletes a variable at a given index
 * @param toDelete Index of the place you want to delete
 */
void CFG::deleteVariable(const int toDelete)
{
    ENSURE(toDelete<m_variables.size(), "The given integer was out of range for the vector");
    m_variables.erase(m_variables.begin()+toDelete);
}

/**
 * Function that deletes a variable terminal out of the vector
 * @param toDelete a string that will be deleted
 */
void CFG::deleteTerminal(const std::string& toDelete)
{
    for (unsigned int i = 0; i<m_terminals.size(); i++) {
        if (m_terminals[i]==toDelete) {
            m_terminals.erase(m_terminals.begin()+i);
        }
    }
    for (auto del : m_terminals) {
        REQUIRE(toDelete!=del, "The terminal wasn't deleted correctly");
    }
}

/**
 * Function that deletes a terminal with an index
 * @param toDelete an index of the place you want to delete
 */
void CFG::deleteTerminal(const int toDelete)
{
    ENSURE(toDelete<m_terminals.size(), "The given integer was out of range for the vector");
    m_terminals.erase(m_terminals.begin()+toDelete);
}

/**
 * Function that deletes a production with an index
 * @param toDelete an index of the place you want to delete
 */
void CFG::deleteProduction(const int toDelete)
{
    ENSURE(toDelete<m_productions.size(), "The given integer was out of range for the vector");
    m_productions.erase(m_productions.begin()+toDelete);
}

/**
* Function that deletes a production out of the vector
* @param toDelete Production that will be deleted
*/
void CFG::deleteProduction(const CFGProduction& toDelete)
{
    for (unsigned int i = 0; i<m_productions.size(); i++) {
        if (m_productions[i]==toDelete) {
            m_productions.erase(m_productions.begin()+i);
        }
    }
    for (auto del : m_productions) {
        REQUIRE(toDelete!=del, "The production wasn't deleted correctly");
    }
}

/**
 * Constructor where a file name to a json file is given, the CFG will then be parsed from this file
 * @param json_filename a string representing the filepath
 */
CFG::CFG(const std::string& json_filename)
{
    std::ifstream i(json_filename);
    json j;
    i >> j;
    try {
        for (const auto& it : j["Variables"].items())
            m_variables.push_back(it.value());
        REQUIRE(!m_variables.empty() and m_variables[0]!="", "Can't have no variables");
    }
    catch (nlohmann::detail::type_error& e) {
        std::cerr << "Did not find \"Variables\" in json file" << std::endl
                  << e.what() << std::endl;
    }
    try {
        for (const auto& it : j["Terminals"].items()) {
            if (it.value().size()==1) {
                m_terminals.push_back(it.value());
            }
            else {
                if (it.value()["type"]=="alphabet") {
                    //"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"
                    m_variables.push_back(it.value()["symbol"]);
                    m_referenceTable[it.value()["symbol"]] = {};
                    for (char alpha = 97; alpha<123; alpha++) {
                        std::string temp = std::string(1, alpha);
                        m_terminals.push_back(temp);
                        m_referenceTable[it.value()["symbol"]].push_back(temp);
                        m_productions.push_back(CFGProduction(it.value()["symbol"], {temp, it.value()["symbol"]}));
                    }
                    m_productions.push_back(CFGProduction(it.value()["symbol"], {}));
                }
                else if (it.value()["type"]=="numbers") {
                    //"0","1","2","3","4","5","6","7","8","9"
                    m_variables.push_back(it.value()["symbol"]);
                    m_referenceTable[it.value()["symbol"]] = {};
                    for (char alpha = 48; alpha<58; alpha++) {
                        std::string temp = std::string(1, alpha);
                        m_terminals.push_back(temp);
                        m_referenceTable[it.value()["symbol"]].push_back(temp);
                        m_productions.push_back(CFGProduction(it.value()["symbol"], {temp, it.value()["symbol"]}));
                    }
                    m_terminals.push_back(".");
                    m_referenceTable[it.value()["symbol"]].push_back(".");
                    m_productions.push_back(CFGProduction(it.value()["symbol"], {".", it.value()["symbol"]}));
                    m_productions.push_back(CFGProduction(it.value()["symbol"], {}));
                }
            }
        }
    }
    catch (nlohmann::detail::type_error& e) {
        std::cerr << "Did not find \"Terminals\" in json file" << std::endl
                  << e.what() << std::endl;
    }
    try {
        for (const auto& it : j["Productions"].items()) {
            std::string head = it.value()["head"];
            std::vector<std::string> body;
            for (const auto& it2 : it.value()["body"].items()) {
                body.push_back(it2.value());
            }
            REQUIRE(!body.empty(), "There was no body given for the production");
            REQUIRE(head!="", "There was no head given for the production");
            std::string action = it.value()["action"];
//            std::string action="";
            m_productions.push_back(CFGProduction(head, body, action));
        }
    }
    catch (nlohmann::detail::type_error& e) {
        std::cerr << "Did not find correct formatted \"Productions\" in json file, does it exist with "
                     "\"head\" and \"body\" and \"action\"?" << std::endl << e.what() << std::endl;
    }
    try {
        m_start = j["Start"];
        REQUIRE(m_start!="", "The startsymbol cant be empty");
    }
    catch (nlohmann::detail::type_error& e) {
        std::cerr << "Did not find \"Terminals\" in json file" << std::endl
                  << e.what() << std::endl;
    }
    i.close();
    j.clear();
}

/**
 * Ostream operator for cfg
 * @param os the ostream object its wirten to
 * @param cfg the cfg that will be displayed
 * @return the ostream object with the cfg written to
 */
std::ostream& operator<<(std::ostream& os, CFG& cfg)
{
    os << "Variables: {";
    for (int i = 0; i<cfg.m_variables.size()-1; i++)
        os << cfg.m_variables[i] << ',' << ' ';
    os << cfg.m_variables.back() << '}' << std::endl;
    os << "Terminals: {";
    for (int i = 0; i<cfg.m_terminals.size()-1; i++)
        os << cfg.m_terminals[i] << ',' << ' ';
    os << cfg.m_terminals.back() << '}' << std::endl;
    os << "Production Rules:\n";
    for (CFGProduction& production : cfg.m_productions) {
        os << '\t' << production.getHead() << " -> ";
        for (const std::string& str : production.getBody())
            os << str;
        os << std::endl;
    }
    return os;
}

/**
 * Function that checks whether a string is a variable or not
 * @param toCheck String that is going to be checked
 * @return bool
 */
bool CFG::isVariable(const std::string& toCheck) const
{
    bool toreturn = false;
    for (auto i:m_variables) {
        if (i==toCheck) {
            toreturn = true;
        }
    }
    return toreturn;
}

/**
 * Function that checks wether a string is a terminal or not
 * @param toCheck a string that is going to be checked
 * @return a bool
 */
bool CFG::isTerminal(const std::string& toCheck) const
{
    bool toreturn = false;
    for (auto i:m_terminals) {
        if (i==toCheck) {
            toreturn = true;
        }
    }
    return toreturn;
}

/**
 * Function that gets all the production starting with the given variable
 * @param variable a string representing a variable
 * @return a vector of productions starting with that variable
 */
std::vector<CFGProduction> CFG::getProductionWithVariable(const std::string& variable) const
{
    std::vector<CFGProduction> toreturn;
    for (auto& i:m_productions) {
        if (i.getHead()==variable) {
            toreturn.push_back(i);
        }
    }
    return toreturn;
}

/**
 * Function that checks whether a variable has epsilon productions or not
 * @param var a string representing the var you are checking for
 * @return true if the production has one or more epsilon production(s) false else wise
 */
bool CFG::varHasEpsilonProduction(const std::string& var) const
{
    for (auto i:getProductionWithVariable(var)) {
        if (i.getBody().empty()) {
            return true;
        }
    }
    return false;
}

/**
 * Function that gets all the production going to a certain vector of string
 * @param tocheck the vector that you are checking if a variable will go to
 * @return a vector of string representing the variables that the previous vector goes to
 */
std::vector<std::string> CFG::getProductionsGoingTo(const std::vector<std::string>& tocheck) const
{
    std::vector<std::string> productions;
    for (auto i:m_productions) {
        if (i.getBody()==tocheck) {
            productions.emplace_back(i.getHead());
        }
    }
    return productions;
}

/**
 * Function that gets all the productions a certain string that is made trough the reference table goed to
 * @param symbol a string representing the symbol you are getting form the reference table
 * @return a vector of strings that each contain a string the reference symbol can go to
 */
std::vector<std::string> CFG::getFromReferenceTable(const std::string& symbol) const
{
    return m_referenceTable[symbol];
}

/**
 * Function that gets teh action a certain production does
 * @param from the head of the production
 * @param To the body of the production
 * @return a string representing the action the production does
 */
std::string CFG::getActionFromProduction(std::string from, std::vector<std::string> To) const
{
    for (CFGProduction i:m_productions) {
        if (i.getHead()==from && i.getBody()==To) {
            return i.getAction();
        }
    }
    return "";
}
