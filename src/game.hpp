#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <stack>
#include <unordered_map>
#include <chrono>
#define noop (void)0

using namespace std;

class BasicStrategy
{
    unordered_map<string, char> bs_map;

    public:
        BasicStrategy();
        char get_strategy(bool, bool, int, int);
};

class BetSpread{
    unordered_map<int, int> count_to_bet;

    public:
        BetSpread();
        int get_bet(int);
};

class Deck{
    char *cards = new char[52];
    int length = 52;
    int ptr = 0;
    public:
        Deck();
        void print_deck();
        bool next();
        char get();
};

int countCard(char);

class Hand{
    public:
        vector<char> cards;
        double bet;
        int cut;
        int score = 0;
        bool handAlive = true;
        bool isWon = false;
        bool isTie = false;
        bool isNatural = false;
        bool isBust = false;
        bool isSurrender = false;
        bool first = true;
        Hand(double);
        int addCard(char);
        void checkNatural();
        void check();
        void print_initial();
};

class House{
    public:
        vector<char> cards;
        int score;
        bool handAlive = true;
        bool isBust = false;
        bool has21 = false;
        int addCard(char, bool);
        void print_dealers();
        void check();
        void print_initial();
};

class Game{
    vector<Hand> hands;
    int noHands;
    vector<int> status;
    double cut;
    int decks;
    double bank;
    double difference;
    double min;
    double max;
    double TC;//true count
    int C;//count
    vector<char> shoe;
    int total_cards;
    vector<char> discards;
    int cardsOutShoe = 0;
    double decksInShoe;
    House house;

    public:
        Game(int, double, double, double, double);
        void start();
        int run_bot(int);
        void initial_deal();
        char draw_from_shoe();
        void display_initial_hand();
        void display_all_hands();
        void display_hand_for_action(int);
        void reshuffle();
    private:
        void shuffle();
};