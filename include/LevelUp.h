#ifndef LEVELUP_LEVELUP_H
#define LEVELUP_LEVELUP_H

#include <iostream>
#include <memory>
#include <chrono>
#include <fstream>
#include <sys/stat.h>
#include "LLParser.h"
#include "GameObjectToSFML.h"
#include "GameObject.h"

class LevelUpExcept : public std::exception {
private:
    std::string _msg;

public:
    /**
     * \brief Default constructor
     */
    explicit LevelUpExcept(std::string msg) { _msg = std::move(msg); }

    /**
     * \brief Throw function
     * @return C-string with error message
     */
    const char* what() const noexcept override
    {
        return _msg.c_str();
    }
};

class LevelUp {
public:
    /**
     * this file parses the arguments given
     * @param argc amount of arguments
     * @param argv the arguments
     *
     * The first argument must always be a filename were the level is
     * Second argument can be -debug to run debug mode were output is given from ll parser and Turing machines
     */
    static void parseArguments(int argc, char* argv[]);

    /**
     * This function checks if all files for the Turing machine and ll parser exist and if not they don't then a function is called to create them
     */
    static void checkFiles();

    /**
     * This function actually runs and parses the game file
     * @param levelFilePath the path the the file that contains the game
     * @param debug a bool that say whether output from llparser and turing machine must be given or not
     */
    static void runGame(std::string levelFilePath, bool debug);

private:
    //here are a bunch of private function that each make a single file for tm or llparser
    /**
     * Creates the LL parse file
     */
    static void createLLParseFile();

    /**
     *
     */
    static void createTMSAX();

    /**
     *
     */
    static void createTMSAY();

    /**
     *
     */
    static void createTMSCX();

    /**
     *
     */
    static void createTMSCY();

    /**
     *
     */
    static void createTMSSX();

    /**
     *
     */
    static void createTMSSY();

    /**
     *
     */
    static void createTMCVM();

    /**
     * Creates main Turing machine
     */
    static void createTMmain();

};

#endif //LEVELUP_LEVELUP_H
