#include "./game.hpp"

int main(int argc, char **argv)
{
    Game game(6, 10, 1000, 0, 0.45);
    int res = game.run_bot(100000);


    return 0;
}
