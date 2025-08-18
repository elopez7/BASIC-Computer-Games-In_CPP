#include "Game.h"
#include <format>
#include <iostream>
#include <random>

Game::Game()
    : funds_ { 100 }
    , betting_amount_ {}
    , valid_bet_ { true }
{
}

Card Game::GetRandomCard()
{
    std::random_device rd {};
    std::mt19937 g { rd() };
    std::uniform_int_distribution card_dist(2, 14);
    return Card { card_dist(g) };
}

void Game::DealInitialCards(Card& card_A, Card& card_B)
{
    do {
        card_A = GetRandomCard();
        card_B = GetRandomCard();

        if (card_A.GetValue() >= card_B.GetValue()) {
            std::cout << std::format("Card A: {} and Card B: {} dealing again.\n", card_A.GetValue(), card_B.GetValue());
        }
    } while (card_A.GetValue() >= card_B.GetValue());
    std::cout << std::format("Here are your next two cards\n{}\n{}\n", card_A.GetValue(), card_B.GetValue());
}

bool Game::PlayRound()
{
    if (funds_ <= 0) {
        return false;
    }

    do {
        std::cout << std::format("{} you have ${}, how much do you want to bet? ", player_name_, funds_);

        while (!(std::cin >> betting_amount_)) {
            std::cout << "Invalid Input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (betting_amount_ == 0) {
            std::cout << "CHICKEN!\n";
            valid_bet_ = true;
        } else if (betting_amount_ > funds_) {
            std::cout << std::format("Sorry {}, you bet too much. You have only ${} to bet.\n", player_name_, funds_);
        } else if (betting_amount_ < 0) {
            std::cout << "You cannot bet a negative amount, that's not how capitalism works!\n";
        } else {
            valid_bet_ = true;
        }
    } while (valid_bet_ == false);

    Card card_A {};
    Card card_B {};
    DealInitialCards(card_A, card_B);

    std::cout << "Dealing Card C\n";
    Card card_C { GetRandomCard() };

    if (card_C.GetValue() > card_A.GetValue() && card_C.GetValue() < card_B.GetValue()) {
        std::cout << std::format("Card C: {}! It looks like we have a winner!\n", card_C.GetValue());
        funds_ += betting_amount_;
        std::cout << std::format("YOU WIN!!\nNOW YOU HAVE ${}.\n", funds_);
    } else {
        std::cout << std::format("Card C: {}! Sorry {}, better luck next time!\n", card_C.GetValue(), player_name_);
        funds_ -= betting_amount_;
        std::cout << std::format("NOW YOU HAVE ${}.\n", funds_);
    }
    return true;
}

void Game::Run()
{
    std::cout << "What is your name, mate? ";
    std::getline(std::cin, player_name_);

    std::cout << "\nACEY-DUCEY IS PLAYED IN THE FOLLOWING MANNER:\n";
    std::cout << "THE DEALER (COMPUTER) DEALS TWO CARDS FACE UP.\n";
    std::cout << "YOU HAVE THE OPTION TO BET OR NOT TO BET DEPENDING\n";
    std::cout << "ON WHETHER OR NOT YOU FEEL THE NEXT CARD WILL HAVE\n";
    std::cout << "A VALUE BETWEEN THE FIRST TWO.\n";
    std::cout << "IF YOU DO NOT WANT TO BET, INPUT A 0.\n";

    bool play_again { true };
    while (funds_ > 0 && play_again) {
        std::cout << std::format("YOU NOW HAVE ${} DOLLARS.\n", funds_);
        PlayRound();
        if (funds_ <= 0) {
            std::cout << std::format("Sorry {}, but you blew all your money.\n", player_name_);
            play_again = false;
        } else {
            std::string response {};
            std::cout << std::format("{}, do you want to play again? ", player_name_);
            std::getline(std::cin, response);
            std::transform(response.begin(), response.end(), response.begin(), ::toupper);
            play_again = (response == "YES");
        }
    }
    std::cout << "O. K. HOPE YOU HAD FUN!\n";
}
