#include <fstream>

#include "CFG.h"
#include "json.hpp"

using json = nlohmann::json;


const std::vector<std::string> &CFG::getVariables() const {
    return m_variables;
}

void CFG::setVariables(const std::vector<std::string> &variables) {
    CFG::m_variables = variables;
}

const std::vector<std::string> &CFG::getTerminals() const {
    return m_terminals;
}

void CFG::setTerminals(const std::vector<std::string> &terminals) {
    CFG::m_terminals = terminals;
}

const std::vector<CFGProduction> &CFG::getProductions() const {
    return m_productions;
}

void CFG::setProductions(const std::vector<CFGProduction> &productions) {
    CFG::m_productions = productions;
}

const std::string &CFG::getStart() const {
    return m_start;
}

void CFG::setStart(const std::string &start) {
    CFG::m_start = start;
}


CFG::CFG( const std::vector<std::string> &variables,
         const std::vector<std::string> &terminals, const std::vector<CFGProduction> &productions,
         const std::string &start) : m_variables(variables), m_terminals(terminals),
                                       m_productions(productions), m_start(start) {}

CFG::CFG() {}

CFG::~CFG() {}

void CFG::addVariable(const std::string &toAdd) {
    m_variables.push_back(toAdd);
}

void CFG::addTerminal(const std::string &toAdd) {
    m_terminals.push_back(toAdd);
}

void CFG::addProduction(const CFGProduction &toAdd) {
    m_productions.push_back(toAdd);
}

void CFG::deleteVariable(const std::string &toDelete) {
    for(unsigned int i=0;i<m_variables.size();i++){
        if(m_variables[i]==toDelete){
            m_variables.erase(m_variables.begin()+i);
        }
    }
}

void CFG::deleteVariable(const int todelete) {
    m_variables.erase(m_variables.begin()+todelete);
}

void CFG::deleteTerminal(const std::string &toDelete) {
    for(unsigned int i=0;i<m_terminals.size();i++){
        if(m_terminals[i]==toDelete){
            m_terminals.erase(m_terminals.begin()+i);
        }
    }
}

void CFG::deleteTerminal(const int toDelete) {
    m_terminals.erase(m_terminals.begin()+toDelete);
}

void CFG::deleteProduction(const int toDelete) {
    m_productions.erase(m_productions.begin()+toDelete);
}


void CFG::deleteProduction(const CFGProduction &toDelete) {
    for(unsigned int i=0;i<m_productions.size();i++){
        if(m_productions[i]==toDelete){
            m_productions.erase(m_productions.begin()+i);
        }
    }
}

CFG::CFG(const std::string &json_filename) {
    std::ifstream i(json_filename);
    json j;
    i >> j;
    try {
        for (const auto &it : j["Variables"].items())
            m_variables.emplace_back(it.value());
    }catch (nlohmann::detail::type_error& e){
        std::cerr << "Did not find \"Variables\" in json file" << std::endl
                    << e.what() << std::endl;
    }
    try {
        for(const auto& it : j["Terminals"].items())
            m_terminals.emplace_back(it.value());
    }catch(nlohmann::detail::type_error& e){
        std::cerr << "Did not find \"Terminals\" in json file" << std::endl
                  << e.what() << std::endl;
    }
    try {
        for(const auto& it : j["Productions"].items()){
            std::string head = it.value()["head"];
            std::vector<std::string> body;
            for(const auto& it2 : it.value()["body"].items()){
                body.emplace_back(it2.value());
            }
            m_productions.emplace_back(CFGProduction(head, body));
        }
    }catch (nlohmann::detail::type_error& e){
        std::cerr << "Did not find correct formatted \"Productions\" in json file, does it exist with "
                     "\"head\" and \"body\"?" << std::endl << e.what() << std::endl;
    }
    try {
        m_start = j["Start"];
    }catch (nlohmann::detail::type_error& e){
        std::cerr << "Did not find \"Terminals\" in json file" << std::endl
                  << e.what() << std::endl;
    }
    i.close();
    j.clear();
}

CNF::CNF(const std::string &json_filename) : CFG(json_filename) {}

std::ostream &operator<<(std::ostream &os, CFG &cfg) {
    os << "Variables: {";
    for(int i = 0; i<cfg.m_variables.size() - 1; i++)
        os << cfg.m_variables[i] << ',' << ' ';
    os << cfg.m_variables.back() << '}' << std::endl;
    os << "Terminals: {";
    for(int i = 0; i<cfg.m_terminals.size() - 1; i++)
        os << cfg.m_terminals[i] << ',' << ' ';
    os << cfg.m_terminals.back() << '}' << std::endl;
    os << "Production Rules:\n";
    for(CFGProduction& production : cfg.m_productions){
        os << '\t' <<production.getHead() << " -> ";
        for(const std::string& str : production.getBody())
            os << str;
        os << std::endl;
    }
    return os;
}
