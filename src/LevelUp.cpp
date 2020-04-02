#include "LevelUp.h"

/**
 * this file parses the arguments given
 * @param argc amount of arguments
 * @param argv the arguments
 *
 * The first argument must always be a filename were the level is
 * Second argument can be -debug to run debug mode were output is given from ll parser and Turing machines
 */
void LevelUp::parseArguments(int argc, char* argv[])
{
    if (argc>1) {
        if (argc>2) {
            if (strcmp(argv[2], "-debug")==0) {
                runGame(argv[1], true);
                return;
            }
        }
        runGame(argv[1], false);
        return;
    }
    else {
        throw LevelUpExcept("not enough arguments given");
    };
}

/**
 * This function actually runs and parses the game file
 * @param levelFilePath the path the the file that contains the game
 * @param debug a bool that say whether output from llparser and turing machine must be given or not
 */
void LevelUp::runGame(std::string levelFilePath, bool debug)
{

    checkFiles();
    auto start = std::chrono::system_clock::now();
    struct stat buffer{};
    bool llExists(stat(levelFilePath.c_str(), &buffer)==0);
    if (!llExists) throw LevelUpExcept("given game file doesnt exist");

    CFG LLcfg = CFG("llParseFile.json");
    LLParser parser(LLcfg, 2);
    std::shared_ptr<Game> game;
    try {
        game = parser.parseFile(levelFilePath);
    }
    catch (LLExcept& e) {
        std::string temp = e.what();
        throw LevelUpExcept("Exception trown in LLParser: "+temp);
    }
    std::string file_TM_check = "./TuringMachine_main.json";
    TuringMachine check_valid(const_cast<char*>(file_TM_check.c_str()));

    std::string file_TM_update_loc = "./TuringMachine_change_value_main.json";
    TuringMachine update_loc(const_cast<char*>(file_TM_update_loc.c_str()));

    if (debug) {
        std::ofstream output("output/ll.html");
        parser.parseTableToHTML(output);
        output.close();
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        std::cout << "the parsing takes: " << elapsed_seconds.count() << "s\n";

        std::ofstream dot_of_TM_check("./output/TM_check.gv");
        check_valid.to_dot(dot_of_TM_check);

        std::ofstream dot_of_TM_update("./output/TM_change_value.gv");
        update_loc.to_dot(dot_of_TM_update);
    }
    if (game==nullptr)throw LevelUpExcept("game was not initialized properly");

    Gui::run_game(game, check_valid, update_loc);

}

/**
 * this function checks if all files for the Turing machine and ll parser exist and if not they don't then a function is called to create them
 */
void LevelUp::checkFiles()
{
    //checking if file exists found at https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    struct stat buffer{};
    bool llExists(stat("llParseFile.json", &buffer)==0);
    if (!llExists) createLLParseFile();

    bool saxExists(stat("Subroutine_add_x.json", &buffer)==0);
    if (!saxExists) createTMSAX();

    bool sayExists(stat("Subroutine_add_y.json", &buffer)==0);
    if (!sayExists) createTMSAY();

    bool scxExists(stat("Subroutine_check_x.json", &buffer)==0);
    if (!scxExists) createTMSCX();

    bool scyExists(stat("Subroutine_check_y.json", &buffer)==0);
    if (!scyExists) createTMSCY();

    bool ssxExists(stat("Subroutine_subtract_x.json", &buffer)==0);
    if (!ssxExists) createTMSSX();

    bool ssyExists(stat("Subroutine_subtract_y.json", &buffer)==0);
    if (!ssyExists) createTMSSY();

    bool tcvmExists(stat("TuringMachine_change_value_main.json", &buffer)==0);
    if (!tcvmExists) createTMCVM();

    bool tmExists(stat("TuringMachine_main.json", &buffer)==0);
    if (!tmExists) createTMmain();
}

void LevelUp::createLLParseFile()
{
    std::ofstream jsonFile("llParseFile.json");

    jsonFile << "{\n"
                "\t\"Variables\": [\"Q000\",\"Q001\",\"Q002\",\"Q003\",\"Q004\",\"Q005\",\"Q006\",\"Q007\",\"Q008\",\"Q009\",\"Q010\",\"Q011\",\"Q012\",\"Q013\",\"Q014\",\"Q015\"],\n"
                "\t\"Terminals\": [\"(\",\")\",\"[\",\"]\",\"{\",\"}\",\"!\",\",\",\":\",\"+\",\n"
                "\t\t{\"symbol\": \"@\",\"type\":\"alphabet\"},\n"
                "\t\t{\"symbol\":\"€\",\"type\":\"numbers\"}\n"
                "\t],\n"
                "\t\"Productions\": [\n"
                "\t\t{\"head\": \"Q000\", \"body\": [\"€\"],\"action\":\"parseSpecial\"},\n"
                "\t\t{\"head\": \"Q001\", \"body\": [\"@\"],\"action\":\"parseSpecial\"},\n"
                "\t\t{\"head\": \"Q009\", \"body\": [\"t\",\"r\",\"u\",\"e\"],\"action\":\"true\"},\n"
                "\t\t{\"head\": \"Q009\", \"body\": [\"f\",\"a\",\"l\",\"s\",\"e\"],\"action\":\"false\"},\n"
                "\n"
                "\t\t{\"head\": \"Q002\", \"body\": [\"Q003\",\"{\",\"Q012\",\"Q011\"],\"action\":\"\"},\n"
                "\t\t{\"head\": \"Q003\", \"body\": [\"Q001\",\"[\",\"g\",\"a\",\"m\",\"e\",\"]\"],\"action\":\"makeGame\"},\n"
                "\t\t{\"head\": \"Q012\", \"body\": [\"!\",\"Q012\",\",\",\"Q012\"],\"action\":\"\"},\n"
                "\n"
                "\t\t{\"head\": \"Q012\", \"body\": [\"Q001\",\"[\",\"l\",\"e\",\"v\",\"e\",\"l\",\"]\",\"{\",\"Q015\",\"Q011\"],\"action\":\"makeLevel\"},\n"
                "\t\t{\"head\": \"Q015\", \"body\": [\"!\",\"Q015\",\",\",\"Q015\"],\"action\":\"\"},\n"
                "\t\t{\"head\": \"Q015\", \"body\": [\"Q004\"],\"action\":\"\"},\n"
                "\t\t{\"head\": \"Q004\", \"body\": [\"!\",\"Q004\",\",\",\"Q004\"],\"action\":\"\"},\n"
                "\n"
                "\n"
                "        {\"head\": \"Q004\", \"body\": [\"+\",\"Q001\",\"Q005\"],\"action\":\"makeGameObject\"},\n"
                "\t\t{\"head\": \"Q004\", \"body\": [\"Q007\"],\"action\":\"\"},\n"
                "\t\t{\"head\": \"Q004\", \"body\": [\"Q013\"],\"action\":\"\"},\n"
                "\n"
                "\n"
                "        {\"head\": \"Q014\", \"body\": [\"g\",\"o\",\"t\",\"o\",\":\",\"Q001\"],\"action\":\"setGoto\"},\n"
                "\t\t{\"head\": \"Q005\", \"body\": [\"[\",\"c\",\"o\",\"n\",\"t\",\"i\",\"n\",\"u\",\"e\",\"]\",\"{\",\"Q014\",\"Q011\"],\"action\":\"makeEnd\"},\n"
                "\t\t{\"head\": \"Q014\", \"body\": [\"!\",\"Q014\",\",\",\"Q014\"],\"action\":\"\"},\n"
                "\t\t{\"head\": \"Q014\", \"body\": [\"Q007\"],\"action\":\"\"},\n"
                "\t\t{\"head\": \"Q014\", \"body\": [\"Q010\"],\"action\":\"\"},\n"
                "\n"
                "\n"
                "\n"
                "\t\t{\"head\": \"Q005\", \"body\": [\"[\",\"e\",\"n\",\"t\",\"i\",\"t\",\"y\",\"]\",\"{\",\"Q006\",\"Q011\"],\"action\":\"makeEntity\"},\n"
                "\t\t{\"head\": \"Q006\", \"body\": [\"!\",\"Q006\",\",\",\"Q006\"],\"action\":\"\"},\n"
                "\t\t{\"head\": \"Q006\", \"body\": [\"Q007\"],\"action\":\"\"},\n"
                "\t\t{\"head\": \"Q006\", \"body\": [\"Q008\"],\"action\":\"\"},\n"
                "\n"
                "        {\"head\": \"Q005\", \"body\": [\"Q012\"],\"action\":\"\"},\n"
                "        {\"head\": \"Q012\", \"body\": [\"[\",\"o\",\"b\",\"j\",\"e\",\"c\",\"t\",\"]\",\"{\",\"Q013\",\"Q011\"],\"action\":\"makeObject\"},\n"
                "        {\"head\": \"Q013\", \"body\": [\"!\",\"Q013\",\",\",\"Q013\"],\"action\":\"\"},\n"
                "        {\"head\": \"Q013\", \"body\": [\"Q007\"],\"action\":\"\"},\n"
                "        {\"head\": \"Q013\", \"body\": [\"Q010\"],\"action\":\"\"},\n"
                "        {\"head\": \"Q013\", \"body\": [\"+\",\"Q001\",\"[\",\"o\",\"b\",\"j\",\"e\",\"c\",\"t\",\"]\",\"{\",\"Q013\",\"Q011\"],\"action\":\"makeObject\"},\n"
                "\n"
                "        {\"head\": \"Q007\", \"body\": [\"w\",\"i\",\"d\",\"t\",\"h\",\":\",\"Q000\"],\"action\":\"setWidth\"},\n"
                "\t\t{\"head\": \"Q007\", \"body\": [\"h\",\"e\",\"i\",\"g\",\"h\",\"t\",\":\",\"Q000\"],\"action\":\"setHeight\"},\n"
                "\t\t{\"head\": \"Q007\", \"body\": [\"c\",\"o\",\"l\",\"o\",\"r\",\":\",\"(\",\"Q000\",\",\",\"Q000\",\",\",\"Q000\",\")\"],\"action\":\"setColor\"},\n"
                "\n"
                "\t\t{\"head\": \"Q008\", \"body\": [\"l\",\"o\",\"c\",\"a\",\"t\",\"i\",\"o\",\"n\",\":\",\"(\",\"Q000\",\",\",\"Q000\",\")\"],\"action\":\"setLocation\"},\n"
                "\t\t{\"head\": \"Q008\", \"body\": [\"p\",\"a\",\"t\",\"h\",\"i\",\"n\",\"g\",\":\",\"Q001\"],\"action\":\"setPathing\"},\n"
                "\t\t{\"head\": \"Q008\", \"body\": [\"t\",\"e\",\"x\",\"t\",\"u\",\"r\",\"e\",\":\",\"Q001\"],\"action\":\"setTexture\"},\n"
                "\t\t{\"head\": \"Q008\", \"body\": [\"j\",\"u\",\"m\",\"p\",\"h\",\"e\",\"i\",\"g\",\"h\",\"t\",\":\",\"Q000\"],\"action\":\"setJump\"},\n"
                "\t\t{\"head\": \"Q008\", \"body\": [\"m\",\"o\",\"v\",\"e\",\"m\",\"e\",\"n\",\"t\",\":\",\"Q000\"],\"action\":\"setMovement\"},\n"
                "\n"
                "\t\t{\"head\": \"Q010\", \"body\": [\"h\",\"i\",\"t\",\"b\",\"o\",\"x\",\":\",\"Q009\"],\"action\":\"setCollision\"},\n"
                "\t\t{\"head\": \"Q010\", \"body\": [\"l\",\"o\",\"c\",\"a\",\"t\",\"i\",\"o\",\"n\",\":\",\"(\",\"Q000\",\",\",\"Q000\",\")\"],\"action\":\"setLocation\"},\n"
                "\t\t{\"head\": \"Q010\", \"body\": [\"t\",\"e\",\"x\",\"t\",\"u\",\"r\",\"e\",\":\",\"Q001\"],\"action\":\"setTexture\"},\n"
                "\t\t{\"head\": \"Q010\", \"body\": [\"Q001\",\"[\",\"o\",\"b\",\"j\",\"e\",\"c\",\"t\",\"]\",\"{\",\"Q013\",\"Q011\"],\"action\":\"makeObject\"},\n"
                "\t\t\n"
                "\n"
                "\t\t{\"head\": \"Q011\", \"body\": [\"}\"],\"action\":\"end\"}\n"
                "\n"
                "\n"
                "\t],\n"
                "\t\"Start\": \"Q002\"\n"
                "}" << std::endl;

    jsonFile.close();

}

void LevelUp::createTMSAX()
{
    std::ofstream File("Subroutine_add_x.json");

    File << "{\n"
            "  \"States\": [\"s_add_1\", \"s_add_2\", \"s_add_3\", \"s_add_4\", \"s_add_5\"],\n"
            "  \"InputAlphabet\": [\"1\", \"x\", \"X\", \"U\", \"2\", \"y\", \"Y\", \"3\", \"+\"],\n"
            "  \"TapeAlphabet\": [\"1\", \"x\", \"X\", \"U\", \"2\", \"y\", \"Y\", \"3\", \"+\", \"B\"],\n"
            "\n"
            "  \"Transitions\": [\n"
            "    {\"from\": \"s_add_1\", \"read\": [\"+\"], \"to\": \"s_add_1\", \"write\": [\"3\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_1\", \"read\": [\"3\"], \"to\": \"s_add_1\", \"write\": [\"Y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_1\", \"read\": [\"Y\"], \"to\": \"s_add_1\", \"write\": [\"Y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_1\", \"read\": [\"y\"], \"to\": \"s_add_2\", \"write\": [\"y\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"s_add_2\", \"read\": [\"Y\"], \"to\": \"s_add_2\", \"write\": [\"y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_2\", \"read\": [\"y\"], \"to\": \"s_add_2\", \"write\": [\"y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_2\", \"read\": [\"2\"], \"to\": \"s_add_3\", \"write\": [\"2\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"s_add_3\", \"read\": [\"y\"], \"to\": \"s_add_3\", \"write\": [\"2\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_3\", \"read\": [\"2\"], \"to\": \"s_add_3\", \"write\": [\"U\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_3\", \"read\": [\"U\"], \"to\": \"s_add_3\", \"write\": [\"X\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_3\", \"read\": [\"X\"], \"to\": \"s_add_3\", \"write\": [\"X\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_3\", \"read\": [\"x\"], \"to\": \"s_add_4\", \"write\": [\"x\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"s_add_3\", \"read\": [\"1\"], \"to\": \"s_add_4\", \"write\": [\"1\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"s_add_4\", \"read\": [\"X\"], \"to\": \"s_add_4\", \"write\": [\"x\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_4\", \"read\": [\"x\"], \"to\": \"s_add_4\", \"write\": [\"x\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_4\", \"read\": [\"1\"], \"to\": \"s_add_5\", \"write\": [\"1\"], \"direction\": [\"left\"]}\n"
            "  ],\n"
            "\n"
            "  \"StartState\": \"s_add_1\",\n"
            "  \"Blank\": \"B\",\n"
            "  \"FinalStates\": [\"s_add_5\"]\n"
            "}" << std::endl;

    File.close();
}

void LevelUp::createTMSAY()
{
    std::ofstream File("Subroutine_add_y.json");

    File << "{\n"
            "  \"States\": [\"s_add_y_1\", \"s_add_y_2\", \"s_add_y_3\"],\n"
            "  \"InputAlphabet\": [\"1\", \"x\", \"X\", \"U\", \"2\", \"y\", \"Y\", \"3\", \"+\"],\n"
            "  \"TapeAlphabet\": [\"1\", \"x\", \"X\", \"U\", \"2\", \"y\", \"Y\", \"3\", \"+\", \"B\"],\n"
            "\n"
            "  \"Transitions\": [\n"
            "    {\"from\": \"s_add_y_1\", \"read\": [\"+\"], \"to\": \"s_add_y_1\", \"write\": [\"3\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_y_1\", \"read\": [\"3\"], \"to\": \"s_add_y_1\", \"write\": [\"Y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_y_1\", \"read\": [\"Y\"], \"to\": \"s_add_y_1\", \"write\": [\"Y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_y_1\", \"read\": [\"y\"], \"to\": \"s_add_y_2\", \"write\": [\"y\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"s_add_y_1\", \"read\": [\"2\"], \"to\": \"s_add_y_2\", \"write\": [\"2\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"s_add_y_2\", \"read\": [\"Y\"], \"to\": \"s_add_y_2\", \"write\": [\"y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_y_2\", \"read\": [\"y\"], \"to\": \"s_add_y_2\", \"write\": [\"y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_y_2\", \"read\": [\"2\"], \"to\": \"s_add_y_2\", \"write\": [\"2\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_y_2\", \"read\": [\"U\"], \"to\": \"s_add_y_2\", \"write\": [\"U\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_y_2\", \"read\": [\"X\"], \"to\": \"s_add_y_2\", \"write\": [\"X\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_y_2\", \"read\": [\"x\"], \"to\": \"s_add_y_2\", \"write\": [\"x\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_add_y_2\", \"read\": [\"1\"], \"to\": \"s_add_y_3\", \"write\": [\"1\"], \"direction\": [\"left\"]}\n"
            "  ],\n"
            "\n"
            "  \"StartState\": \"s_add_y_1\",\n"
            "  \"Blank\": \"B\",\n"
            "  \"FinalStates\": [\"s_add_y_3\"]\n"
            "}" << std::endl;

    File.close();
}

void LevelUp::createTMSCX()
{
    std::ofstream File("Subroutine_check_x.json");

    File << "{\n"
            "  \"States\": [\"q1\", \"q2\", \"q3\", \"q4\", \"q13\", \"q14\"],\n"
            "  \"InputAlphabet\": [\"2\", \"x\", \"X\", \"U\", \"F\", \"P\"],\n"
            "  \"TapeAlphabet\": [\"2\", \"x\", \"X\", \"U\", \"F\", \"P\", \"B\"],\n"
            "\n"
            "  \"Tapes\": 2,\n"
            "\n"
            "  \"Transitions\": [\n"
            "    {\"from\": \"q1\", \"read\": [\"x\", \"x\"], \"to\": \"q1\", \"write\": [\"x\", \"x\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"x\", \"X\"], \"to\": \"q2\", \"write\": [\"x\", \"X\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"x\", \"X\"], \"to\": \"q2\", \"write\": [\"x\", \"X\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"X\", \"x\"], \"to\": \"q3\", \"write\": [\"X\", \"x\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q3\", \"read\": [\"X\", \"x\"], \"to\": \"q3\", \"write\": [\"X\", \"x\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"X\", \"2\"], \"to\": \"q4\", \"write\": [\"X\", \"2\"], \"direction\": [\"right\", \"stay\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"x\", \"2\"], \"to\": \"q4\", \"write\": [\"x\", \"2\"], \"direction\": [\"right\", \"stay\"]},\n"
            "    {\"from\": \"q3\", \"read\": [\"U\", \"X\"], \"to\": \"q4\", \"write\": [\"P\", \"X\"], \"direction\": [\"stay\", \"right\"]},\n"
            "    {\"from\": \"q3\", \"read\": [\"U\", \"x\"], \"to\": \"q4\", \"write\": [\"P\", \"x\"], \"direction\": [\"stay\", \"stay\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"P\", \"x\"], \"to\": \"q4\", \"write\": [\"P\", \"x\"], \"direction\": [\"stay\", \"right\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"P\", \"X\"], \"to\": \"q4\", \"write\": [\"P\", \"X\"], \"direction\": [\"stay\", \"right\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"x\", \"2\"], \"to\": \"q4\", \"write\": [\"x\", \"2\"], \"direction\": [\"right\", \"stay\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"X\", \"2\"], \"to\": \"q4\", \"write\": [\"X\", \"2\"], \"direction\": [\"right\", \"stay\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"U\", \"2\"], \"to\": \"q4\", \"write\": [\"P\", \"2\"], \"direction\": [\"stay\", \"stay\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"X\", \"X\"], \"to\": \"q13\", \"write\": [\"X\", \"X\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q3\", \"read\": [\"X\", \"X\"], \"to\": \"q13\", \"write\": [\"X\", \"X\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"X\", \"X\"], \"to\": \"q13\", \"write\": [\"X\", \"X\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"P\", \"2\"], \"to\": \"q14\", \"write\": [\"P\", \"2\"], \"direction\": [\"right\", \"stay\"]},\n"
            "    {\"from\": \"q13\", \"read\": [\"F\", \"2\"], \"to\": \"q14\", \"write\": [\"F\", \"2\"], \"direction\": [\"right\", \"stay\"]},\n"
            "    {\"from\": \"q13\", \"read\": [\"X\", \"X\"], \"to\": \"q13\", \"write\": [\"X\", \"X\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q13\", \"read\": [\"U\", \"X\"], \"to\": \"q13\", \"write\": [\"F\", \"X\"], \"direction\": [\"stay\", \"right\"]},\n"
            "    {\"from\": \"q13\", \"read\": [\"F\", \"X\"], \"to\": \"q13\", \"write\": [\"F\", \"X\"], \"direction\": [\"stay\", \"right\"]},\n"
            "    {\"from\": \"q13\", \"read\": [\"U\", \"2\"], \"to\": \"q13\", \"write\": [\"F\", \"2\"], \"direction\": [\"stay\", \"stay\"]},\n"
            "    {\"from\": \"q13\", \"read\": [\"X\", \"2\"], \"to\": \"q13\", \"write\": [\"X\", \"2\"], \"direction\": [\"right\", \"stay\"]}\n"
            "  ],\n"
            "\n"
            "  \"StartState\": \"q1\",\n"
            "  \"Blank\": \"B\",\n"
            "  \"FinalStates\": [\"q14\"]\n"
            "}" << std::endl;

    File.close();
}

void LevelUp::createTMSCY()
{
    std::ofstream File("Subroutine_check_y.json");

    File << "{\n"
            "  \"States\": [\"q5\", \"q6\", \"q7\", \"q8\"],\n"
            "  \"InputAlphabet\": [\"3\", \"y\", \"Y\"],\n"
            "  \"TapeAlphabet\": [\"3\", \"y\", \"Y\", \"B\"],\n"
            "\n"
            "  \"Tapes\": 2,\n"
            "\n"
            "  \"Transitions\": [\n"
            "    {\"from\": \"q5\", \"read\": [\"y\", \"y\"], \"to\": \"q5\", \"write\": [\"y\", \"y\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q5\", \"read\": [\"y\", \"Y\"], \"to\": \"q6\", \"write\": [\"y\", \"Y\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q6\", \"read\": [\"y\", \"Y\"], \"to\": \"q6\", \"write\": [\"y\", \"Y\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q5\", \"read\": [\"Y\", \"y\"], \"to\": \"q7\", \"write\": [\"Y\", \"y\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"Y\", \"y\"], \"to\": \"q7\", \"write\": [\"Y\", \"y\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q6\", \"read\": [\"y\", \"3\"], \"to\": \"q8\", \"write\": [\"y\", \"3\"], \"direction\": [\"stay\", \"stay\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"3\", \"y\"], \"to\": \"q8\", \"write\": [\"3\", \"y\"], \"direction\": [\"stay\", \"stay\"]},\n"
            "    {\"from\": \"q8\", \"read\": [\"3\", \"y\"], \"to\": \"q8\", \"write\": [\"3\", \"y\"], \"direction\": [\"stay\", \"right\"]},\n"
            "    {\"from\": \"q8\", \"read\": [\"3\", \"Y\"], \"to\": \"q8\", \"write\": [\"3\", \"Y\"], \"direction\": [\"stay\", \"right\"]},\n"
            "    {\"from\": \"q8\", \"read\": [\"y\", \"3\"], \"to\": \"q8\", \"write\": [\"y\", \"3\"], \"direction\": [\"right\", \"stay\"]},\n"
            "    {\"from\": \"q8\", \"read\": [\"Y\", \"3\"], \"to\": \"q8\", \"write\": [\"Y\", \"3\"], \"direction\": [\"right\", \"stay\"]}\n"
            "  ],\n"
            "\n"
            "  \"StartState\": \"q5\",\n"
            "  \"Blank\": \"B\",\n"
            "  \"FinalStates\": [\"q8\"]\n"
            "}" << std::endl;

    File.close();
}

void LevelUp::createTMSSX()
{
    std::ofstream File("Subroutine_subtract_x.json");

    File << "{\n"
            "  \"States\": [\"s_subtract_1\", \"s_subtract_2\", \"s_subtract_3\", \"s_subtract_4\", \"s_subtract_5\", \"s_subtract_6\", \"s_subtract_7\"],\n"
            "  \"InputAlphabet\": [\"1\", \"x\", \"X\", \"U\", \"2\", \"y\", \"Y\", \"3\", \"-\"],\n"
            "  \"TapeAlphabet\": [\"1\", \"x\", \"X\", \"U\", \"2\", \"y\", \"Y\", \"3\", \"-\", \"B\"],\n"
            "\n"
            "  \"Transitions\": [\n"
            "    {\"from\": \"s_subtract_1\", \"read\": [\"-\"], \"to\": \"s_subtract_1\", \"write\": [\"B\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_1\", \"read\": [\"3\"], \"to\": \"s_subtract_1\", \"write\": [\"B\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_1\", \"read\": [\"Y\"], \"to\": \"s_subtract_2\", \"write\": [\"3\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_2\", \"read\": [\"Y\"], \"to\": \"s_subtract_2\", \"write\": [\"Y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_2\", \"read\": [\"y\"], \"to\": \"s_subtract_3\", \"write\": [\"Y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_3\", \"read\": [\"y\"], \"to\": \"s_subtract_3\", \"write\": [\"y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_3\", \"read\": [\"2\"], \"to\": \"s_subtract_4\", \"write\": [\"y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_4\", \"read\": [\"U\"], \"to\": \"s_subtract_4\", \"write\": [\"2\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_4\", \"read\": [\"X\"], \"to\": \"s_subtract_5\", \"write\": [\"U\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_5\", \"read\": [\"X\"], \"to\": \"s_subtract_5\", \"write\": [\"X\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_5\", \"read\": [\"x\"], \"to\": \"s_subtract_6\", \"write\": [\"X\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_6\", \"read\": [\"x\"], \"to\": \"s_subtract_6\", \"write\": [\"x\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_6\", \"read\": [\"1\"], \"to\": \"s_subtract_7\", \"write\": [\"1\"], \"direction\": [\"left\"]}\n"
            "  ],\n"
            "\n"
            "  \"StartState\": \"s_subtract_1\",\n"
            "  \"Blank\": \"B\",\n"
            "  \"FinalStates\": [\"s_subtract_7\"]\n"
            "}" << std::endl;

    File.close();
}

void LevelUp::createTMSSY()
{
    std::ofstream File("Subroutine_subtract_y.json");

    File << "{\n"
            "  \"States\": [\"s_subtract_y_1\", \"s_subtract_y_2\", \"s_subtract_y_3\", \"s_subtract_y_4\"],\n"
            "  \"InputAlphabet\": [\"1\", \"x\", \"X\", \"U\", \"2\", \"y\", \"Y\", \"3\", \"-\"],\n"
            "  \"TapeAlphabet\": [\"1\", \"x\", \"X\", \"U\", \"2\", \"y\", \"Y\", \"3\", \"-\", \"B\"],\n"
            "\n"
            "  \"Transitions\": [\n"
            "    {\"from\": \"s_subtract_y_1\", \"read\": [\"-\"], \"to\": \"s_subtract_y_1\", \"write\": [\"B\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_y_1\", \"read\": [\"3\"], \"to\": \"s_subtract_y_1\", \"write\": [\"B\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_y_1\", \"read\": [\"Y\"], \"to\": \"s_subtract_y_2\", \"write\": [\"3\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_y_2\", \"read\": [\"Y\"], \"to\": \"s_subtract_y_2\", \"write\": [\"Y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_y_2\", \"read\": [\"y\"], \"to\": \"s_subtract_y_3\", \"write\": [\"Y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_y_3\", \"read\": [\"y\"], \"to\": \"s_subtract_y_3\", \"write\": [\"y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_y_3\", \"read\": [\"2\"], \"to\": \"s_subtract_y_3\", \"write\": [\"2\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_y_3\", \"read\": [\"U\"], \"to\": \"s_subtract_y_3\", \"write\": [\"U\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_y_3\", \"read\": [\"X\"], \"to\": \"s_subtract_y_3\", \"write\": [\"X\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_y_3\", \"read\": [\"x\"], \"to\": \"s_subtract_y_3\", \"write\": [\"x\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"s_subtract_y_3\", \"read\": [\"1\"], \"to\": \"s_subtract_y_4\", \"write\": [\"1\"], \"direction\": [\"left\"]}\n"
            "  ],\n"
            "\n"
            "  \"StartState\": \"s_subtract_y_1\",\n"
            "  \"Blank\": \"B\",\n"
            "  \"FinalStates\": [\"s_subtract_y_4\"]\n"
            "}" << std::endl;

    File.close();
}

void LevelUp::createTMCVM()
{
    std::ofstream File("TuringMachine_change_value_main.json");

    File << "{\n"
            "  \"States\": [\"q1\", \"q2\", \"q3\", \"q4\", \"q5\", \"q6\", \"q7\", \"q8\", \"q9\", \"q10\"],\n"
            "  \"InputAlphabet\": [\"1\", \"x\", \"X\", \"U\", \"2\", \"y\", \"Y\", \"3\", \"+\", \"-\", \"F\", \"S\"],\n"
            "  \"TapeAlphabet\": [\"1\", \"x\", \"X\", \"U\", \"2\", \"y\", \"Y\", \"3\", \"+\", \"-\", \"F\", \"S\", \"B\"],\n"
            "\n"
            "  \"Transitions\": [\n"
            "    {\"from\": \"q1\", \"read\": [\"1\"], \"to\": \"q1\", \"write\": [\"1\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"x\"], \"to\": \"q1\", \"write\": [\"x\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"X\"], \"to\": \"q1\", \"write\": [\"X\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"U\"], \"to\": \"q1\", \"write\": [\"U\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"2\"], \"to\": \"q1\", \"write\": [\"2\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"y\"], \"to\": \"q1\", \"write\": [\"y\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"Y\"], \"to\": \"q1\", \"write\": [\"Y\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"3\"], \"to\": \"q1\", \"write\": [\"3\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"+\"], \"to\": \"q6\", \"write\": [\"+\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q1\", \"read\": [\"-\"], \"to\": \"q10\", \"write\": [\"-\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q10\", \"read\": [\"F\"], \"to\": \"q10\", \"write\": [\"F\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"q10\", \"read\": [\"-\"], \"to\": \"q2\", \"write\": [\"-\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"x\"], \"to\": \"q2\", \"write\": [\"x\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"X\"], \"to\": \"q2\", \"write\": [\"X\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"U\"], \"to\": \"q2\", \"write\": [\"U\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"2\"], \"to\": \"q2\", \"write\": [\"2\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"y\"], \"to\": \"q2\", \"write\": [\"y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"Y\"], \"to\": \"q2\", \"write\": [\"Y\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"3\"], \"to\": \"q2\", \"write\": [\"3\"], \"direction\": [\"left\"]},\n"
            "    {\"from\": \"q2\", \"read\": [\"1\"], \"to\": \"q3\", \"write\": [\"1\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q3\", \"read\": [\"X\"], \"to\": \"q7\", \"write\": [\"X\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q3\", \"read\": [\"x\"], \"to\": \"q4\", \"write\": [\"x\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"x\"], \"to\": \"q4\", \"write\": [\"x\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"X\"], \"to\": \"q4\", \"write\": [\"X\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"U\"], \"to\": \"q4\", \"write\": [\"U\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"2\"], \"to\": \"q4\", \"write\": [\"2\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"Y\"], \"to\": \"q7\", \"write\": [\"Y\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q4\", \"read\": [\"y\"], \"to\": \"q5\", \"write\": [\"y\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q5\", \"read\": [\"y\"], \"to\": \"q5\", \"write\": [\"y\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q5\", \"read\": [\"Y\"], \"to\": \"q5\", \"write\": [\"Y\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q5\", \"read\": [\"3\"], \"to\": \"q5\", \"write\": [\"3\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q5\", \"read\": [\"-\"], \"to\": \"q9\", \"write\": [\"-\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"X\"], \"to\": \"q7\", \"write\": [\"X\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"U\"], \"to\": \"q7\", \"write\": [\"U\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"2\"], \"to\": \"q7\", \"write\": [\"2\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"y\"], \"to\": \"q7\", \"write\": [\"y\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"Y\"], \"to\": \"q7\", \"write\": [\"Y\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"3\"], \"to\": \"q7\", \"write\": [\"3\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"+\"], \"to\": \"q7\", \"write\": [\"B\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"-\"], \"to\": \"q7\", \"write\": [\"B\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"F\"], \"to\": \"q7\", \"write\": [\"B\"], \"direction\": [\"right\"]},\n"
            "    {\"from\": \"q7\", \"read\": [\"S\"], \"to\": \"q7\", \"write\": [\"B\"], \"direction\": [\"right\"]}\n"
            "  ],\n"
            "\n"
            "  \"StartState\": \"q1\",\n"
            "  \"Blank\": \"B\",\n"
            "  \"FinalStates\": [\"q8\"],\n"
            "\n"
            "  \"Subroutines\": [\n"
            "    {\"from\": \"q6\", \"readFrom\": [\"F\"], \"writeFrom\": [\"B\"], \"directionFrom\": [\"left\"], \"to\": \"q8\", \"readTo\": [\"B\"], \"writeTo\": [\"B\"], \"directionTo\": [\"right\"], \"subroutine_file\": \"./Subroutine_add_x.json\"},\n"
            "    {\"from\": \"q6\", \"readFrom\": [\"S\"], \"writeFrom\": [\"B\"], \"directionFrom\": [\"left\"], \"to\": \"q8\", \"readTo\": [\"B\"], \"writeTo\": [\"B\"], \"directionTo\": [\"right\"], \"subroutine_file\": \"./Subroutine_add_y.json\"},\n"
            "    {\"from\": \"q9\", \"readFrom\": [\"F\"], \"writeFrom\": [\"B\"], \"directionFrom\": [\"left\"], \"to\": \"q8\", \"readTo\": [\"B\"], \"writeTo\": [\"B\"], \"directionTo\": [\"right\"], \"subroutine_file\": \"./Subroutine_subtract_x.json\"},\n"
            "    {\"from\": \"q10\", \"readFrom\": [\"S\"], \"writeFrom\": [\"B\"], \"directionFrom\": [\"left\"], \"to\": \"q8\", \"readTo\": [\"B\"], \"writeTo\": [\"B\"], \"directionTo\": [\"right\"], \"subroutine_file\": \"./Subroutine_subtract_y.json\"}\n"
            "  ]\n"
            "}" << std::endl;

    File.close();
}

/**
 * Creates main Turing machine
 */
void LevelUp::createTMmain()
{
    std::ofstream File("TuringMachine_main.json");

    File << "{\n"
            "  \"States\": [\"q0\", \"q9\", \"q10\", \"q11\", \"q12\", \"q15\"],\n"
            "  \"InputAlphabet\": [\"1\", \"2\", \"3\", \"x\", \"X\", \"y\", \"Y\", \"U\",\"P\", \"F\"],\n"
            "  \"TapeAlphabet\": [\"1\", \"2\", \"3\", \"x\", \"X\", \"y\", \"Y\", \"U\", \"P\", \"F\", \"B\"],\n"
            "\n"
            "  \"Tapes\": 2,\n"
            "\n"
            "  \"Transitions\":[\n"
            "    {\"from\":\"q0\", \"read\": [\"B\", \"B\"], \"to\": \"q0\", \"write\": [\"B\", \"B\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\":\"q10\", \"read\": [\"3\", \"B\"], \"to\": \"q12\", \"write\": [\"3\", \"B\"], \"direction\": [\"stay\", \"stay\"]},\n"
            "    {\"from\":\"q10\", \"read\": [\"3\", \"1\"], \"to\": \"q11\", \"write\": [\"3\", \"1\"], \"direction\": [\"left\", \"stay\"]},\n"
            "    {\"from\":\"q11\", \"read\": [\"2\", \"1\"], \"to\": \"q11\", \"write\": [\"2\", \"1\"], \"direction\": [\"left\", \"stay\"]},\n"
            "    {\"from\":\"q11\", \"read\": [\"x\", \"1\"], \"to\": \"q11\", \"write\": [\"x\", \"1\"], \"direction\": [\"left\", \"stay\"]},\n"
            "    {\"from\":\"q11\", \"read\": [\"X\", \"1\"], \"to\": \"q11\", \"write\": [\"X\", \"1\"], \"direction\": [\"left\", \"stay\"]},\n"
            "    {\"from\":\"q11\", \"read\": [\"y\", \"1\"], \"to\": \"q11\", \"write\": [\"y\", \"1\"], \"direction\": [\"left\", \"stay\"]},\n"
            "    {\"from\":\"q11\", \"read\": [\"Y\", \"1\"], \"to\": \"q11\", \"write\": [\"Y\", \"1\"], \"direction\": [\"left\", \"stay\"]},\n"
            "    {\"from\":\"q11\", \"read\": [\"U\", \"1\"], \"to\": \"q11\", \"write\": [\"U\", \"1\"], \"direction\": [\"left\", \"stay\"]},\n"
            "    {\"from\":\"q11\", \"read\": [\"1\", \"1\"], \"to\": \"q0\", \"write\": [\"1\", \"1\"], \"direction\": [\"stay\", \"stay\"]},\n"
            "    {\"from\": \"q9\", \"read\": [\"F\", \"2\"], \"to\": \"q9\", \"write\": [\"U\", \"2\"], \"direction\": [\"right\", \"stay\"]},\n"
            "    {\"from\": \"q9\", \"read\": [\"P\", \"2\"], \"to\": \"q15\", \"write\": [\"U\", \"2\"], \"direction\": [\"right\", \"stay\"]},\n"
            "    {\"from\": \"q15\", \"read\": [\"2\", \"2\"], \"to\": \"q15\", \"write\": [\"2\", \"2\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q15\", \"read\": [\"y\", \"y\"], \"to\": \"q15\", \"write\": [\"y\", \"y\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q15\", \"read\": [\"Y\", \"y\"], \"to\": \"q15\", \"write\": [\"Y\", \"y\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q15\", \"read\": [\"y\", \"Y\"], \"to\": \"q15\", \"write\": [\"y\", \"Y\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q15\", \"read\": [\"Y\", \"Y\"], \"to\": \"q15\", \"write\": [\"Y\", \"Y\"], \"direction\": [\"right\", \"right\"]},\n"
            "    {\"from\": \"q15\", \"read\": [\"3\", \"y\"], \"to\": \"q15\", \"write\": [\"3\", \"y\"], \"direction\": [\"stay\", \"right\"]},\n"
            "    {\"from\": \"q15\", \"read\": [\"3\", \"Y\"], \"to\": \"q15\", \"write\": [\"3\", \"Y\"], \"direction\": [\"stay\", \"right\"]},\n"
            "    {\"from\": \"q15\", \"read\": [\"y\", \"3\"], \"to\": \"q15\", \"write\": [\"y\", \"3\"], \"direction\": [\"right\", \"stay\"]},\n"
            "    {\"from\": \"q15\", \"read\": [\"Y\", \"3\"], \"to\": \"q15\", \"write\": [\"Y\", \"3\"], \"direction\": [\"right\", \"stay\"]},\n"
            "    {\"from\": \"q15\", \"read\": [\"3\", \"3\"], \"to\": \"q10\", \"write\": [\"3\", \"3\"], \"direction\": [\"stay\", \"right\"]}\n"
            "  ],\n"
            "\n"
            "  \"StartState\": \"q0\",\n"
            "  \"Blank\": \"B\",\n"
            "  \"FinalStates\": [\"q12\"],\n"
            "\n"
            "  \"Subroutines\": [\n"
            "    {\"from\": \"q0\", \"readFrom\": [\"1\", \"1\"], \"writeFrom\": [\"1\", \"1\"], \"directionFrom\": [\"right\", \"right\"], \"to\": \"q9\", \"readTo\": [\"2\", \"2\"], \"writeTo\": [\"2\", \"2\"], \"directionTo\": [\"left\", \"stay\"], \"subroutine_file\": \"./Subroutine_check_x.json\"},\n"
            "    {\"from\": \"q9\", \"readFrom\": [\"2\", \"2\"], \"writeFrom\": [\"2\", \"2\"], \"directionFrom\": [\"right\", \"right\"], \"to\": \"q10\", \"readTo\": [\"3\", \"3\"], \"writeTo\": [\"3\", \"3\"], \"directionTo\": [\"stay\", \"right\"], \"subroutine_file\": \"./Subroutine_check_y.json\"}\n"
            "  ]\n"
            "}" << std::endl;

    File.close();
}
