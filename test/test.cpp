
# include <gtest/gtest.h>

# include "GenADTests.cpp"

int main(int argc, char **argv) {
    std::cout << "Running all Tests!\n";
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
