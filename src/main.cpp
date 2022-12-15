#include "./game.hpp"

int main(int argc, char **argv)
{
    Game game(6, 10, 1000, 0, 0.4);
    int res = game.run_bot(100000, {10, 50, 75, 300, 400, 500});


    return 0;
}
