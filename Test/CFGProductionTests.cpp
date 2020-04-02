//
// Created by Robbe Van de Velde on 2018-12-18.
//

#include <gtest/gtest.h>
#include <fstream>
#include "../include/CFGProduction.h"

class CFGProductionTests: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
        testCFGProduction= new CFGProduction();
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
        delete testCFGProduction;
    }

    CFGProduction* testCFGProduction = new CFGProduction();
    // Declares the variables your tests want to use.

};

TEST_F(CFGProductionTests, INITTest){
    EXPECT_EQ("", testCFGProduction->getHead());
    EXPECT_EQ("", testCFGProduction->getAction());
}

TEST_F(CFGProductionTests, functiontest){
    testCFGProduction->setHead("a");
    std::vector <std::string> a;
    a.push_back("a");
    a.push_back("b");
    a.push_back("c");
    testCFGProduction->setBody(a);
    testCFGProduction->setAction("GoToEnd");
    testCFGProduction->deleteFromBody(0);
    EXPECT_EQ("b", testCFGProduction->getBody()[0]);
    EXPECT_DEATH(testCFGProduction->deleteFromBody(8), "The given integer was out of range for the vector");
}

