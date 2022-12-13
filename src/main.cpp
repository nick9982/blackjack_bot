#include "./game.hpp"

int main(int argc, char **argv)
{
    int avg = 0;
    for(int i = 0; i < 8; i++)
    {
        Game game(6, 10.0, 1000.0, 10000.0, 0.1);
        cout << "game " << i+1 << endl;
        avg += game.run_bot(2);
    }

    cout << "Overall gains: $" << avg/4 << endl;


    return 0;
}
