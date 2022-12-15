#include "./game.hpp"

using namespace std;
namespace plt = matplotlibcpp;

Deck::Deck()
{
    for(int i = 0; i < 13; i++)
    {
        char card = 0;
        switch(i)
        {
            case 8:
                card = 84;
                break;
            case 9:
                card = 74;
                break;
            case 10:
                card = 81;
                break;
            case 11:
                card = 75;
                break;
            case 12:
                card = 65;
                break;
            default:
                card = i + 50;
                break;
        }
        for(int j = 0; j < 4; j++)
        {
            this->cards[4*i + j] = card;
        }
    }
}

void Deck::print_deck()
{
    for(int i = 0; i < this->length; i++)
    {
        cout << this->cards[i] << endl;
    }
}

bool Deck::next()
{
    if(ptr == 52)
    {
        ptr = 0;
        return 0;
    }
    return 1;
};

char Deck::get()
{
    return cards[ptr++];
}

int randomize()
{
    auto start = chrono::_V2::high_resolution_clock::now();
    for(int i = 0; i < 200000; i++)
    {
        noop;
    }
    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(end-start);
    return duration.count();
}

Game::Game(int decks, double min, double max, double money, double cut)
{
    this->cut = decks * cut;
    this->bank = money;
    this->decksInShoe = decks;
    this->decks = decks;
    this->total_cards = decks*52;
    this->min = min;
    this-> max = max;
    this->C = 0;


    for(int i = 0; i < decks; i++)
    {
        Deck deck;
        while(deck.next())
        {
            this->shoe.push_back(deck.get());
        }
    }

    shuffle();
}

BasicStrategy::BasicStrategy()
{
    string key = "";
    //key isPair, isA, player hand, dealer hand
    for(int i = 2; i < 22; i++)
    {
        for(int j = 2; j < 12; j++)
        {
            key.append("0");
            key.append("0");
            key.append(to_string(i));
            key.append(to_string(j));
            if(i >= 17) this->bs_map[key] = 's';
            else if(j == 2 && (i == 11 || i == 10)) this->bs_map[key] = 'd';
            else if(j >= 3 && j <= 6 && i >= 9 && i <= 11) this->bs_map[key] = 'd';
            else if(j <= 3 && i >= 13) this->bs_map[key] = 's';
            else if(j >= 4 && j <= 6 && i >= 12) this->bs_map[key] = 's';
            else if(j == 10 && i == 11) this->bs_map[key] = 'd';
            else if((i == 11 || i == 10) && j <= 9 && j >= 7) this->bs_map[key] = 'd';
            else this->bs_map[key] = 'h';
            key = "";
        }
    }

    for(int i = 2; i < 11; i++)
    {
        for(int j = 2; j < 12; j++)
        {
            key.append("0");
            key.append("1");
            key.append(to_string(i));
            key.append(to_string(j));
            if(i >= 8) this->bs_map[key] = 's';
            else if((i == 4 || i == 5) && j >= 4 && j <= 6) this->bs_map[key] = 'd';
            else if((i == 6 || i == 7) && j >= 3 && j <= 6) this->bs_map[key] = 'd';
            else if(i == 7 && j == 2) this->bs_map[key] = 's';
            else if((i == 2 || i == 3) && (j == 5 || j == 6)) this->bs_map[key] = 'd';
            else if(i == 7 && (j == 7 || j == 8)) this->bs_map[key] = 's';
            else this->bs_map[key] = 'h';
            key = "";
        }
    }

    for(int i = 2; i < 12; i++)
    {
        for(int j = 2; j < 12; j++)
        {
            key.append("1");
            if(i == 11)key.append("1");
            else key.append("0");

            key.append(to_string(i));
            key.append(to_string(j));
            if(i == 8 || i == 11) this->bs_map[key] = 'p';
            else if(i == 10) this->bs_map[key] = 's';
            else if(i <= 9 && i >= 6 && j <= 6) this->bs_map[key] = 'p';
            else if(i == 7 && j == 7) this->bs_map[key] = 'p';
            else if(i == 9 && j == 7) this->bs_map[key] = 's';
            else if(i == 9 && (j == 8 || j == 9)) this->bs_map[key] = 'p';
            else if(i == 9 && j >= 10) this->bs_map[key] = 's';
            else if(i == 5 && j <= 9) this->bs_map[key] = 'd';
            else if(i == 4 && (j == 5 || j == 6)) this->bs_map[key] = 'p';
            else if((i == 2 || i == 3) && j <= 7) this->bs_map[key] = 'p';
            else this->bs_map[key] = 'h';
            key = "";
        }
    }
}

char BasicStrategy::get_strategy(bool isPair, bool isAce, int hand, int house)
{
    string key = "";
    if(isPair) key.append("1");
    else key.append("0");

    if(isAce) key.append("1");
    else key.append("0");

    key.append(to_string(hand));
    key.append(to_string(house));

    return this->bs_map[key];
}

BetSpread::BetSpread(vector<int> bets)
{
    this->count_to_bet[0] = {1, bets[0]};
    this->count_to_bet[1] = {1, bets[1]};
    this->count_to_bet[2] = {1, bets[2]};
    this->count_to_bet[3] = {1, bets[3]};
    this->count_to_bet[4] = {2, bets[4]};
    this->count_to_bet[5] = {2, bets[5]};
    cout << bets[0] << " s " << bets[5] << endl;
}

pair<int, int> BetSpread::get_bet(int count)
{
    if(count < 0) count = 0;
    if(count > 5) count = 5;
    return this->count_to_bet[count];
}

int Game::run_bot(int game_loops, vector<int> bets)
{
    BasicStrategy basicStrategy;
    BetSpread betSpread(bets);
    int original_bank = this->bank;
    int hand_cnt = game_loops;
    vector<int> time;
    vector<double> bank_at_time;
    while(game_loops > 0)
    {
        time.push_back(hand_cnt-game_loops);
        pair<int, int> decisions = betSpread.get_bet(this->TC);
        for(int i = 0; i < decisions.first; i++)
        {
            Hand hand(decisions.second);
            this->hands.push_back(hand);
        }

        this->initial_deal();
        char card = this->house.cards[0];
        int housescore = 0;
        switch(card)
        {
            case '2':
                housescore = 2;
                break;
            case '3':
                housescore = 3;
                break;
            case '4':
                housescore = 4;
                break;
            case '5':
                housescore = 5;
                break;
            case '6':
                housescore = 6;
                break;
            case '7':
                housescore = 7;
                break;
            case '8':
                housescore = 8;
                break;
            case '9':
                housescore = 9;
                break;
            case 'A':
                housescore = 11;
                break;
            default:
                housescore = 10;
                break;
        }

        for(int i = 0; i < decisions.first; i++)
        {
            int rounds = 0;
            this->hands[i].check();
            while(this->hands[i].handAlive)
            {
                char action;
                int score = this->hands[i].score;
                if(rounds == 0)
                {
                    if(this->hands[i].cards[0] == this->hands[i].cards[1])
                    {
                        if(this->hands[i].cards[0] == 'A') action = basicStrategy.get_strategy(true, true, 11, housescore);
                        else
                        {
                            score = score/2;
                            action = basicStrategy.get_strategy(true, false, score, housescore);
                        }
                    }
                    else if(this->hands[i].cards[0] == 'A' || this->hands[i].cards[1] == 'A')
                    {
                        score -= 11;
                        action = basicStrategy.get_strategy(false, true, score, housescore);
                    }
                    else
                    {
                        action = basicStrategy.get_strategy(false, false, score, housescore);
                    }
                }
                else
                {
                    //cout << "house score: " << housescore << " player score: " << this->hands[i].score << endl;
                    action = basicStrategy.get_strategy(false, false, score, housescore);
                }
                if(action == 's')
                {
                    this->hands[i].handAlive = false;
                }
                else if(action == 'h')
                {
                    this->C += this->hands[i].addCard(draw_from_shoe());
                    this->TC = this->C / this->decksInShoe;
                    this->hands[i].check();
                }
                else if(action == 'd')
                {
                    if(rounds != 0)
                    {
                        this->C += this->hands[i].addCard(draw_from_shoe());
                        this->TC = this->C / this->decksInShoe;
                        this->hands[i].check();                    
                    }
                    else
                    {
                        this->hands[i].bet = this->hands[i].bet * 2;
                        this->C += this->hands[i].addCard(draw_from_shoe());
                        this->TC = this->C / this->decksInShoe;
                        this->hands[i].check();
                        this->hands[i].handAlive = false;
                    }
                }
                else if(action == 'p')
                {
                    if(this->hands[i].cards[0] == this->hands[i].cards[1] && rounds == 0)
                    {
                        char tmp = this->hands[i].cards[0];
                        this->hands[i].cards.erase(this->hands[i].cards.begin()+1, this->hands[i].cards.end());
                        Hand newHand(this->hands[i].bet);
                        newHand.cards.push_back(tmp);
                        this->hands.insert(this->hands.begin()+i, newHand);
                        this->hands[i].check();
                    }
                }
                //cout  << rounds << " " << endl;
                rounds++;
            }
        }

        this->house.check();

        this->C += countCard(this->house.cards[0]);
        while(this->house.handAlive)
        {
            this->C += this->house.addCard(draw_from_shoe(), false);
            this->house.check();
        }

        this->TC = this->C / this->decksInShoe;
        int totalWin = 0;
        for(int i = 0; i < this->hands.size(); i++)
        {
            this->hands[i].check();
            if(!this->hands[i].isBust)
            {
                if(this->house.isBust)
                {
                    this->hands[i].isWon = true;
                }
                else if(this->hands[i].score > this->house.score)
                {
                    this->hands[i].isWon = true;
                }
                else if(this->hands[i].score == this->house.score)
                {
                    this->hands[i].isTie = true;
                }
            }
            if(!this->hands[i].isTie)
            {
                if(this->hands[i].isWon)
                {
                    if(this->hands[i].isNatural) totalWin += this->hands[i].bet * 1.5;
                    else totalWin += this->hands[i].bet;
                }
                else if(!this->hands[i].isWon) totalWin -= this->hands[i].bet;
            }
        }

        //this->house.print_dealers();
        //this->display_all_hands();
        this->bank += totalWin;
        this->hands.clear();
        this->house.cards.clear();
        this->house.handAlive = true;
        this->house.isBust = false;
        this->house.has21 = false;

        if(this->decksInShoe < this->cut)
        {
            this->reshuffle();
        }
        bank_at_time.push_back(this->bank);
        game_loops--;
    }

    plt::plot(time, bank_at_time);
    plt::xlabel("hands played");
    plt::ylabel("profit");
    plt::title("Blackjack Spread Statistics");
    plt::show();
    return this->bank - original_bank;
}

Hand::Hand(double bet)
{
    this->bet = bet;
}

void Game::start()
{
    while(true)
    {
        int hands;
        cout << "How many hands?" << endl;
        cin >> hands;

        this->noHands = hands;

        for(int i = 0; i < hands; i++)
        {
            double bet;
            cout << "Bet for hand " << i+1 << endl;
            cin >> bet;
            while(bet < this->min || bet > this->max)
            {
                cout << "Bet for hand " << i+1 << endl;
                cin >> bet;
            }
            Hand hand(bet);
            this->hands.push_back(hand);
        }

        //initial deal
        this->initial_deal();

        this->display_initial_hand();
		for(int i = 0; i < hands; i++)
		{
            int rounds = 0;
            this->hands[i].check();
            while(this->hands[i].handAlive)
            {
                this->display_hand_for_action(i);
                char action;
                cout << "Action?" << endl;
                //stand - s
                //hit - h
                //double down - d
                //split pairs - p
                //surrender - S
                cin >> action;
                if(action == 's')
                {
                    this->hands[i].handAlive = false;
                }
                else if(action == 'h')
                {
                    this->C += this->hands[i].addCard(draw_from_shoe());
                    this->TC = this->C / this->decksInShoe;
                    this->hands[i].check();
                }
                else if(action == 'd')
                {
                    this->hands[i].bet = this->hands[i].bet * 2;
                    this->C += this->hands[i].addCard(draw_from_shoe());
                    this->TC = this->C / this->decksInShoe;
                    this->hands[i].check();
                    this->hands[i].handAlive = false;
                }
                else if(action == 'p')
                {
                    if(this->hands[i].cards[0] == this->hands[i].cards[1] && rounds == 0)
                    {
                        char tmp = this->hands[i].cards[0];
                        this->hands[i].cards.erase(this->hands[i].cards.begin()+1, this->hands[i].cards.end());
                        Hand newHand(this->hands[i].bet);
                        newHand.cards.push_back(tmp);
                        this->hands.insert(this->hands.begin()+i, newHand);
                    }
                }
                else if(action == 'S')
                {
                    if(rounds == 0)
                    {
                        this->hands[i].handAlive = false;
                        this->hands[i].isSurrender = true;
                    }
                }
                rounds++;
            }
            this->display_hand_for_action(i);
            cout << "Dealers deck:" << endl;
            this->house.print_initial();

		}

        //the following code might not be necessary will see in a bit
        this->house.check();
        this->C += countCard(this->house.cards[0]);
        while(this->house.handAlive)
        {
            this->C += this->house.addCard(draw_from_shoe(), false);
            this->house.check();
        }
        this->TC = this->C / this->decksInShoe;
        int totalWin = 0;
        for(int i = 0; i < this->hands.size(); i++)
        {
            this->hands[i].check();
            if(!this->hands[i].isBust && !this->hands[i].isSurrender)
            {
                if(this->house.isBust)
                {
                    this->hands[i].isWon = true;
                }
                else if(this->hands[i].score > this->house.score)
                {
                    this->hands[i].isWon = true;
                }
                else if(this->hands[i].score == this->house.score)
                {
                    this->hands[i].isTie = true;
                }
            }
            if(!this->hands[i].isTie)
            {
                if(this->hands[i].isWon)
                {
                    if(this->hands[i].isNatural) totalWin += this->hands[i].bet * 1.5;
                    else totalWin += this->hands[i].bet;
                }
                else if(this->hands[i].isSurrender) totalWin -= this->hands[i].bet/2;
                else if(!this->hands[i].isWon) totalWin -= this->hands[i].bet;
            }
        }

        this->bank += totalWin;
        this->house.print_dealers();
        cout << endl << this->house.score;
        this->display_all_hands();
        if(totalWin > 0)cout << "You have won $" << totalWin << endl;
        else if(totalWin == 0) cout << "You have tied" << endl;
        else if(totalWin < 0) cout << "You have lost $" << -1*totalWin << endl;
        cout << "New balance: $" << this->bank << endl;
        this->hands.clear();
        this->house.cards.clear();
        this->house.handAlive = true;
        this->house.isBust = false;
        this->house.has21 = false;

        //check if deck is to be shuffled
        if(this->decksInShoe < this->cut)
        {
            this->reshuffle();
        }
    }
}

void Game::reshuffle()
{
    this->discards.clear();
    this->shoe.clear();
    this->decksInShoe = this->decks;
    this->cardsOutShoe = 0;
    this->C = 0;
    this->TC = 0;

    for(int i = 0; i < this->decks; i++)
    {
        Deck deck;
        while(deck.next())
        {
            this->shoe.push_back(deck.get());
        }
    }

    shuffle();
}

void House::print_dealers()
{
    cout << "Dealers deck:" << endl;
    for(int i = 0; i < this->cards.size(); i++)
    {
        if(i == this->cards[i] - 1) cout << this->cards[i] << endl;
        else cout << this->cards[i] << "   ";
    }
}

int sumHand(vector<char> cards)
{
    int sum = 0;
    int aCnt = 0;
    for(int i = 0; i < cards.size(); i++)
    {
        switch (cards[i])
        {
            case '2':
                sum += 2;
                break;
            case '3':
                sum += 3;
                break;
            case '4':
                sum += 4;
                break;
            case '5':
                sum += 5;
                break;
            case '6':
                sum += 6;
                break;
            case '7':
                sum += 7;
                break;
            case '8':
                sum += 8;
                break;
            case '9':
                sum += 9;
                break;
            case 'A':
                aCnt++;
                break;
            default:
                sum += 10;
                break;
        }
    }

    for(int i = 0; i < aCnt; i++)
    {
        if((sum + 11) > 21)
        {
            sum += 1;
        }
        else if(i > 0) sum += 1;
        else sum += 11;
    }

    return sum;
}

void House::check()
{
    this->score = sumHand(this->cards);
    if(score > 21)
        this->isBust = true;
    if(score >= 17)
        this->handAlive = false;
}

void Hand::check()
{
    this->score = sumHand(this->cards);
    if(score > 21)
    {
        this->handAlive = false;
        this->isBust = true;
    }
    if(score == 21)
    {
        this->handAlive = false;
        if(this->first) this->isNatural = true;
    }
    this->first = false;
}

char Game::draw_from_shoe()
{
    this->cardsOutShoe++;
    this->decksInShoe = ((double)(this->total_cards - this->cardsOutShoe))/52;
    if(this->shoe.empty()) return 0;
    char tmp = this->shoe.back();
    this->shoe.pop_back();
    return tmp;
}

void Game::initial_deal()
{
    for(int j = 0; j < 2; j++)
    {
        for(int i = 0; i < this->hands.size()+1; i++)
        {
            if(i == this->hands.size())
            {
                if(j == 1)
                {
                    this->C += this->house.addCard(draw_from_shoe(), false);
                    this->TC = this->C / this->decksInShoe;
                }
                else
                {
                    this->C += this->house.addCard(draw_from_shoe(), true);
                    this->TC = this->C / this->decksInShoe;
                }
            }
            else
            {
                this->C += this->hands[i].addCard(draw_from_shoe());
                this->TC = this->C / this->decksInShoe;
                if(i == 1) this->hands[i].checkNatural();
            }
        }
    }
}

void Hand::checkNatural()
{
    if(sumHand(this->cards) == 21)
    {
        this->isNatural = true;
        this->handAlive = false;
    }
}

string parseDoubleStr(string str)
{
    string res = "";
    for(int i = 0; i < str.length(); i++)
    {
        if(str.at(i) == '.')
        {
            res = str.substr(0, i+3);
        }
    }
    return res;
}

void Game::display_hand_for_action(int hand)
{
    Hand hand_ = this->hands[hand];
    string status = "";
    if(hand_.handAlive) status = "Active";
    else if(hand_.isBust) status = "Busted";
    else status = "stand";
    cout << "count: " << this->C << " true count: " << this->TC << " bank: $" << this->bank;
    cout << " decks: " << this->decksInShoe << " status: " << status << endl;

    cout << "+----";

    for(int i = 0; i < hand_.cards.size(); i++)
    {
        if(i == hand_.cards.size() - 1) cout << "-----+";
        else cout << "-----";
    }

    cout << endl << "| #"<<hand+1<<" ";

    for(int i = 0; i < hand_.cards.size(); i++)
    {
        if( i == hand_.cards.size() - 1) cout << "     |";
        else cout << "     ";
    }

    cout << endl << "|    ";
    for(int i = 0; i < hand_.cards.size(); i++)
    {
        if( i == hand_.cards.size() - 1) cout << "     |";
        else cout << "     ";
    }

    cout << endl << "|    ";
    for(int i = 0; i < hand_.cards.size(); i++)
    {
        if(i == hand_.cards.size()-1) cout << hand_.cards[i] << "    |";
        else cout << hand_.cards[i] << "    ";
    }

    cout << endl << "|    ";
    for(int i = 0; i < hand_.cards.size(); i++)
    {
        if( i == hand_.cards.size() - 1) cout << "     |";
        else cout << "     ";
    }

    double bet = hand_.bet;
    string bets = parseDoubleStr(to_string(bet));
    int lenbet = bets.length();
    int lenRow = hand_.cards.size() * 5 - 2;
    int len = lenRow - lenbet;

    cout << endl << "|BET: $" << bets;
    for(int i = 0; i < len; i++)
    {
        cout << " ";
    }
    cout <<"|"<< endl << "+----";
    for(int i = 0; i < hand_.cards.size(); i++)
    {
        if(hand_.cards.size()-1 == i) cout << "-----+" << endl;
        else cout << "-----";
    }
}

void Game::display_initial_hand()
{
    cout << "count: " << this->C << " true count: " << this->TC << " bank: $" << this->bank;
    cout << " decks: " << this->decksInShoe << endl;
    cout << endl << "dealers hand:" << endl;
    this->house.print_initial();
    cout << endl;

    //preparing to print user deck
    cout <<"+";
    for(int i = 0; i < noHands; i++)
    {
        cout << "--------------+";
    }
    cout << endl << "|";
    for(int i = 0; i < noHands; i++)
    {
        cout << " #"<<i+1<<"           |";
    }
    cout << endl << "|";
    for(int i = 0; i < noHands; i++)
    {
        cout << "              |";
    }
    cout << endl << "|";
    for(int i = 0; i < noHands; i++)
    {
        this->hands[i].print_initial();
    }
    cout << endl << "|";
    for(int i = 0; i < noHands; i++)
    {
        cout << "              |";
    }
    cout << endl << "|";
    for(int i = 0; i < noHands; i++)
    {
        double bet = this->hands[i].bet;
        string bets = parseDoubleStr(to_string(bet));
        int len = 8-bets.length();
        cout << "BET: $" << bets;
        for(int j = 0; j < len; j++)
        {
            cout << " ";
        }
        cout << "|";
    }
    cout << endl;
    cout <<"+";
    for(int i = 0; i < noHands; i++)
    {
        cout << "--------------+";
    }

    cout << endl;
}

void Hand::print_initial()
{
    cout << "    " << this->cards[0] << "    " << this->cards[1] << "    |";
}

void Game::shuffle()
{
    auto rng = default_random_engine {};
    rng.seed(randomize());
    std::shuffle(begin(this->shoe), end(this->shoe), rng);
}

int House::addCard(char card, bool isHidden)
{
    this->cards.push_back(card);
    if(isHidden) return 0;
    return countCard(card);
}

int Hand::addCard(char card)
{
    this->cards.push_back(card);
    return countCard(card);
}
void House::print_initial()
{
    cout << this->cards[0] << "  -FD-" << endl;
}

int countCard(char card)
{
    switch(card)
    {
        case '2':
            return 1;
        case '3':
            return 1;
        case '4':
            return 1;
        case '5':
            return 1;
        case '6':
            return 1;
        case 'T':
            return -1;
        case 'J':
            return -1;
        case 'Q':
            return -1;
        case 'K':
            return -1;
        case 'A':
            return -1;
        default:
            return 0;
    }
}


void Game::display_all_hands()
{
    cout << endl << "+----";
    for(int i = 0; i < this->hands.size(); i++)
    {
        if( i != 0) cout << "----";
        for(int j = 0; j < this->hands[i].cards.size(); j++)
        {
            if(j == this->hands[i].cards.size() - 1) cout << "-----+";
            else cout << "-----";
        }
    }

    cout << endl << "| #"<<1<<" ";
    for(int i = 0; i < this->hands.size(); i++)
    {
        if(i != 0) cout << " #"<<i+1<<" ";
        for(int j = 0; j < this->hands[i].cards.size(); j++)
        {
            if(j == this->hands[i].cards.size() - 1) cout << "     |";
            else cout << "     ";
        }
    }

    cout << endl << "|    ";
    for(int i = 0; i < this->hands.size(); i++)
    {
        if(i != 0) cout << "    ";
        for(int j = 0; j < this->hands[i].cards.size(); j++)
        {
            if(j == this->hands[i].cards.size() - 1) cout << "     |";
            else cout << "     ";
        }
    }
    
    cout << endl << "|    ";
    for(int i = 0; i < this->hands.size(); i++)
    {
        if(i != 0) cout << "    ";
        for(int j = 0; j < this->hands[i].cards.size(); j++)
        {
            if(j == this->hands[i].cards.size() - 1) cout << this->hands[i].cards[j] << "    |";
            else cout << this->hands[i].cards[j] << "    ";
        }
    }

    cout << endl << "|    ";
    for(int i = 0; i < this->hands.size(); i++)
    {
        if(i != 0) cout << "    ";
        for(int j = 0; j < this->hands[i].cards.size(); j++)
        {
            if(j == this->hands[i].cards.size() - 1) cout << "     |";
            else cout << "     ";
        }
    }

    cout << endl << "|";
    for(int i = 0; i < this->hands.size(); i++)
    {
        double bet = this->hands[i].bet;
        string bets = parseDoubleStr(to_string(bet));
        int lenbet = bets.length() + 2;
        int lenRow = this->hands[i].cards.size() * 5;
        int len = lenRow - lenbet;
        cout << "BET: $" << bets;
        for(int i = 0; i < len; i++)
        {
            cout << " ";
        }
        cout << "|";
    }

    cout << endl << "|";
    for(int i = 0; i < this->hands.size(); i++)
    {
        string status = "";
        if(this->hands[i].isNatural && this->hands[i].isWon) status = "NATURAL";
        else if(this->hands[i].isWon) status = "WIN";
        else if(this->hands[i].isBust) status = "BUSTED";
        else if(this->hands[i].isTie) status = "PUSH";
        else status = "LOST";

        int lenStatus = status.size() + 4;
        int lenRow = this->hands[i].cards.size() * 5;
        int len = lenRow - lenStatus;
        cout << "STATUS: " << status;
        for(int i = 0; i < len; i++)
        {
            cout << " ";
        }
        cout << "|";
    }
    cout << endl << "+----";
    for(int i = 0; i < this->hands.size(); i++)
    {
        if(i != 0) cout << "----";
        for(int j = 0; j < this->hands[i].cards.size(); j++)
        {
            if(j == this->hands[i].cards.size()-1) cout << "-----+";
            else cout << "-----";
        }
    }
    cout << endl;
}