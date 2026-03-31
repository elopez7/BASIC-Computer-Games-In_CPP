#ifndef CARD_H
#define CARD_H
#include <compare>
#include <format>
#include <array>
#include <string_view>
#include <cassert>

class Card {
public:
    Card() = delete;
    explicit Card(int value) : value_{ value } {
        assert(value >= 2 && value <= 14);
    }
    ~Card() = default;
    auto operator<=>(const Card& other) const = default;
    const int value_;
};

template<>
struct std::formatter<Card>{
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Card& card, std::format_context& ctx) const {
        static constexpr std::array<std::string_view, 4> card_letters{"Jack", "Queen", "King", "Ace"};
        
        if (card.value_ >= 11) {
            return std::format_to(ctx.out(), "{}", card_letters[card.value_ - 11]);
        }
        return std::format_to(ctx.out(), "{}", card.value_);
    }
};


#endif // !CARD_H