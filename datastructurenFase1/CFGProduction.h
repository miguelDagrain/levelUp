#ifndef LEVELUP_CFGPRODUCTION_H
#define LEVELUP_CFGPRODUCTION_H


#include <vector>
#include <iostream>

class CFGProduction {
public:
    CFGProduction(const std::string &head, const std::vector<std::string> &body);

    CFGProduction();

    const std::string &getHead() const;

    void setHead(const std::string &head);

    const std::vector<std::string> &getBody() const;

    void setBody(const std::vector<std::string> &head);

    void addTobody(const std::string& toAdd);

    void deleteFromBody(const std::string& toDelete);

    void deleteFromBody(int toDelete);

    virtual ~CFGProduction();

    bool operator==(const CFGProduction &rhs) const;

    bool operator!=(const CFGProduction &rhs) const;

private:

    std::string m_head;

    std::vector<std::string> m_body;
};


#endif //LEVELUP_CFGPRODUCTION_H
