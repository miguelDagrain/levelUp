#ifndef LEVELUP_CFG_H
#define LEVELUP_CFG_H

#include <iostream>
#include <vector>
#include <map>
#include "CFGProduction.h"

class CFG {
public:
    /**
     * Constructor for CFG
     * @param variables a vector of strings representing the variables
     * @param terminals a vector of string representing the terminals
     * @param productions a vector of CFG productions
     * @param start a string representing the starting variable
     */
    CFG(const std::vector<std::string>& variables,
            const std::vector<std::string>& terminals, const std::vector<CFGProduction>& productions,
            const std::string& start);

    /**
     * Default constructor
     */
    CFG();

    /**
     * Constructor where a file name to a json file is given, the CFG will then be parsed from this file
     * @param json_filename a string representing the filepath
     */
    explicit CFG(const std::string& json_filename);

    /**
     * Getter for the variables
     * @return a vector of string representing the variables
     */
    const std::vector<std::string>& getVariables() const;

    /**
     * Setter for the variables.
     * @param variables a vector of strings.
     */
    void setVariables(const std::vector<std::string>& variables);

    /**
     * Getter for the terminals.
     * @return a vector of strings representing the terminals.
     */
    const std::vector<std::string>& getTerminals() const;

    /**
     * Setter for the terminals
     * @param terminals a vector of strings
     */
    void setTerminals(const std::vector<std::string>& terminals);

    /**
     * Getter for productions.
     * @return a vector of CFG productions
     */
    const std::vector<CFGProduction>& getProductions() const;

    /**
     * Setter for productions
     * @param productions a vector of CFG productions
     */
    void setProductions(const std::vector<CFGProduction>& productions);

    /**
     * Getter for the start symbol
     * @return a string representing the start symbol
     */
    const std::string& getStart() const;

    /**
     * Setter for the start symbol
     * @param start a string that will become the start symbol
     */
    void setStart(const std::string& start);

    /**
     * Function that adds a variable to the vector
     * @param toAdd a string representing a variable
     */
    void addVariable(const std::string& toAdd);

    /**
     * Function that adds a terminal to the vector
     * @param toAdd String representing a terminal
     */
    void addTerminal(const std::string& toAdd);

    /**
     * Function that adds a production the the vector
     * @param toAdd a cfgproduction that will be added
     */
    void addProduction(const CFGProduction& toAdd);

    /**
     * Function that deletes a variable out of the vector
     * @param toDelete String that will be deleted
     */
    void deleteVariable(const std::string& toDelete);

    /**
     * Function that deletes a variable at a given index
     * @param toDelete Index of the place you want to delete
     */
    void deleteVariable(int toDelete);

    /**
     * Function that deletes a variable terminal out of the vector
     * @param toDelete a string that will be deleted
     */
    void deleteTerminal(const std::string& toDelete);

    /**
     * Function that deletes a terminal with an index
     * @param toDelete an index of the place you want to delete
     */
    void deleteTerminal(int toDelete);

    /**
     * Function that deletes a production with an index
     * @param toDelete an index of the place you want to delete
     */
    void deleteProduction(int toDelete);

    /**
    * Function that deletes a production out of the vector
    * @param toDelete Production that will be deleted
    */
    void deleteProduction(const CFGProduction& toDelete);

    /**
     * Destructor for cfg
     */
    virtual ~CFG();

    /**
     * Ostream operator for cfg
     * @param os the ostream object its wirten to
     * @param cfg the cfg that will be displayed
     * @return the ostream object with the cfg written to
     */
    friend std::ostream& operator<<(std::ostream& os, CFG& cfg);

    /**
     * Function that checks whether a string is a variable or not
     * @param toCheck String that is going to be checked
     * @return bool
     */
    bool isVariable(const std::string& toCheck) const;

    /**
     * Function that checks wether a string is a terminal or not
     * @param toCheck a string that is going to be checked
     * @return a bool
     */
    bool isTerminal(const std::string& toCheck) const;

    /**
     * Function that gets all the production starting with the given variable
     * @param variable a string representing a variable
     * @return a vector of productions starting with that variable
     */
    std::vector<CFGProduction> getProductionWithVariable(const std::string& variable) const;

    /**
     * Function that checks whether a variable has epsilon productions or not
     * @param var a string representing the var you are checking for
     * @return true if the production has one or more epsilon production(s) false else wise
     */
    bool varHasEpsilonProduction(const std::string& var) const;

    /**
     * Function that gets all the production going to a certain vector of string
     * @param tocheck the vector that you are checking if a variable will go to
     * @return a vector of string representing the variables that the previous vector goes to
     */
    std::vector<std::string> getProductionsGoingTo(const std::vector<std::string>& tocheck) const;

    /**
     * Function that gets all the productions a certain string that is made trough the reference table goed to
     * @param symbol a string representing the symbol you are getting form the reference table
     * @return a vector of strings that each contain a string the reference symbol can go to
     */
    std::vector<std::string> getFromReferenceTable(const std::string& symbol) const;

    /**
     * Function that gets teh action a certain production does
     * @param from the head of the production
     * @param To the body of the production
     * @return a string representing the action the production does
     */
    std::string getActionFromProduction(std::string from,std::vector<std::string> To) const;

private:
    std::vector<std::string> m_variables;
    std::vector<std::string> m_terminals;
    std::vector<CFGProduction> m_productions;
    std::string m_start;
    mutable std::map<std::string, std::vector<std::string> > m_referenceTable;
};

/**
 * Ostream operator for cfg
 * @param os the ostream object its wirten to
 * @param cfg the cfg that will be displayed
 * @return the ostream object with the cfg written to
 */
std::ostream& operator<<(std::ostream& os, CFG& cfg);


#endif //LEVELUP_CFG_H
