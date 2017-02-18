#include "tests.h"

int main(int argc, char **argv){
    //srand(time(0));
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}