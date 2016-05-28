#ifndef LONGINT_H
#define LONGINT_H
#include <iostream>
#include "deque.h"

using namespace std;

/// <summary>
/// LongInt class holds a deque of chars representing
/// each digit in a large Integer, the LongInt can be
/// positive or negative, LongInt objects are comparable with 
/// each other and there are operators for adding
/// or subtracting LongInt objects 
/// </summary>
class LongInt {
	friend istream &operator >> (istream &in, LongInt &rhs);
	friend ostream &operator<<(ostream &out, const LongInt &rhs);
public:
	// Constructors
	LongInt(const string str);
	LongInt(const LongInt &rhs);
	LongInt();

	// Destructor
	~LongInt();

	// Arithmetic binary operators
	LongInt operator+(const LongInt &rhs) const;
	LongInt operator-(const LongInt &rhs) const;
	LongInt operator/(const LongInt &rhs) const;
	LongInt operator*(const LongInt &rhs) const;

	// assignment operators
	const LongInt &operator=(const LongInt &rhs);

	// Logical binary operators
	bool operator< (const LongInt & rhs) const;
	bool operator<=(const LongInt & rhs) const;
	bool operator> (const LongInt & rhs) const;
	bool operator>=(const LongInt & rhs) const;
	bool operator==(const LongInt & rhs) const;
	bool operator!=(const LongInt & rhs) const;

private:
	Deque<char> digits;
	bool negative;
	void remove0s();
	int asciiCharToInt(const char c) const;
	char oneDigitIntToAsciiChar(int a) const;
	LongInt processAddened(int currentAddened, LongInt sum) const;
	int calcCarry(int ten, int currentAddened) const;
	int calcTest(Deque<char> minuend, int test) const;
	int calcBorrow(Deque<char> minuend, int test) const;
	bool isDiffNegative(Deque<char> minuend, int test) const;
	LongInt multiply(LongInt a, LongInt b) const;
};

#endif
