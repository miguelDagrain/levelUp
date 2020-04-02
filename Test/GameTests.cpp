//
// Created by Robbe Van de Velde on 25/11/2018.
//
#include <gtest/gtest.h>
#include <fstream>
#include "../include/Game.h"

class GameTests: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
        testGame= new Game("TestUp");
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
        delete testGame;
    }

    Game* testGame = new Game();
    // Declares the variables your tests want to use.

};
TEST_F(GameTests, testCheck){
    bool test = false;
    ASSERT_FALSE(test);
}