#include <memory>

#include <LLParser.h>
#include <cmath>
#include <thread>

/**
 * Constructor for the LL parser where the cfg is given.
 * @param cfg The cfg that is used for the parse table of the LL parser.
 */
LLParser::LLParser(const CFG& cfg, const int lookahead)
        :m_CFG(cfg), m_lookahead(lookahead) { this->makeParseTable(); }

/**
* Helper function that makes the parse table with the current cfg for a LL(k) parser.
*
* This function uses multi-threading so the speed may differ depending on the amount of threads your processor has.
*/
void LLParser::makeParseTable()
{
    std::vector<thread*> threads;
    auto lamba = [&](auto i) {
        setMatchingProductions(i, m_lookahead);
    };
    //loop over all variables and then convert all terminal to get all possible cases
    for (auto i:m_CFG.getVariables()) {
        std::vector<CFGProduction> productions = m_CFG.getProductionWithVariable(i);
        if (productions.empty() && m_CFG.getFromReferenceTable(i).empty()) {
            //if there are no productions for a variable then set all terminals combos to error
            for (const auto& j:m_CFG.getTerminals()) {
                if (m_ParseTable[i][{j}].empty()) {
                    m_ParseTable[i][{j}] = {"error"};
                }
            }
        }
        else {
            //set the calculation of every symbol on a different thread so it can run in parallel
            threads.push_back(new thread(lamba, i));
        }
    }
    //set the eos for all the variables
    for (const auto& i:m_CFG.getVariables()) {
        if (m_CFG.varHasEpsilonProduction(i)) {
            m_ParseTable[i][{"EOS"}] = {};
        }
        else {
            m_ParseTable[i][{"EOS"}] = {"error"};
        }
    }
    //join all the threads
    for (const auto& i:threads) {
        i->join();
    }
    //destroy all threads
    for (const auto& i:threads) {
        delete i;
    }
}

/**
 * This function takes a variable from the stack and the
 * terminal you read of the input and will give you the production you should use.
 * @param terminal The first next terminal from the input.
 * @param variable Variable you are trying to turn into the terminal.
 * @return String containing the production you should use.
 */
std::vector<std::string> LLParser::getMatchingProduction(const std::vector<std::string>& terminal,
        std::string& variable) const
{
    std::vector<CFGProduction> productions = m_CFG.getProductionWithVariable(variable);
    bool hasEmptyProduction = false;
    //see if the string end with eos
    bool EOS = (!terminal.empty()) ? terminal[terminal.size()-1]=="EOS" : false;

    //if it ends with eos and the size is 2 then we need to check if the first var is a refernce var and then calculates the replacement if so
    if (EOS && terminal.size()==2) {
        if (!m_CFG.getFromReferenceTable(variable).empty()) {
            for (const auto& i:m_CFG.getFromReferenceTable(variable)) {
                if (i==terminal[0]) {
                    return {i};
                }
            }
        }
    }
    //lastresort is for when no direct replcament is possible for all the variables it will then set it to a possible chenge for part of the variables
    std::vector<std::string> lastresort = {};
    //the actuall big algorithm is here
    for (const auto& i:productions) {
        if (i.getBody().empty()) {
            hasEmptyProduction = true;
        }
        //if it is asking to go to epsilon there must be an epsilon production else it is error
        if (i.getBody().empty() && terminal.empty()) {
            return {};
        }
        else {
            if (!i.getBody().empty()) {
                if (i.getBody().size()==1 && i.getBody()[0].empty()) {
                    if (terminal.empty())return i.getBody();
                    if (terminal.size()==1 && terminal[0]=="EOS")return i.getBody();
                }
                else {
                    if (prodGoesToTerminals(i.getBody(), terminal)) {
                        return i.getBody();
                    }
                    else if (prodGoesToTerminals(i.getBody(), {terminal[0]})) {
                        lastresort = i.getBody();
                    }
                }
            }
        }
    }
    if (!lastresort.empty()) {
        return lastresort;
    }
    if (terminal.size()==2 && !m_CFG.getFromReferenceTable(variable).empty()) {
        for (const auto& i:m_CFG.getFromReferenceTable(variable)) {
            if (i==terminal[0]) {
                return {i};
            }
        }
    }
    //if there is an empty production then it will return an empty vector else it will return error
    if (hasEmptyProduction) {
        return {};
    }
    return {"error"};
}

/**
 * Function that checks if the first character of a given string is a variable.
 * @param toCheck The string you are checking on.
 * @return Bool representing whether it is a variable or not.
 */
std::string LLParser::isFirstVariable(const std::string toCheck) const
{
    std::string toreturn;
    for (const auto& i:m_CFG.getVariables()) {
        if (i.size()<=toCheck.size()) {
            if (toCheck.compare(0, i.size(), i)==0) {
                toreturn = i;
            }
        }
    }
    return toreturn;
}

/**
 * Function that checks if the first symbol from a given string is a terminal.
 * @param toCheck The string you are checking on.
 * @return Bool representing whether it is a terminal or not.
 */
std::string LLParser::isFirstTerminal(const std::string toCheck) const
{
    std::string toreturn;
    for (const auto& i:m_CFG.getTerminals()) {
        if (i.size()<=toCheck.size()) {
            if (!toCheck.compare(0, i.size(), i)) {
                toreturn = i;
            }
        }
    }
    return toreturn;
}

/**
 * Function that checks all productions of a variable recursively and checks if
 * it ever goes to a certain terminal.
 * @param var Variable you are doing the check for.
 * @param terminal The terminal you are trying to turn the variable in.
 * @param alreadyChecked Helper vector to make sure you don't get stuck in an infinite loop.
 * @return Bool representing whether it goes to the terminal or not.
 */
bool LLParser::prodGoesToTerminals(std::vector<std::string> prod, std::vector<std::string> terminal) const
{
    //if the terminal vector is empty that means all terminals have been matched thus we must return true
    //but it can also give true if the only terminal given is "end of string" but only if the production is also empty
    if (!terminal.empty()) {
        bool haseos = terminal[terminal.size()-1]=="EOS";
        if (terminal[0].empty() && prod.empty() && haseos) return true;
        if (prod.empty()) return false;

        if (m_CFG.isTerminal(prod[0])) {
            if (prod[0]==terminal[0]) {
                return prodGoesToTerminals({prod.begin()+1, prod.end()}, {terminal.begin()+1, terminal.end()});
            }
            else {
                return false;
            }
        }
        else if (m_CFG.isVariable(prod[0])) {
            for (const auto& i:m_CFG.getProductionWithVariable(prod[0])) {
                std::vector<std::string> temp = prod;
                if (!temp.empty()) {
                    temp.erase(temp.begin());
                    temp.insert(temp.begin(), i.getBody().begin(), i.getBody().end());
                }
                else {
                    temp.erase(temp.begin());
                }
                //recursive call
                if (prodGoesToTerminals(temp, terminal)) return true;

            }
            if (!m_CFG.getFromReferenceTable(prod[0]).empty()) {
                for (const auto& i:m_CFG.getFromReferenceTable(prod[0])) {
                    if (i==terminal[0]) {
                        std::vector<std::string> newprod = prod;
                        newprod.erase(newprod.begin());
                        std::vector<std::string> newterminal = terminal;
                        newterminal.erase(newterminal.begin());
                        return prodGoesToTerminals(newprod, newterminal);
                    }
                }
            }
            return false;
        }
        else {
            throw LLExcept("Symbol: "+prod[0]+" that is in a production not possible in this cfg");
        }
    }
    else {
        return true;
    }
}

/**
 * Function that parses a file with the LL process and returns a vector of actions another function must do.
 * @param input The string you are checking.
 * @return Vector of actions that can be easily parsed to the actual game.
 */
std::vector<std::string> LLParser::parse(std::string input) const
{
    //parse process it trought the typical ll parse method
    //it constantly puts the next 2 veriables from the input trough the parse tabkle with the current variable at the top of stack
    //if the top of stack is not a variable then it must be a terminal, it will pop that one and check of the input currently starts with the popped terminal
    //if so also remove it from the input, if not input is not accepted
    std::vector<std::string> stack = {m_CFG.getStart()};
    //the input gets splits in vector of terminal/variables so it's easier to work with
    std::vector<std::string> in = splitString(std::move(input));
    std::vector<std::string> actions;
    string error_msg;
    while (true) {
        if (!stack.empty()) {
            //if terminal pop it and check if it was the same as back of input and pop that
            if (m_CFG.isTerminal(stack.back())) {
                if (!in.empty() && in[0]==stack.back()) {
                    if (error_msg.size()<20)
                        error_msg.append(in[0]);
                    else {
                        error_msg.erase(error_msg.begin());
                        error_msg.append(in[0]);
                    }
                    in.erase(in.begin());
                    stack.pop_back();
                }
                else {
                    throw LLExcept("Given file is not accepted fault happened at : "+error_msg+" ->");
                }
            }
                //if it is a variable exchange it trough parsetable
            else if (m_CFG.isVariable(stack.back())) {
                if (!in.empty()) {
                    if (!isFirstTerminal(in[0]).empty()) {
                        std::vector<std::string> toAdd;
                        std::vector<std::string> lookupVec;
                        if (in.size()>=m_lookahead) {
                            lookupVec = {in.begin(), in.begin()+m_lookahead};
                        }
                        else {
                            lookupVec = in;
                            lookupVec.emplace_back("EOS");
                        }
                        toAdd = m_ParseTable[stack.back()][lookupVec];
                        std::string action = m_CFG.getActionFromProduction(stack.back(), toAdd);
                        //the parseSpecial action is an exception to the rule, when a parsespecial happens all the input gets parsed
                        // until there is a terminal that is not part of the referencevar stated in the production
                        //the output from this parsespecial is all the characters that were parser
                        //this is used to get the names of gameobjects and numbers
                        if (action=="parseSpecial") {
                            actions.emplace_back(parseReferenceVar(in, stack.back()));
                            stack.pop_back();
                            //if not a parsespecial then just the action to the actions vector
                        }
                        else {
                            if (!action.empty()) actions.emplace_back(action);
                            stack.pop_back();
                            for (int i = static_cast<int>(toAdd.size()-1); i>=0; i--) {
                                stack.emplace_back(toAdd[i]);
                            }
                        }
                    }
                    else {
                        throw (LLExcept("Input is not recognised as a terminal"));
                    }
                }
                else {
                    std::vector<std::string> toAdd = m_ParseTable[stack.back()][{"EOS"}];
                    stack.pop_back();
                    for (unsigned long i = toAdd.size()-1; i>=0; i--) {
                        stack.emplace_back(toAdd[i]);
                    }

                }
            }
            else if (stack.back()=="error") {
                throw LLExcept("Given file is not accepted fault happened at "+error_msg+" ->");
            }
            else {
                throw LLExcept("Symbol at the back of the stack: "+stack.back()+" is not terminal or variable");
            }
        }
        else {
            if (in.empty() && stack.empty()) {
                return actions;
            }
            else {
                throw LLExcept("Input not accepted: stack or input were not empty at the end of parse process");
            }
        }
    }
}

/**
 * Helper function that parses the special variables (variable and number) at runtime
 * so it's possible to give a string as input.
 * @param in Current state of the input, after the function all the chars of the special string will be popped.
 * @param Var Variable that is the special character.
 * @return String that represents the character that were part of the special string.
 */
std::string LLParser::parseReferenceVar(std::vector<std::string>& in, const std::string& Var) const
{
    std::vector<std::string> stack = {Var};
    std::string specialParse;
    std::string error_msg;
    while (true) {
        if (!stack.empty()) {
            if (m_CFG.isTerminal(stack.back())) {
                if (!in.empty() && in[0]==stack.back()) {
                    if (error_msg.size()<10)
                        error_msg.append(in[0]);
                    else {
                        error_msg.erase(error_msg.begin());
                        error_msg.append(in[0]);
                    }
                    in.erase(in.begin());
                    specialParse += stack.back();
                    stack.pop_back();
                }
                else {
                    throw LLExcept("Given file is not accepted fault happened at "+error_msg+" ->");
                }
            }
            else if (m_CFG.isVariable(stack.back())) {
                if (!in.empty()) {
                    if (!isFirstTerminal(in[0]).empty()) {
                        std::vector<std::string> toAdd;
                        std::vector<std::string> lookupVec;
                        if (in.size()>=m_lookahead) {
                            lookupVec = {in.begin(), in.begin()+m_lookahead};
                        }
                        else {
                            lookupVec = in;
                            lookupVec.emplace_back("EOS");
                        }
                        toAdd = m_ParseTable[stack.back()][lookupVec];
                        stack.pop_back();
                        for (int i = static_cast<int>(toAdd.size()-1); i>=0; i--) {
                            stack.emplace_back(toAdd[i]);
                        }

                    }
                    else {
                        throw (LLExcept("Input is not recognised as a terminal"));
                    }
                }
                else {
                    std::vector<std::string> toAdd = m_ParseTable[stack.back()][{"EOS"}];
                    stack.pop_back();
                    for (int i = static_cast<int>(toAdd.size()-1); i>=0; i--) {
                        stack.emplace_back(toAdd[i]);
                    }

                }
            }
            else if (stack.back()=="error") {
                throw LLExcept("Given file is not accepted fault happened at character: "+error_msg+" ->");
            }
            else {
                throw LLExcept("Symbol at the back of the stack: "+stack.back()+" is not terminal or variable");
            }
        }
        else {
            if (stack.empty()) {
                return specialParse;
            }
            else {
                throw LLExcept("Input not accepted: stack was not empty at the end of a special parse process");
            }
        }
    }

}

/**
 * Turns the current parse table into html.
 * @param onStream The stream you write to (should be a html file).
 */
void LLParser::parseTableToHTML(std::ostream& onStream) const
{
    onStream << "<style>"
             << "body {"
             << "width: 960px;"
             << "margin: auto;"
             << "}"
             << "#content {"
             << "line-height: 1.2;"
             << "width: 100%;"
             << "border: solid 1px #000000;"
             << "font-family: \"Sans Serif\", Arial;"
             << "padding: 10px;"
             << "}"
             << "</style>" << std::endl;
    onStream << "<div align=\"center\">\n\t<h3>Parse table</h3>\n</div>" << std::endl;
    onStream << "<table style=\"width:100%\" border=\"1px solid black\">\n\t<tr>" << std::endl;
    onStream << "\t\t<th>/</th>" << std::endl;
    HTMlAddTerminals(onStream, {}, m_lookahead);
    onStream << "\t\t<th>EOS</th>\n\t</tr>" << std::endl;
    for (const auto& i:m_CFG.getVariables()) {
        onStream << "\t<tr>\n\t\t<th>" << i << "</th>" << std::endl;
        HTMLAddSymbolTerminals(onStream, {}, m_lookahead, i);
        onStream << "\t<tr>" << std::endl;
    }
    onStream << "</table>";
}

/**
 * Helper (recursive) function that adds the row of terminals at the top of the file.
 * @param onStream The file you are writing to.
 * @param current Helping variable that is passed to the next call of the function.
 * @param lookahead Depth of the recursion.
 */
void LLParser::HTMlAddTerminals(std::ostream& onStream, std::vector<std::string> current, int lookahead) const
{
    if (lookahead>0) {
        for (const auto& i:m_CFG.getTerminals()) {
            std::vector<std::string> newvec = current;
            newvec.emplace_back(i);
            HTMlAddTerminals(onStream, newvec, lookahead-1);
        }
    }
    else {
        onStream << "\t\t<th>" << concatVec(current) << "</th>" << std::endl;
    }
    if (lookahead>0 && lookahead!=m_lookahead) {
        std::vector<std::string> newvec = current;
        newvec.emplace_back("EOS");
        onStream << "\t\t<th>" << concatVec(newvec) << "</th>" << std::endl;
    }
}

/**
 * Helper function that adds the production of 1 symbol to the file.
 * @param onStream The html file you are writing to.
 * @param current The string that is recursively given.
 * @param lookahead Remaining depth of the recursion.
 * @param symbol Symbol you are getting the productions for.
 */
void LLParser::HTMLAddSymbolTerminals(std::ostream& onStream, std::vector<std::string> current, int lookahead,
        std::string symbol) const
{
    if (lookahead>0) {
        for (const auto& i:m_CFG.getTerminals()) {
            std::vector<std::string> newvec = current;
            newvec.emplace_back(i);
            HTMLAddSymbolTerminals(onStream, newvec, lookahead-1, symbol);
        }
    }
    else {
        onStream << "\t\t<th>" << concatVec(m_ParseTable[symbol][current]) << "</th>" << std::endl;
    }
    if (lookahead>0) {
        onStream << "\t\t<th>" << concatVec(m_ParseTable[symbol][current]) << "</th>" << std::endl;
    }
}

/**
 * Function that splits a string into a vector of terminals.
 * @param toSplit String to split.
 * @return Vector of std::strings where each string is a single terminal.
 */
std::vector<std::string> LLParser::splitString(std::string toSplit) const
{
    std::vector<std::string> toreturn;
    while (!toSplit.empty()) {
        if (!isFirstTerminal(toSplit).empty()) {
            std::string temp = isFirstTerminal(toSplit);
            toreturn.emplace_back(temp);
            toSplit = toSplit.substr(temp.size(), toSplit.size()-temp.size());
        }
        else if (!isFirstVariable(toSplit).empty()) {
            std::string temp = isFirstVariable(toSplit);
            toreturn.emplace_back(temp);
            toSplit = toSplit.substr(temp.size(), toSplit.size()-temp.size());
        }
        else {
            std::string error_msg = "Given string contains symbols that are not a terminal: => ";
            error_msg += toSplit.front();
            error_msg += " <= ";
            error_msg += toSplit.substr(1, 40);
            throw LLExcept(error_msg);
        }
    }
    return toreturn;
}

/**
 * Function that takes a path of a file then streams the whole file to the parse function as a string
 * and returns the parsed file as a game.
 * @param path The path of the file.
 * @return Shared pointer to a fully parsed game.
 * @throws LLExcept when a fault happens
 */
std::shared_ptr<Game> LLParser::parseFile(const std::string& path) const
{
    std::ifstream myfile;
    myfile.open(path);
    std::string file((std::istreambuf_iterator<char>(myfile)),
            std::istreambuf_iterator<char>());
    file.erase(std::remove(file.begin(), file.end(), '\n'), file.end());
    file.erase(std::remove(file.begin(), file.end(), ' '), file.end());
    file.erase(std::remove(file.begin(), file.end(), '\v'), file.end());
    file.erase(std::remove(file.begin(), file.end(), '\t'), file.end());
    return parseLLoutput(parse(file));
}

/**
 * This is a recursive function that makes sure all possible terminal combinations are checked for the lookahead.
 * @param variable The variable you are checking to.
 * This function changes the parse table so that a var has a complete roster of possible productions.
 */
void LLParser::setMatchingProductions(std::string variable, int lookahead, std::vector<std::string> current)
{
    if (lookahead>0) {
        for (auto i:m_CFG.getTerminals()) {
            std::vector<std::string> newvec = current;
            newvec.emplace_back(i);
            setMatchingProductions(variable, lookahead-1, newvec);
        }
    }
    else {
        m_ParseTable[variable][current] = getMatchingProduction(current, variable);
    }
    if (lookahead>0) {
        std::vector<std::string> newvec = current;
        newvec.emplace_back("EOS");
        m_ParseTable[variable][newvec] = getMatchingProduction(newvec, variable);
    }
}

/**
 * Helper function that concatenates a vector of strings to one string.
 * @param vec The vector of strings.
 * @return String containing all the strings of the vec
 */
std::string LLParser::concatVec(const std::vector<std::string>& vec) const
{
    std::string toreturn;
    for (const auto& i:vec) {
        if (!i.empty()) {
            toreturn.append(i);
        }
        else {
            toreturn.append("&#949");
        };
    }
    return toreturn;
}

/**
 * Function that takes the partially parsed vector of strings from the parse function and turns
 * it into a shared ptr of a game.
 * @param output The vector of strings that the parse function produces.
 * @return Shared_ptr of game that contains everything that was in the file.
 */
std::shared_ptr<Game> LLParser::parseLLoutput(std::vector<std::string> output) const
{
    std::shared_ptr<Game> toreturn;
    if (!output.empty() && output[0]=="makeGame") {
        output.erase(output.begin());
        toreturn = parseGame(output);
    }
    else {
        if (output.empty()) {
            throw LLExcept("Parsed file does not contain recipe to create game");
        }
        else {
            throw LLExcept("First instruction of the parsed file is not to create a game but is "+output[0]);
        }
    }
    return toreturn;
}

/**
 * Function that actually start the parsing process of the game.
 * @param output Vector of strings were the first index contains the name of the game.
 * @return Shared pointer to a fully parsed game.
 */
std::shared_ptr<Game> LLParser::parseGame(std::vector<std::string>& output) const
{
    if (!output.empty()) {
        Game* g = new Game(output[1]);
        std::shared_ptr<Game> newGame(g);
        newGame.get()->setName(output[0]);
        output.erase(output.begin());
        while (true) {
            if (!output.empty()) {
                if (output.size()>1 && output[0]=="makeLevel") {
                    output.erase(output.begin());
                    newGame.get()->addLevel(parseLevel(output));
                }
                else if (output[0]=="end") {
                    cout << newGame->getHeight() << endl;
                    return newGame;
                }
                else {
                    throw LLExcept("While making the game there was an unknown instruction called "+output[0]);
                }
            }
            else {
                throw LLExcept("Output was empty when it shouldnt be");
            }
        }
    }
    else {
        throw LLExcept("There was no name found for the game");
    }
}

/**
 * Function that parses a level and all its variables.
 * @param output Vector of string were the first index is the name of the level.
 * @return Shared pointer to a fully parsed level.
 */
std::shared_ptr<Level> LLParser::parseLevel(std::vector<std::string>& output) const
{
    std::shared_ptr<Level> newLevel;
    if (!output.empty()) {
        newLevel = std::make_shared<Level>(output[0]);
        output.erase(output.begin());
        while (!output.empty()) {
            if (output[0]=="setWidth") {
                output.erase(output.begin());
                if (!output.empty()) {
                    newLevel.get()->setWidth(static_cast<int>(specialStringToDouble(output[0])));
                    output.erase(output.begin());
                }
                else {
                    throw LLExcept("While trying to set the width of the level no width was found!");
                }
            }
            else if (output[0]=="setHeight") {
                output.erase(output.begin());
                if (!output.empty()) {
                    newLevel.get()->setHeight(static_cast<int>(specialStringToDouble(output[0])));
                    output.erase(output.begin());
                }
                else {
                    throw LLExcept("While trying to set the height of the level no width was found");
                }
            }
            else if (output[0]=="setColor") {
                output.erase(output.begin());
                if (output.size()>2) {
                    std::shared_ptr<Color> color = std::make_shared<Color>(
                            static_cast<int>(specialStringToDouble(output[0])),
                            static_cast<int>(specialStringToDouble(output[1])),
                            static_cast<int>(specialStringToDouble(output[2])));
                    newLevel.get()->setColor(color);
                }
                else {
                    throw LLExcept("While trying to set the color of the level there were no parameters found");
                }
                output.erase(output.begin(), output.begin()+3);
            }
            else if (output[0]=="makeGameObject") {
                output.erase(output.begin());
                if (output.size()>1) {
                    std::string name = output[0];
                    if (output[1]=="makeEntity") {
                        output.erase(output.begin()+1);
                        newLevel.get()->addEntity(parseEntity(output));
                    }
                    else if (output[1]=="makeObject") {
                        output.erase(output.begin()+1);
                        newLevel.get()->addObject(parseObject(output));
                    }
                    else if (output[1]=="makeEnd") {
                        output.erase(output.begin());
                        newLevel.get()->setEndObject(parseEnd(output));
                    }
                    else {
                        throw LLExcept("while trying to make a game object, an  invalid game object was given");
                    }

                }
                else {
                    throw LLExcept(
                            "While trying to make a new object in level there was no name given for this object");
                }
            }
            else if (output[0]=="end") {
                output.erase(output.begin());
                return newLevel;
            }
            else {
                if (!output.empty()) {
                    throw LLExcept("While creating a level the parser could not recognise input "+output[0]);
                }
            }
        }
    }
    else {
        throw LLExcept("Parsed file does no contain a name for the level");
    }
    return newLevel;

}

/**
 * Function that parses an end object for a level and all its variables
 * @param output a vector of string were the first index is the name of the end object
 * @return a shared pointer to a fully parsed en object
 */
std::shared_ptr<EndObject> LLParser::parseEnd(std::vector<std::string>& output) const
{
    std::shared_ptr<EndObject> newEnd(new EndObject());
    newEnd.get()->setName(output[0]);
    output.erase(output.begin());
    while (!output.empty()) {
        if (output[0]=="setWidth") {
            output.erase(output.begin());
            if (!output.empty()) {
                newEnd.get()->setWidth(static_cast<int>(specialStringToDouble(output[0])));
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While trying to set the width of an entity no width was found");
            }
        }
        else if (output[0]=="setHeight") {
            output.erase(output.begin());
            if (!output.empty()) {
                newEnd.get()->setHeight(static_cast<int>(specialStringToDouble(output[0])));
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While trying to set the height of an entity no height was found");
            }
        }
        else if (output[0]=="setColor") {
            output.erase(output.begin());
            if (output.size()>2) {
                std::shared_ptr<Color> color = std::make_shared<Color>(
                        static_cast<int>(specialStringToDouble(output[0])),
                        static_cast<int>(specialStringToDouble(output[1])),
                        static_cast<int>(specialStringToDouble(output[2])));
                newEnd.get()->setColor(color);
            }
            else {
                throw LLExcept("While trying to set the color of an entity there were no parameters found");
            }
            output.erase(output.begin(), output.begin()+3);
        }
        else if (output[0]=="setLocation") {
            output.erase(output.begin());
            if (output.size()>1) {
                newEnd.get()->setLocation(
                        make_pair(specialStringToDouble(output[0]), specialStringToDouble(output[1])));
                output.erase(output.begin());
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While setting the location for an end object not enough variables were found");
            }
        } else if (output[0]=="setTexture") {
            output.erase(output.begin());
            if (!output.empty()) {
                newEnd.get()->setTexturePath("./res/sprites/"+output[0]+".png");
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While setting the texture path of an end object there was no path found");
            }
        }
        else if (output[0]=="setCollision") {
            output.erase(output.begin());
            if (!output.empty()) {
                newEnd.get()->setCollision(output[0]=="true");
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While setting the collision of and end object there was no bool found");
            }
        }
        else if (output[0]=="makeObject") {
            output.erase(output.begin());
            newEnd.get()->addSubObject(parseObject(output));
        }
        else if (output[0]=="setGoto") {
            output.erase(output.begin());
            newEnd.get()->setGoto(output[0]);
            output.erase(output.begin());
        }
        else if (output[0]=="end") {
            output.erase(output.begin());
            return newEnd;
        }
        else {
            if (!output.empty()) {
                throw LLExcept("While creating an object the parser could not recognise input "+output[0]);
            }
        }
    }
    return newEnd;
}

/**
 * Function that parses an entity and all its variables.
 * @param output Vector of string were the first index is the name of the entity.
 * @return Shared pointer to a fully parsed entity
 */
std::shared_ptr<Entity> LLParser::parseEntity(std::vector<std::string>& output) const
{
    std::shared_ptr<Entity> newEntity(new Entity());
    newEntity.get()->setName(output[0]);
    output.erase(output.begin());
    while (!output.empty()) {
        if (output[0]=="setWidth") {
            output.erase(output.begin());
            if (!output.empty()) {
                newEntity.get()->setWidth(static_cast<int>(specialStringToDouble(output[0])));
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While trying to set the width of an entity no width was found");
            }
        }
        else if (output[0]=="setHeight") {
            output.erase(output.begin());
            if (!output.empty()) {
                newEntity.get()->setHeight(static_cast<int>(specialStringToDouble(output[0])));
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While trying to set the height of an entity no height was found");
            }
        }
        else if (output[0]=="setColor") {
            output.erase(output.begin());
            if (output.size()>2) {
                std::shared_ptr<Color> color = std::make_shared<Color>(
                        static_cast<int>(specialStringToDouble(output[0])),
                        static_cast<int>(specialStringToDouble(output[1])),
                        static_cast<int>(specialStringToDouble(output[2])));
                newEntity.get()->setColor(color);
            }
            else {
                throw LLExcept("While trying to set the color of an entity there were no parameters found");
            }
            output.erase(output.begin(), output.begin()+3);
        }
        else if (output[0]=="setLocation") {
            output.erase(output.begin());
            if (output.size()>1) {
                newEntity.get()->setLocation(
                        make_pair(specialStringToDouble(output[0]), specialStringToDouble(output[1])));
                output.erase(output.begin());
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While setting the location of an entity not enough variables were found");
            }
        }
        else if (output[0]=="setPathing") {
            output.erase(output.begin());
            if (!output.empty()) {
                newEntity->setPlayerBool(output[0]=="playercontrolled");
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While setting the pathing of an entity there was no pathing found");
            }
        }
        else if (output[0]=="setTexture") {
            output.erase(output.begin());
            if (!output.empty()) {
                newEntity.get()->setTexturePath("./res/sprites/"+output[0]+".png");
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While setting the texture path of an entity there was no path found");
            }
        }
        else if (output[0]=="setMovement") {
            output.erase(output.begin());
            if (!output.empty()) {
                newEntity.get()->setMovementSpeed(specialStringToDouble(output[0]));
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While setting the movementspeed of an entity no number was given");
            }
        }
        else if (output[0]=="setJump") {
            output.erase(output.begin());
            if (!output.empty()) {
                newEntity.get()->setJumpHeight(specialStringToDouble(output[0]));
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While parsing entity no number was given to set the jumpheight to");
            }
        }
        else if (output[0]=="end") {
            output.erase(output.begin());
            return newEntity;
        }
        else {
            if (!output.empty()) {
                throw LLExcept("While creating an entity the parser could not recognise input "+output[0]);
            }
        }
    }
    return std::shared_ptr<Entity>();
}

/**
 * Function that parses an object and all its variables recursively.
 * @param output Vector of string were the first index is the name of the object.
 * @return Shared pointer to a fully parsed object.
 */
std::shared_ptr<Object> LLParser::parseObject(std::vector<std::string>& output, double parentX, double parentY) const
{
    std::shared_ptr<Object> newObject(new Object());
    newObject.get()->setName(output[0]);
    output.erase(output.begin());
    while (!output.empty()) {
        if (output[0]=="setWidth") {
            output.erase(output.begin());
            if (!output.empty()) {
                newObject.get()->setWidth(static_cast<int>(specialStringToDouble(output[0])));
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While trying to set the width of an entity no width was found");
            }
        }
        else if (output[0]=="setHeight") {
            output.erase(output.begin());
            if (!output.empty()) {
                newObject.get()->setHeight(static_cast<int>(specialStringToDouble(output[0])));
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While trying to set the height of an entity no height was found");
            }
        }
        else if (output[0]=="setColor") {
            output.erase(output.begin());
            if (output.size()>2) {
                std::shared_ptr<Color> color = std::make_shared<Color>(
                        static_cast<int>(specialStringToDouble(output[0])),
                        static_cast<int>(specialStringToDouble(output[1])),
                        static_cast<int>(specialStringToDouble(output[2])));
                newObject.get()->setColor(color);
            }
            else {
                throw LLExcept("While trying to set the color of an entity there were no parameters found");
            }
            output.erase(output.begin(), output.begin()+3);
        }
        else if (output[0]=="setTexture") {
            output.erase(output.begin());
            if (!output.empty()) {
                newObject.get()->setTexturePath("./res/sprites/"+output[0]+".png");
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While setting the texture path of an object there was no path found");
            }
        }
        else if (output[0]=="setLocation") {
            if (newObject->getSubObjects().size()==0) {
                output.erase(output.begin());
                if (output.size()>1) {
                    newObject.get()->setLocation(
                            make_pair(specialStringToDouble(output[0])+parentX,
                                    specialStringToDouble(output[1])+parentY));
                    output.erase(output.begin());
                    output.erase(output.begin());
                }
                else {
                    throw LLExcept("While parsing an object no locations were given");
                }
            }
            else {
                throw LLExcept("Cannot set location of an object after sub-objects have been made");
            }
        }
        else if (output[0]=="setCollision") {
            output.erase(output.begin());
            if (!output.empty()) {
                newObject.get()->setCollision(output[0]=="true");
                output.erase(output.begin());
            }
            else {
                throw LLExcept("While trying to set the collsion of an object no bool was given");
            }
        }
        else if (output[0]=="makeObject") {
            output.erase(output.begin());
            newObject.get()->addSubObject(
                    parseObject(output, newObject.get()->getLocation().first, newObject.get()->getLocation().second));
        }
        else if (output[0]=="end") {
            output.erase(output.begin());
            return newObject;
        }
        else {
            if (!output.empty()) {
                throw LLExcept("While creating an object the parser could not recognise input "+output[0]);
            }
        }
    }
    return std::shared_ptr<Object>();
}

/**
 * Function that turns a string to a double where there can be a single "." to denote decimal numbers.
 * @param str The string that is turned into a double.
 * @return Double
 */
double LLParser::specialStringToDouble(std::string& str) const
{
    bool pointFound = false;
    double toreturn = 0;
    std::vector<int> greaterThanOne;
    std::vector<int> smallerThanOne;
    for (auto i:str) {
        if (i!='.') {
            if (i-'0'>=0 && i-'0'<10) {
                if (!pointFound) {
                    greaterThanOne.push_back(i-'0');
                }
                else {
                    smallerThanOne.push_back(i-'0');
                }
            }
            else {
                throw LLExcept("Something else than a number found when parsing a number");
            }
        }
        else {
            pointFound = true;
        }
    }
    for (int i = 0; i<greaterThanOne.size(); i++) {
        toreturn += greaterThanOne[i]*pow(10, greaterThanOne.size()-i-1);
    }
    for (unsigned int i = 0; i<smallerThanOne.size(); i++) {
        toreturn += smallerThanOne[i]/pow(10, i+1);
    }
    return toreturn;
}
