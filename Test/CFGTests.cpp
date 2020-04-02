//
// Created by Robbe Van de Velde on 25/11/2018.
//

#include <gtest/gtest.h>
#include <fstream>
#include "../include/CFG.h"

class CFGTests: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
        testCFG= new CFG("Test/testFiles/CFG1.json");
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
        delete testCFG;
    }

    CFG* testCFG = new CFG();
    // Declares the variables your tests want to use.

};

TEST_F(CFGTests, ParseTest){
    EXPECT_EQ("S", testCFG->getVariables()[0]);
    EXPECT_EQ("S", testCFG->getStart());
    EXPECT_EQ("a", testCFG->getTerminals()[0]);
}

TEST_F(CFGTests, RangeTests){
    EXPECT_DEATH(testCFG->deleteVariable(1), "The given integer was out of range for the vector");
    EXPECT_DEATH(testCFG->deleteTerminal(2), "The given integer was out of range for the vector");
    EXPECT_DEATH(testCFG->deleteProduction(2), "The given integer was out of range for the vector");
    EXPECT_TRUE(testCFG->getTerminals()[0] == "a");
    testCFG->deleteTerminal(0);
    EXPECT_FALSE(testCFG->getTerminals()[0] == "a");
    EXPECT_TRUE(testCFG->getVariables()[0] == "S");
    testCFG->deleteVariable(0);
    EXPECT_FALSE(testCFG->getVariables()[0] == "");
}

//tests for failed input
TEST_F(CFGTests, FailInput){
    EXPECT_DEATH(CFG("Test/testFiles/CFGFail1"), "The startsymbol cant be empty");
    EXPECT_DEATH(CFG("Test/testFiles/CFGFail2"), "There was no head given for the production");
    EXPECT_DEATH(CFG("Test/testFiles/CFGFail3"), "Can't have no variables");
}

TEST_F(CFGTests, FunctionTests){
    EXPECT_TRUE(testCFG->isTerminal("a"));
    EXPECT_TRUE(testCFG->isVariable("S"));
    std::vector <std::string> a;
    EXPECT_EQ(a, testCFG->getProductionsGoingTo(a));    //test if there is no production going to a except itself wich implies that there is none
    EXPECT_EQ("", testCFG->getActionFromProduction("a", a));
    a.push_back("a");
    a.push_back("S");
    a.push_back("b");
    EXPECT_EQ("testAction", testCFG->getActionFromProduction("S", a));
    EXPECT_FALSE(testCFG->varHasEpsilonProduction("S"));
}
