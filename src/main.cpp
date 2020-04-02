#include "LevelUp.h"

int main(int argc, char* argv[])
{
    try {
        LevelUp::parseArguments(argc, argv);
    }
    catch (LevelUpExcept& e) {
        cout << e.what() << endl;
    }
}
