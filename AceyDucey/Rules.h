#ifndef RULES_H
#define RULES_H
#include "Card.h"

constexpr bool IsPairValid(const Card& low, const Card& high){
	return low < high;
}

constexpr bool IsDrawnCardWithinRange(const Card& low, const Card& high, const Card& drawn) {
	return (drawn > low && drawn < high);
}

constexpr bool IsBetValid(int bettingAmount, int playerFunds) {
	return (bettingAmount <= playerFunds && bettingAmount > 0);
}





#endif // !RULES_H
