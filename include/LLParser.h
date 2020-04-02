#include <utility>

#ifndef MEBTO_LLPARSER_H
#define MEBTO_LLPARSER_H

#include "CFG.h"
#include "Game.h"
#include "EndObject.h"
#include <fstream>
#include <functional>
#include <streambuf>
#include <algorithm>
#include <map>
#include <memory>

using parseTable = std::map<std::string, std::map<std::vector<std::string>, std::vector<std::string>>>;

/**
 * This class is used for LL parser exceptions
 */
class LLExcept : public std::exception {
private:
    std::string _msg;

public:
    /**
     * Constructor for LL exception taking its error message as argument
     */
    explicit LLExcept(std::string msg) { _msg = std::move(msg); }

    /**
     * Throw function
     * @return C-string with error message
     */
    const char* what() const noexcept override {
        return _msg.c_str();
    }
};

/**
 * This class implements the LL parser functionality
 */
class LLParser {
public:
    /**
     * Constructor for the LL parser where the cfg is given.
     * @param cfg The cfg that is used for the parse table of the LL parser.
     */
    LLParser(const CFG& cfg, int lookahead );

    /**
     * Function that parses a file with the LL process and returns a vector of actions another function must do.
     * @param input The string you are checking.
     * @return Vector of actions that can be easily parsed to the actual game.
     */
    std::vector<std::string> parse(std::string input) const;

    /**
     * Turns the current parse table into html.
     * @param onStream The stream you write to (should be a html file).
     */
    void parseTableToHTML(std::ostream& onStream) const;

    /**
     * Function that takes a path of a file then streams the whole file to the parse function as a string
     * and returns the parsed file as a game.
     * @param path The path of the file.
     * @return Shared pointer to a fully parsed game.
     * @throws LLExcept when a fault happens
     */
    std::shared_ptr<Game> parseFile(const std::string& path) const;

private:
    CFG m_CFG;

    /**
     * m_parseTable is mutable to circumvent the fact that the basic access operator of std::map "[]" is
     * not const thus is not able to read objects out of the map in const functions correctly.
     */
    mutable parseTable m_ParseTable;

    /**
     * The lookahead symbol for this parser. If the value is 2, the parser will look at the next 2 input symbols to
     * determine the correct production.
     * This class has not been tested beyond 2 lookahead symbols.
     */
    int m_lookahead;

    /**
     * Helper function that makes the parse table with the current cfg for a LL(k) parser.
     *
     * This function uses multi-threading so the speed may differ depending on the amount of threads your processor has.
     */
    void makeParseTable();


    /**
     * Helper (recursive) function that adds the row of terminals at the top of the file.
     * @param onStream The file you are writing to.
     * @param current Helping variable that is passed to the next call of the function.
     * @param lookahead Depth of the recursion.
     */
    void HTMlAddTerminals(std::ostream& onStream, std::vector<std::string> current, int lookahead) const;

    /**
     * Helper function that adds the production of 1 symbol to the file.
     * @param onStream The html file you are writing to.
     * @param current The string that is recursively given.
     * @param lookahead Remaining depth of the recursion.
     * @param symbol Symbol you are getting the productions for.
     */
    void HTMLAddSymbolTerminals(std::ostream& onStream, std::vector<std::string> current, int lookahead,
            std::string symbol) const;

    /**
     * This function takes a variable from the stack and the
     * terminal you read of the input and will give you the production you should use.
     * @param terminal The first next terminal from the input.
     * @param variable Variable you are trying to turn into the terminal.
     * @return String containing the production you should use.
     */
    std::vector<std::string> getMatchingProduction(const std::vector<std::string>& terminal, std::string& variable) const;

    /**
     * Function that checks if the first character of a given string is a variable.
     * @param toCheck The string you are checking on.
     * @return Bool representing whether it is a variable or not.
     */
    std::string isFirstVariable(std::string toCheck) const;

    /**
     * Function that checks if the first symbol from a given string is a terminal.
     * @param toCheck The string you are checking on.
     * @return Bool representing whether it is a terminal or not.
     */
    std::string isFirstTerminal(string toCheck) const;

    /**
     * Function that checks all productions of a variable recursively and checks if
     * it ever goes to a certain terminal.
     * @param var Variable you are doing the check for.
     * @param terminal The terminal you are trying to turn the variable in.
     * @param alreadyChecked Helper vector to make sure you don't get stuck in an infinite loop.
     * @return Bool representing whether it goes to the terminal or not.
     */
    bool prodGoesToTerminals(std::vector<std::string> prod, std::vector<std::string> terminal) const;

    /**
     * This is a recursive function that makes sure all possible terminal combinations are checked for the lookahead.
     * @param variable The variable you are checking to.
     * This function changes the parse table so that a var has a complete roster of possible productions.
     */
    void setMatchingProductions(std::string variable, int lookahead, std::vector<std::string> current = {});

    /**
     * Function that splits a string into a vector of terminals.
     * @param toSplit String to split.
     * @return Vector of std::strings where each string is a single terminal.
     */
    std::vector<std::string> splitString(std::string toSplit) const;

    /**
     * Helper function that concatenates a vector of strings to one string.
     * @param vec The vector of strings.
     * @return String containing all the strings of the vec
     */
    inline std::string concatVec(const std::vector<std::string>& vec) const;

    /**
     * Helper function that parses the special variables (variable and number) at runtime
     * so it's possible to give a string as input.
     * @param in Current state of the input, after the function all the chars of the special string will be popped.
     * @param Var Variable that is the special character.
     * @return String that represents the character that were part of the special string.
     */
    std::string parseReferenceVar(std::vector<std::string>& in, const std::string& Var) const;

    /**
     * Function that takes the partially parsed vector of strings from the parse function and turns
     * it into a shared ptr of a game.
     * @param output The vector of strings that the parse function produces.
     * @return Shared_ptr of game that contains everything that was in the file.
     */
    std::shared_ptr<Game> parseLLoutput(std::vector<std::string> output) const;

    /**
     * Function that actually start the parsing process of the game.
     * @param output Vector of strings were the first index contains the name of the game.
     * @return Shared pointer to a fully parsed game.
     */
    std::shared_ptr<Game> parseGame(std::vector<std::string>& output) const;

    /**
     * Function that parses an object and all its variables recursively.
     * @param output Vector of string were the first index is the name of the object.
     * @return Shared pointer to a fully parsed object.
     */
    std::shared_ptr<Object> parseObject(std::vector<std::string>& output,double parentX=0, double parentY=0) const;

    /**
     * Function that parses an entity and all its variables.
     * @param output Vector of string were the first index is the name of the entity.
     * @return Shared pointer to a fully parsed entity
     */
    std::shared_ptr<Entity> parseEntity(std::vector<std::string>& output) const;

    /**
     * Function that parses a level and all its variables.
     * @param output Vector of string were the first index is the name of the level.
     * @return Shared pointer to a fully parsed level.
     */
    std::shared_ptr<Level> parseLevel(std::vector<std::string>& output) const;

    /**
     * Function that parses an end object for a level and all its variables.
     * @param output Vector of strings were the first index is the name of the end object.
     * @return Shared pointer to a fully parsed en object.
     */
    std::shared_ptr<EndObject> parseEnd(std::vector<std::string>& output) const;

    /**
     * Function that turns a string to a double where there can be a single "." to denote decimal numbers.
     * @param str The string that is turned into a double.
     * @return Double
     */
    double specialStringToDouble(std::string& str) const;

};

#endif //MEBTO_LLPARSER_H
