#include "CFGProduction.h"
#include "DesignByContract.h"

/**
 * returns head of the production
 *@return head
 */
const std::string& CFGProduction::getHead() const
{
    return m_head;
}

/**
 * Sets the head of a production
 * @param string String that is supposed to become the head
 */
void CFGProduction::setHead(const std::string& head)
{
    CFGProduction::m_head = head;
}

/**
 * returns the body of a CFG production
 * @return the vector of the body
 */
const std::vector<std::string>& CFGProduction::getBody() const
{
    return m_body;
}

/**
 * sets the body
 * @param head The vector of strings that is supposed to become the body
 */
void CFGProduction::setBody(const std::vector<std::string>& head)
{
    CFGProduction::m_body = head;
}

/**
 * Constructor by value
 * @param head the head of the production (where the production comes from)
 * @param body the body of the production (all the symbols teh production goes to)
 * @param action the action that this production should take. This is only used in the parsing process
 */
CFGProduction::CFGProduction(const std::string& head, const std::vector<std::string>& body, const std::string& action)
        :m_head(head),
         m_body(body),
         m_action(action) { }

/**
 * Adds a string to body
 * @param toAdd The string that needs to be added
 */
void CFGProduction::addTobody(const std::string& toAdd)
{
    m_body.push_back(toAdd);
}

/**
 * Deletes a string from the body
 * @param toDelete The string which needs to be deleted.
 */
void CFGProduction::deleteFromBody(const std::string& toDelete)
{
    for (unsigned int i = 0; i<m_body.size(); i++) {
        if (m_body[i]==toDelete) {
            m_body.erase(m_body.begin()+i);
        }
    }
}

/**
 * Deletes a string from the body with given integer
 * @param toDelete   integer wich is supposed to be the index of the item
 */
void CFGProduction::deleteFromBody(const int toDelete)
{
    ENSURE(toDelete<m_body.size(), "The given integer was out of range for the vector");
    m_body.erase(m_body.begin()+toDelete);
}

/**
 * Checks if 2 CFG productions have the same items
 * @param rhs a CFG production
 * @return  true if they are the same, else: false
 */
bool CFGProduction::operator==(const CFGProduction& rhs) const
{
    return m_head==rhs.m_head &&
            m_body==rhs.m_body &&
            m_action==rhs.m_action;
}

/**
 * Checks if 2 cfg productions are not the same
 * @param rhs a cfg production
 */
bool CFGProduction::operator!=(const CFGProduction& rhs) const
{
    return !(rhs==*this);
}

/**
 * returns the action
 * @return the action (string)
 */
const std::string& CFGProduction::getAction() const
{
    return m_action;
}

/**
 * sets the action
 * @param action String which is supposed to become the action
 */
void CFGProduction::setAction(const std::string& action)
{
    CFGProduction::m_action = action;
}
