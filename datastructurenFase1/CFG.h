#ifndef LEVELUP_CFG_H
#define LEVELUP_CFG_H

#include <iostream>
#include <vector>
#include "CFGProduction.h"

class CFG {
public:
    CFG(const std::vector<std::string> &variables,
        const std::vector<std::string> &terminals, const std::vector<CFGProduction> &productions,
        const std::string &start);

    CFG();

    explicit CFG(const std::string& json_filename);

    const std::vector<std::string> &getVariables() const;
    void setVariables(const std::vector<std::string> &variables);

    const std::vector<std::string> &getTerminals() const;

    void setTerminals(const std::vector<std::string> &terminals);

    const std::vector<CFGProduction> &getProductions() const;

    void setProductions(const std::vector<CFGProduction> &productions);

    const std::string &getStart() const;

    void setStart(const std::string &start);

    void addVariable(const std::string& toAdd);

    void addTerminal(const std::string& toAdd);

    void addProduction(const CFGProduction& toAdd);

    void deleteVariable(const std::string& toDelete);

    void deleteVariable(int todelete);

    void deleteTerminal(const std::string& toDelete);

    void deleteTerminal(int toDelete);

    void deleteProduction(int toDelete);

    void deleteProduction(const CFGProduction &toDelete);

    virtual ~CFG();

    friend std::ostream& operator<<(std::ostream& os, CFG& cfg);



private:
    std::vector<std::string> m_variables;

    std::vector<std::string> m_terminals;

    std::vector<CFGProduction> m_productions;

    std::string m_start;
};

std::ostream& operator<<(std::ostream& os, CFG& cfg);

class CNF: public CFG{
public:
    explicit CNF(const std::string &json_filename);

    CNF() = default;
};


#endif //LEVELUP_CFG_H
