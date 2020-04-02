CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(LevelUp)

SET(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_EXE_LINKER_FLAGS -pthread)
FIND_PACKAGE(SFML 2 COMPONENTS system window graphics)


if(UNIX AND NOT APPLE)
    # Set include dir
    include_directories(gtest_linux/include)
    # Set Library dir
    link_directories(gtest_linux/lib)
elseif(APPLE)
    # Set include dir
    include_directories(gtest_mac/include)
    # Set Library dir
    link_directories(gtest_mac/lib)
endif()


SET(SRC
        src/main.cpp
        src/CFG.cpp
        src/CFGProduction.cpp
        src/LLParser.cpp
        src/GameObject.cpp
        src/Game.cpp
        src/Entity.cpp
        src/Object.cpp
        src/Level.cpp
        src/EndObject.cpp
        src/GameObjectToSFML.cpp
        src/TuringMachine.cpp
        include/DesignByContract.h src/LevelUp.cpp include/LevelUp.h)
SET(DEBUG_SOURCE_FILES
        src/CFG.cpp
        src/CFGProduction.cpp
        src/LLParser.cpp
        src/GameObject.cpp
        src/Game.cpp
        src/Entity.cpp
        src/Object.cpp
        src/Level.cpp
        src/EndObject.cpp
        src/TuringMachine.cpp
        include/EndObject.h
        src/GameObjectToSFML.cpp
        include/DesignByContract.h
        Test/TestMain.cpp
        Test/GameTests.cpp
        Test/CFGTests.cpp
        include/DesignByContract.h
        Test/CFGProductionTests.cpp Test/DataStructureTests.cpp)

INCLUDE_DIRECTORIES(
        include
                ${SFML_INCLUDE_DIR}
)

ADD_EXECUTABLE(LevelUP ${SRC})

TARGET_LINK_LIBRARIES(LevelUP ${SFML_LIBRARIES})

INSTALL(TARGETS LevelUP DESTINATION ${PROJECT_SOURCE_DIR})



# Create DEBUG targetsC
ADD_EXECUTABLE(LevelUP_Debug ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(LevelUP_Debug gtest ${SFML_LIBRARIES})

INSTALL(TARGETS LevelUP_Debug DESTINATION ${PROJECT_SOURCE_DIR})