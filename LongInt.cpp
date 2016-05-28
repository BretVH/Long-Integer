#include "longint.h"
#include<iostream>
#include <string.h>
#include <string>
#include <typeinfo>

using namespace std;

/// <summary>
/// Operator>> adds the numeric
/// digits input specified by the user in the param in, ignoring characters
/// that are not digits, to the digits ADT of the param rhs.
/// </summary>
/// <param name="in">The in.</param>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// istream object
///</returns>
istream &operator >> (istream &in, LongInt &rhs)
{
	string integerChars;
	in >> integerChars;
	for (int i = 0; i < integerChars.length(); i++)
	{
		if (i == 0 && integerChars[0] == '-')
			rhs.negative = true;
		else if (isdigit(integerChars[i]))
			rhs.digits.addBack(integerChars[i]);
	}
	return in;
}

/// <summary>
/// Operator<< prints the LongInt specified by the param.
/// </summary>
/// <param name="out">The out.</param>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// ostream object containing the characters that represent the param
///</returns>
ostream &operator<<(ostream &out, const LongInt &rhs)
{
	LongInt output = rhs;
	output.remove0s();
	if (output.digits.isEmpty())
	{
		out << "0";
		return out;
	}
	if (output.negative)
	{
		if (!output.digits.isEmpty())
			out << "-";
		while (!output.digits.isEmpty())
			out << output.digits.removeFront();
	}
	else
	{
		while (!output.digits.isEmpty())
			out << output.digits.removeFront();
	}
	return out;
}

/// <summary>
/// Constructor
/// Initializes a new instance of the <see cref="LongInt" /> class.
/// with digits contained in the param
/// </summary>
/// <param name="longInteger">String representation of the LongInt.</param>
LongInt::LongInt(const string longInteger)
{
	int j = longInteger.length();
	string integerChars = longInteger;
	negative = false;
	for (int i = 0; i < j; i++)
	{
		if (i == 0 && longInteger[0] == '-')
			negative = true;
		else if (isdigit(integerChars[i]))
			digits.addBack(integerChars[i]);
	}
	remove0s();
}

/// <summary>
/// copy constructor
/// Initializes a new instance of the <see cref="LongInt" /> class.
/// that is equivalent to the param
/// </summary>
/// <param name="rhs">The RHS.</param>
LongInt::LongInt(const LongInt &rhs)
{
	negative = rhs.negative;
	digits = rhs.digits;
}

/// <summary>
/// Default constructor
/// Initializes a new instance of the <see cref="LongInt" /> class.
/// </summary>
LongInt::LongInt()
{
	negative = false;
}

// Destructor
/// <summary>
/// Finalizes an instance of the <see cref="LongInt" /> class.
/// </summary>
LongInt::~LongInt()
{
	digits.clear();
}

LongInt LongInt::operator/ (const LongInt &rhs) const
{
	LongInt quotient("0");
	LongInt zero("0");
	bool isNegative = false;
	LongInt one("1");
	LongInt numerator = *this;
	LongInt divisor = rhs;
	if (!numerator.negative && divisor.negative || numerator.negative && !divisor.negative)
		isNegative = true;
	numerator.negative = false;
	divisor.negative = false;
	if (divisor == zero) {
		//return 0 <-- hacky may have to think up something later
		return quotient;
	}
	while (numerator >= divisor)
	{
		quotient = quotient + one;
		numerator = numerator - divisor;
	}
	if (isNegative)
		quotient.negative = true;
	return quotient;
}

LongInt LongInt::operator* (const LongInt & rhs) const
{
	LongInt product("0");
	LongInt a = *this;
	LongInt b = rhs;
	LongInt one("1");
	LongInt zero("0");
	bool isNegative = false;
	if ((!a.negative && b.negative) || (a.negative && !b.negative))
		isNegative = true;
	a.negative = false;
	b.negative = false;
	//try to speed up the calculation...
	if (b > a) {
		LongInt temp = b;
		b = a;
		a = temp;
	}
	product = multiply(a, b);
	if (isNegative)
		product.negative = true;
	return product;
}

// Arithmetic binary operators
/// <summary>
/// Operator+ adds this LongInt to the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// A LongInt that is equivalent to this LongInt + the param
///</returns>
LongInt LongInt::operator+(const LongInt &rhs) const
{
	LongInt sum;
	Deque<char> addenedA = digits;
	Deque<char> addenedB = rhs.digits;
	int carry = 0;
	//if both integers are negative or neither are negative then 
	//ther is no change of sign in the sum
	if ((!negative && !rhs.negative) || (negative && rhs.negative))
	{
		//The length of the sum must be at least as long as the 
		//longest integer being summed.
		int minLength = (addenedA.size() <= addenedB.size()) ? addenedA.size() : addenedB.size();
		int ten = 10;
		for (int length = minLength; length > 0; length--)
		{
			int a = asciiCharToInt(addenedA.removeBack());
			int b = asciiCharToInt(addenedB.removeBack());
			int digitToAdd = (a + b + carry) % ten;
			digitToAdd = oneDigitIntToAsciiChar(digitToAdd);
			sum.digits.addFront(digitToAdd);
			carry = (a + b + carry) / ten;
		}
		while (!addenedA.isEmpty())
		{
			int currentAddened = asciiCharToInt(addenedA.removeBack()) + carry;
			carry = calcCarry(ten, currentAddened);
			sum = processAddened(currentAddened, sum);
		}
		while (!addenedB.isEmpty())
		{
			int currentAddened = asciiCharToInt(addenedB.removeBack()) + carry;
			carry = calcCarry(ten, currentAddened);
			sum = processAddened(currentAddened, sum);
		}
		if (carry > 0)
			//use ascii value for the char '1'
			sum.digits.addFront(49);
		if (negative && rhs.negative)
			sum.negative = true;
	}
	else if (!negative && rhs.negative)
	{
		LongInt minuend = *this;
		LongInt subtrahend = rhs;
		subtrahend.negative = false;
		sum = minuend - subtrahend;
		if (subtrahend > minuend)
			sum.negative = true;
	}
	else if (negative && !rhs.negative)
	{
		LongInt subtrahend = *this;
		subtrahend.negative = false;
		sum = rhs - subtrahend;
	}
	return sum;
}

/// <summary>
/// Operator- subtracts the specified RHS from this LongInt.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
///  A LongInt equivalent to this LongInt - the param
///</returns>
LongInt LongInt::operator-(const LongInt &rhs) const
{
	LongInt difference;
	Deque<char> minuend = digits;
	Deque<char> subtrahend = rhs.digits;
	int borrow = 0;
	int nextDigit = 0;

	if (!negative && !rhs.negative)
	{
		if (*this < rhs)
		{
			minuend = rhs.digits;
			subtrahend = digits;
			difference.negative = true;
		}
		while (minuend.getFront() == '0' && minuend.size() > 1) {
			minuend.removeFront();
		}
		while (subtrahend.getFront() == '0' && subtrahend.size() > 1) {
			subtrahend.removeFront();
		}
		for (int length = subtrahend.size(); length > 0; length--)
		{
			int a = asciiCharToInt(minuend.removeBack());
			int b = 0;
			if (!subtrahend.isEmpty()) {
				b = asciiCharToInt(subtrahend.removeBack());
			}
			nextDigit = a - b - borrow;
			borrow = 0;
			if (nextDigit < 0)
			{
				nextDigit = calcTest(minuend, nextDigit);
				difference.negative = isDiffNegative(minuend, nextDigit);
				borrow = calcBorrow(minuend, nextDigit);
			}
			nextDigit = oneDigitIntToAsciiChar(nextDigit);
			difference.digits.addFront(nextDigit);
		}
		while (!minuend.isEmpty())
		{
			int test = asciiCharToInt(minuend.getBack()) - borrow;
			borrow = 0;
			if (test < 0)
			{
				test = calcTest(minuend, test);
				difference.negative = isDiffNegative(minuend, test);
				borrow = calcBorrow(minuend, test);
			}
			test = oneDigitIntToAsciiChar(test);
			difference.digits.addFront(test);
			minuend.removeBack();
		}
	}
	else if (negative && rhs.negative)
	{
		LongInt longIntMinuend = rhs;
		longIntMinuend.negative = false;
		LongInt longIntSubtrahend = *this;
		longIntSubtrahend.negative = false;
		difference = longIntMinuend - longIntSubtrahend;
	}
	else if (!negative && rhs.negative)
	{
		LongInt addened = rhs;
		addened.negative = false;
		LongInt addendB = *this;
		difference = addened + addendB;
	}
	else if (negative && !rhs.negative)
	{
		LongInt addened = *this;
		addened.negative = false;
		difference = rhs + addened;
		difference.negative = true;
	}
	difference.remove0s();
	return difference;
}

// assignment operators
/// <summary>
/// Operator=s creates a deep copy of the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// A deep copy of the LongInt passed as a param
///</returns>
const LongInt& LongInt::operator=(const LongInt &rhs)
{
	digits = rhs.digits;
	negative = rhs.negative;
	return *this;
}

// Logical binary operators
/// <summary>
/// Operator< checks if this LongInt is less than the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// True if this LongInt is less than the param, false otherwise
///</returns>
bool LongInt::operator< (const LongInt & rhs) const
{
	LongInt leftSide = *this;
	LongInt rightSide = rhs;
	if (negative && !rhs.negative)
		return true;
	else if (!negative && rhs.negative)
		return false;
	else if (!negative && !rhs.negative)
	{
		leftSide.remove0s();
		rightSide.remove0s();
		if (leftSide.digits.size() != rightSide.digits.size())
			return (leftSide.digits.size() < rightSide.digits.size());
		else
		{
			LongInt tempLeft = leftSide;
			LongInt tempRight = rightSide;
			while (!tempLeft.digits.isEmpty())
			{
				const int left = tempLeft.digits.removeFront();
				const int right = tempRight.digits.removeFront();
				if (left != right)
					return (left < right);
			}
		}
	}
	else if (negative && rhs.negative)
	{
		if (digits.size() != rhs.digits.size())
			return (digits.size() > rhs.digits.size());
		else
		{
			LongInt tempLeft = *this;
			LongInt tempRight = rhs;
			while (!tempLeft.digits.isEmpty())
			{
				int left = tempLeft.digits.removeFront();
				int right = tempRight.digits.removeFront();
				if (left != right)
					return (left > right);
			}
		}
	}
	return false;
}

/// <summary>
/// Operator<= checks if this LongInt is less than or
///  equal to the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
///  true if this LongInt is less than or equal
/// to the param, false otherwise
///</returns>
bool LongInt::operator<=(const LongInt & rhs) const
{
	return (*this == rhs || *this < rhs);
}

/// <summary>
/// Operator> checks if this LongInt is greater than the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// true if this LongInt is greater than the param, false otherwise
///</returns>
bool LongInt::operator> (const LongInt & rhs) const
{
	if (negative && !rhs.negative)
		return false;
	else if (!negative && rhs.negative)
		return true;
	else if (!negative && !rhs.negative)
	{
		if (digits.size() != rhs.digits.size())
			return (digits.size() > rhs.digits.size());
		else
		{
			LongInt tempLeft = *this;
			LongInt tempRight = rhs;
			while (!tempLeft.digits.isEmpty())
			{
				int left = tempLeft.digits.removeFront();
				int right = tempRight.digits.removeFront();
				if (left != right)
					return (left > right);
			}
		}
	}
	else if (negative && rhs.negative)
	{
		if (digits.size() != rhs.digits.size())
			return (digits.size() < rhs.digits.size());
		else
		{
			LongInt tempLeft = *this;
			LongInt tempRight = rhs;
			while (!tempLeft.digits.isEmpty())
			{
				int left = tempLeft.digits.removeFront();
				int right = tempRight.digits.removeFront();
				if (left != right)
					return (left < right);
			}
		}
	}
	return false;
}

/// <summary>
/// Operator>= checks if this LongInt is greater than
/// or equal to the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// true if this LongInt is greater than or equal to
/// the param, false otherwise
///</returns>
bool LongInt::operator>=(const LongInt & rhs) const
{
	return (*this == rhs || *this > rhs);
}

/// <summary>
/// Operator== checks if this LongInt equals the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// true if this LongInt is equal to the param, false otherwise
///</returns>
bool LongInt::operator==(const LongInt & rhs) const
{
	if (negative && !rhs.negative)
		return false;
	else if (!negative && rhs.negative)
		return false;
	else if (!negative && !rhs.negative || negative && rhs.negative)
	{
		if (digits.size() != rhs.digits.size())
			return false;
		else
		{
			LongInt tempLeft = *this;
			LongInt tempRight = rhs;
			while (!tempLeft.digits.isEmpty())
			{
				int left = tempLeft.digits.getFront();
				int right = tempRight.digits.getFront();
				tempLeft.digits.removeFront();
				tempRight.digits.removeFront();
				if (left != right)
					return false;
			}
		}
	}
	return true;
}

/// <summary>
/// Operator!= checks if this LongInt is not equal to the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// true if this LongInt does not equal the param, false otherwise
///</returns>
bool LongInt::operator!=(const LongInt & rhs) const
{
	return !(*this == rhs);
}

/// <summary>
/// Remove0s removes any zeros that occur before
/// the first significant digit in this instance
/// </summary>
void LongInt::remove0s()
{
	while (!digits.isEmpty() && digits.getFront() == '0')
		digits.removeFront();
	if (digits.isEmpty())
	{
		digits.addFront(48);
		negative = false;
	}
}

/// <summary>
/// Converts asciiChar to integer, assuming the char is a digit 0-9.
/// </summary>
/// <param name="rhs">The char</param>
/// <returns>
/// integer value of char 0-9
///</returns>
int LongInt::asciiCharToInt(const char c) const
{
	//assume c is an ascii int
	char copy = c;
	return (int)copy - 48;
}

/// <summary>
/// Converts a digit between 0-9, to an ascii char.
/// </summary>
/// <param name="rhs">The int</param>
/// <returns>
/// char 0-9
///</returns>
char LongInt::oneDigitIntToAsciiChar(int a) const
{
	return a + 48;
}

LongInt LongInt::processAddened(int currentAddened, LongInt sum) const
{
	int ten = 10;
	if (currentAddened >= ten)
	{
		currentAddened -= ten;
		currentAddened = oneDigitIntToAsciiChar(currentAddened);
		sum.digits.addFront(currentAddened);
	}
	else
	{
		currentAddened = oneDigitIntToAsciiChar(currentAddened);
		sum.digits.addFront(currentAddened);
	}
	return sum;
}

int LongInt::calcCarry(int ten, int currentAddened) const
{
	if (currentAddened >= ten)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int LongInt::calcTest(Deque<char> minuend, int test) const
{
	if (minuend.isEmpty())
	{
		return -test;
	}
	else
	{
		return test += 10;
	}
}

int LongInt::calcBorrow(Deque<char> minuend, int test) const
{
	if (!minuend.isEmpty())
	{
		return 1;
	}
	return 0;
}

bool LongInt::isDiffNegative(Deque<char> minuend, int test) const
{
	if (minuend.isEmpty())
	{
		return true;
	}
	return false;
}

LongInt LongInt::multiply(LongInt a, LongInt b) const
{
	LongInt product("0");
	LongInt temp("0");
	LongInt zero("0");
	int originalSize = b.digits.size();
	for (int i = 0; i < originalSize; i++) {
		product = product + temp;
		temp = zero;
		int carry = 0;
		int currentDigit = asciiCharToInt(b.digits.removeBack());
		Deque<char> tempDigits = a.digits;
		for (int j = 0; j < a.digits.size(); j++) {
			if (j == 0) {
				for (int k = 0; k < i; k++) {
					if (k == 0)
						continue;
					else
						temp.digits.addBack('0');
				}
			}
			int productOfDigits = asciiCharToInt(tempDigits.removeBack()) * currentDigit;
			int digitToAdd = (productOfDigits + carry) % 10;
			carry = (productOfDigits + carry) / 10;
			if (i == 0 && j == 0) {
				temp.digits.addFront(oneDigitIntToAsciiChar(digitToAdd));
				temp.digits.removeBack();
			}
			else
				temp.digits.addFront(oneDigitIntToAsciiChar(digitToAdd));
		}
		temp.digits.addFront(oneDigitIntToAsciiChar(carry));
	}
	product = product + temp;
	return product;
}


