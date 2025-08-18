#include "Card.h"

Card::Card(int value)
    : value_ { value }
{
}

int Card::GetValue() const
{
    return value_;
}

std::string Card::to_string() const
{
    if (value_ >= 2 && value_ <= 10) {
        return std::to_string(value_);
    }

    switch (value_) {
    case 11:
        return "JACK";
    case 12:
        return "QUEEN";
    case 13:
        return "KING";
    case 14:
        return "ACE";
    default:
        break;
    }
}
