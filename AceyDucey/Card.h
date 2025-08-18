#ifndef CARD_H
#define CARD_H
#include <string>

class Card {
public:
    Card() = default;
    explicit Card(int value);
    virtual ~Card() = default;
    int GetValue() const;

private:
    std::string to_string() const;
    int value_;
};

#endif // !CARD_H
