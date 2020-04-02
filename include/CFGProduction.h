#ifndef LEVELUP_CFGPRODUCTION_H
#define LEVELUP_CFGPRODUCTION_H

#include <vector>
#include <iostream>

class CFGProduction {
public:
    /**
     * Constructor by value
     * @param head the head of the production (where the production comes from)
     * @param body the body of the production (all the symbols teh production goes to)
     * @param action the action that this production should take. This is only used in the parsing process
     */
    CFGProduction(const std::string& head, const std::vector<std::string>& body,const std::string& action="");

    /**
     * Default constructor for CFG Production
     */
    CFGProduction()= default;

    /**
     * returns head of the production
     *@return head
     */
    const std::string& getHead() const;

    /**
     * Sets the head of a production
     * @param string String that is supposed to become the head
     */
    void setHead(const std::string& head);

    /**
     * returns the body of a CFG production
     * @return the vector of the body
     */
    const std::vector<std::string>& getBody() const;

    /**
     * sets the body
     * @param head The vector of strings that is supposed to become the body
     */
    void setBody(const std::vector<std::string>& head);

    /**
     * Adds a string to body
     * @param toAdd The string that needs to be added
     */
    void addTobody(const std::string& toAdd);

    /**
     * Deletes a string from the body
     * @param toDelete The string which needs to be deleted.
     */
    void deleteFromBody(const std::string& toDelete);

    /**
     * Deletes a string from the body with given integer
     * @param toDelete   integer wich is supposed to be the index of the item
     */
    void deleteFromBody(int toDelete);

    /**
     * Default destructor
     */
    virtual ~CFGProduction()= default;

    /**
     * Checks if 2 CFG productions have the same items
     * @param rhs a CFG production
     * @return  true if they are the same, else: false
     */
    bool operator==(const CFGProduction& rhs) const;

    /**
     * Checks if 2 cfg productions are not the same
     * @param rhs a cfg production
     */
    bool operator!=(const CFGProduction& rhs) const;

    /**
     * returns the action
     * @return the action (string)
     */
    const std::string &getAction() const;

    /**
     * sets the action
     * @param action String which is supposed to become the action
     */
    void setAction(const std::string &action);

private:
    std::string m_head;
    std::vector<std::string> m_body;
    std::string m_action;
};

#endif //LEVELUP_CFGPRODUCTION_H
