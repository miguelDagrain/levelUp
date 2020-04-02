#include "CFGProduction.h"

const std::string &CFGProduction::getHead() const {
    return m_head;
}

void CFGProduction::setHead(const std::string &head) {
    CFGProduction::m_head = head;
}

const std::vector<std::string> &CFGProduction::getBody() const {
    return m_body;
}

void CFGProduction::setBody(const std::vector<std::string> &head) {
    CFGProduction::m_body = head;
}

CFGProduction::CFGProduction(const std::string &head, const std::vector<std::string> &body) : m_head(head),
                                                                                                  m_body(body) {}

CFGProduction::CFGProduction() {}

CFGProduction::~CFGProduction() {}

void CFGProduction::addTobody(const std::string &toAdd) {
    m_body.push_back(toAdd);
}

void CFGProduction::deleteFromBody(const std::string &toDelete) {
    for(unsigned int i=0;i<m_body.size();i++){
        if(m_body[i]==toDelete){
            m_body.erase(m_body.begin()+i);
        }
    }
}

void CFGProduction::deleteFromBody(const int toDelete) {
    m_body.erase(m_body.begin()+toDelete);
}

bool CFGProduction::operator==(const CFGProduction &rhs) const {
    return m_head == rhs.m_head &&
           m_body == rhs.m_body;
}

bool CFGProduction::operator!=(const CFGProduction &rhs) const {
    return !(rhs == *this);
}
