
#include <gtest/gtest.h>
#include <fstream>
#include <memory>
#include "../include/Game.h"
#include "../include/GameObject.h"
#include "../include/Entity.h"
#include "../include/Level.h"
#include "../include/EndObject.h"
#include "../include/Object.h"
class DtataSturctureTests: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {}

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {}

    Game* testGame = new Game();
    // Declares the variables your tests want to use.

};
TEST_F(DtataSturctureTests, ObjectGettersSetters){
    Object temp=Object();

    //testing collision getters/setters
    temp.setCollision(true);
    EXPECT_EQ(true,temp.canCollide());
    temp.setCollision(false);
    EXPECT_EQ(false,temp.canCollide());

    //testing color getters/seeters
    temp.setColor(std::make_shared<Color>(Color(6,7,8)));
    EXPECT_EQ(6,temp.getColor()->r);
    EXPECT_EQ(7,temp.getColor()->g);
    EXPECT_EQ(8,temp.getColor()->b);
    EXPECT_EXIT(temp.setColor(std::make_shared<Color>(Color(-5,7,8))),::testing::AssertionResult,"after initialisation of a color all color values must be above 0");
    EXPECT_EXIT(temp.setColor(std::make_shared<Color>(Color(6,-7,8))),::testing::AssertionResult,"after initialisation of a color all color values must be above 0");
    EXPECT_EXIT(temp.setColor(std::make_shared<Color>(Color(5,7,-8))),::testing::AssertionResult,"after initialisation of a color all color values must be above 0");
    EXPECT_EXIT(temp.setColor(std::make_shared<Color>(Color(6,7,300))),::testing::AssertionResult,"after initialisation of a color all color values must be below 256");
    EXPECT_EXIT(temp.setColor(std::make_shared<Color>(Color(6,358,8))),::testing::AssertionResult,"after initialisation of a color all color values must be below 256");
    EXPECT_EXIT(temp.setColor(std::make_shared<Color>(Color(482,7,8))),::testing::AssertionResult,"after initialisation of a color all color values must be below 256");

    //testing name getters/setters
    temp.setName("testname");
    EXPECT_EQ("testname",temp.getName());
    temp.setName("");
    EXPECT_EQ("",temp.getName());
    temp.setName("afergethsbqfezHKFZAEVKARGHVQETRLIZVJEVRBLFUQLKZFJDVK TSERKVJLHQREYOIVBDOVQEYHBGOISLTJKHQELhikslgqbohv kbdqfbhokqjlef qsgjlkevbktkhfbhvethkjlgb etlkjb khqtrn bknletjbsrlfjdbsektjdfsnboilj kfd");
    EXPECT_EQ("afergethsbqfezHKFZAEVKARGHVQETRLIZVJEVRBLFUQLKZFJDVK TSERKVJLHQREYOIVBDOVQEYHBGOISLTJKHQELhikslgqbohv kbdqfbhokqjlef qsgjlkevbktkhfbhvethkjlgb etlkjb khqtrn bknletjbsrlfjdbsektjdfsnboilj kfd",temp.getName());

    //testing height setter/getter
    temp.setHeight(100);
    EXPECT_EQ(100,temp.getHeight());
    temp.setHeight(0);
    EXPECT_EQ(0,temp.getHeight());
    EXPECT_EXIT(temp.setHeight(-52),::testing::AssertionResult,"height may not be below 0");

    //testing width setter/getter
    temp.setWidth(100);
    EXPECT_EQ(100,temp.getWidth());
    temp.setWidth(0);
    EXPECT_EQ(0,temp.getWidth());
    EXPECT_EXIT(temp.setWidth(-52),::testing::AssertionResult,"width may not be below 0");

    //testing location getter/setter
    temp.setLocation(std::make_pair(0,100));
    EXPECT_EQ(0,std::get<0>(temp.getLocation()));
    EXPECT_EQ(100,std::get<1>(temp.getLocation()));
    temp.setLocation(std::make_pair(0,0));
    EXPECT_EQ(0,std::get<0>(temp.getLocation()));
    EXPECT_EQ(0,std::get<1>(temp.getLocation()));
    temp.setLocation(std::make_pair(0,-100));
    EXPECT_EQ(0,std::get<0>(temp.getLocation()));
    EXPECT_EQ(-100,std::get<1>(temp.getLocation()));
}

TEST_F(DtataSturctureTests, LevelgettersSetters){
    Level temp=Level();
    std::shared_ptr<Object> tempobject=make_shared<Object>(Object());
    tempobject->setName("tester");

    temp.addObject(tempobject);
    EXPECT_EQ("tester",temp.getObjects()[0]->getName());

    tempobject->setName("reformed tester");
    EXPECT_EQ("reformed tester",temp.getObjects()[0]->getName());
}

TEST_F(DtataSturctureTests, EntitygettersSetters){
    Entity temp=Entity();

    //getter and setter tests for jumpheight
    temp.setJumpHeight(100);
    EXPECT_EQ(100,temp.getJumpHeight());
    temp.setJumpHeight(0);
    EXPECT_EQ(0,temp.getJumpHeight());
    EXPECT_EXIT(temp.setJumpHeight(-100),::testing::AssertionResult,"jumheight may not be below 0");

    //getter and setter test for movementspeed
    temp.setMovementSpeed(1025);
    EXPECT_EQ(1025,temp.getMovementSpeed());
    temp.setMovementSpeed(0);
    EXPECT_EQ(0,temp.getMovementSpeed());
    temp.setMovementSpeed(-56);
    EXPECT_EQ(-56,temp.getMovementSpeed());

    //getter and setter test for playerbool
    temp.setPlayerBool(true);
    EXPECT_EQ(true,temp.isPlayer());
    temp.setPlayerBool(false);
    EXPECT_EQ(false,temp.isPlayer());

    //getter and setter test for texturepath
    temp.setTexturePath("testpath");
    EXPECT_EQ("testpath",temp.getTexturePath());
    temp.setTexturePath("notATestPath");
    EXPECT_EQ("notATestPath",temp.getTexturePath());
}

TEST_F(DtataSturctureTests, EndgettersSetters){
    EndObject temp=EndObject();

    //getter and setter test for goto
    temp.setGoto("goingto");
    EXPECT_EQ("goingto",temp.getGoto());
    temp.setGoto("newlevel");
    EXPECT_EQ("newlevel",temp.getGoto());
}

TEST_F(DtataSturctureTests, ObjectNestingTests){
    std::shared_ptr<Object> nest1=std::make_shared<Object>();
    nest1->setName("nest1");
    std::shared_ptr<Object> nest2=std::make_shared<Object>();
    nest2->setName("nest2");
    std::shared_ptr<Object> nest3=std::make_shared<Object>();
    nest3->setName("nest3");
    std::shared_ptr<Object> nest4=std::make_shared<Object>();
    nest4->setName("nest4");
    std::shared_ptr<Object> nest5=std::make_shared<Object>();
    nest5->setName("nest5");

    nest1->addSubObject(nest2);
    nest2->addSubObject(nest3);
    nest3->addSubObject(nest4);
    nest4->addSubObject(nest5);

    EXPECT_EQ("nest1",nest1->getName());
    EXPECT_EQ("nest2",nest1->getSubObjects()[0]->getName());
    EXPECT_EQ("nest3",nest1->getSubObjects()[0]->getSubObjects()[0]->getName());
    EXPECT_EQ("nest4",nest1->getSubObjects()[0]->getSubObjects()[0]->getSubObjects()[0]->getName());
    EXPECT_EQ("nest5",nest1->getSubObjects()[0]->getSubObjects()[0]->getSubObjects()[0]->getSubObjects()[0]->getName());
}