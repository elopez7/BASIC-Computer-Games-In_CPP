#ifndef GAME_H
#define GAME_H
#include "Card.h"
#include <string>

class Game {
public:
    Game();
    ~Game() = default;
    Card GetRandomCard();
    void DealInitialCards(Card& card_A, Card& card_B);
    bool PlayRound();
    void Run();

private:
    int funds_;
    int betting_amount_;
    bool valid_bet_;
    std::string player_name_;
};

#endif // !GAME_H
