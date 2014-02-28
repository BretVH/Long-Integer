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
istream &operator>>( istream &in, LongInt &rhs )
{
	string b;
	in >> b;
	int j = b.length();
	for( int i = 0; i < j; i++ )
	{
		if( i == 0 && b[0] == '-' )
			rhs.negative = true;
		else if( isdigit(b[i]) )
			rhs.digits.addBack(b[i]);
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
ostream &operator<<( ostream &out, const LongInt &rhs )
{
	LongInt newLongInt = rhs;
	newLongInt.remove0s();
	if( newLongInt.digits.isEmpty() )
	{
		out << "0";
		return out;
	}
	if( newLongInt.negative )
	{
		if( !newLongInt.digits.isEmpty() )	
			out << "-";
		while( !newLongInt.digits.isEmpty() )
			out << newLongInt.digits.removeFront();
	}
	else
	{
		while( !newLongInt.digits.isEmpty() )
			out << newLongInt.digits.removeFront();
	}
	return out;
}

/// <summary>
/// Constructor
/// Initializes a new instance of the <see cref="LongInt" /> class.
/// with digits contained in the param
/// </summary>
/// <param name="str">The STR.</param>
LongInt::LongInt( const string str )
{
	int j = str.length();
	string mystr = str;
	negative = false;
	for( int i = 0; i < j; i++ )
	{
		if( i == 0 && str[0] == '-' )
			negative = true;
		else if( isdigit( mystr[i] ) )
			digits.addBack( mystr[i] );
	}
}

/// <summary>
/// copy constructor
/// Initializes a new instance of the <see cref="LongInt" /> class.
/// that is equivalent to the param
/// </summary>
/// <param name="rhs">The RHS.</param>
LongInt::LongInt( const LongInt &rhs )
{
	negative = rhs.negative;
	digits = rhs.digits;
}

/// <summary>
/// Default constructor
/// Initializes a new instance of the <see cref="LongInt" /> class.
/// </summary>
LongInt::LongInt( )
{
	negative = false;
}

// Destructor
/// <summary>
/// Finalizes an instance of the <see cref="LongInt" /> class.
/// </summary>
LongInt::~LongInt( )
{
	digits.clear();
}

LongInt LongInt::operator/ ( const LongInt &rhs ) const
{
	LongInt newLongInt("0");
	LongInt reset("0");
	bool myNeg = false;
	LongInt sub("1");
	LongInt lhsInt = *this;
	LongInt rhsDInt = rhs;
	LongInt intermediate;
	if ( !lhsInt.negative && rhsDInt.negative || lhsInt.negative && !rhsDInt.negative )
		myNeg = true;
	lhsInt.negative = false;
	rhsDInt.negative = false;
	while ( lhsInt - rhsDInt >= reset )
	{
		newLongInt = newLongInt + sub;
		lhsInt = lhsInt - rhsDInt;
	}
	if ( myNeg )
		newLongInt.negative = true;
	return newLongInt; 
}

LongInt LongInt::operator* (const LongInt & rhs ) const
{
	LongInt newLongInt("0");
	LongInt intermediateLongInt("0");
	LongInt reset("1");
       LongInt lhs = *this;
	LongInt rhsD = rhs;
	LongInt add("1");
	bool myNeg = false;
	if ( (!lhs.negative && rhsD.negative) || (lhs.negative && !rhsD.negative) )
		myNeg = true;
	lhs.negative = false;
	rhsD.negative = false;
	while( rhsD > reset )
	{
		newLongInt = newLongInt + lhs;
		rhsD = rhsD - add;
		//cout << newLongInt << "newlongint" << endl;
		//cout << rhsD << "rhsD" << endl;
		//cout << (rhsD > reset) << endl;
	}
       if ( myNeg )
		newLongInt.negative = true;
	return newLongInt; 
}

// Arithmetic binary operators
/// <summary>
/// Operator+ adds this LongInt to the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// A LongInt that is equivalent to this LongInt + the param
///</returns>
LongInt LongInt::operator+( const LongInt &rhs ) const
{
	LongInt newLongInt;
	Deque<char> lhs = digits;
	Deque<char> rhsD = rhs.digits;
	int carry = 0;
	int minLength = 
		( digits.size( ) <= rhs.digits.size( ) ) ? digits.size( ) : rhs.digits.size( );

	if( ( !negative && !rhs.negative ) || ( negative && rhs.negative ) )
	{
		for ( int length = minLength; length > 0; length-- )
		{
			newLongInt.digits.addFront( ( ( ( lhs.getBack() - 48 ) + ( rhsD.getBack() - 48 ) + carry ) % 10 ) + 48 );
			carry = ( ( lhs.removeBack() - 48 ) + ( rhsD.removeBack() - 48 ) ) / 10;
		}
		while( !lhs.isEmpty() )
		{
			int newer = ( ( ( lhs.getBack() - 48 ) + carry) );
			if( newer >= 10 )
			{
				carry = 1;
				newLongInt.digits.addFront( newer + 38 );
				lhs.removeBack();
			}
			else 
			{
				newLongInt.digits.addFront( ( ( lhs.removeBack() - 48 ) + carry ) + 48 );
				carry = 0;
			}		
		}
		while( !rhsD.isEmpty() )
		{
			int newer = ( ( rhsD.getBack() - 48 ) + carry );
			if( newer >=10 )
			{
				carry = 1;
				newLongInt.digits.addFront( newer + 38 );
				rhsD.removeBack();
			}
			else 
			{
				newLongInt.digits.addFront( ( ( rhsD.removeBack() - 48 ) + carry )  + 48 );
				carry = 0;
			}
		}
		if( carry > 0 )
			newLongInt.digits.addFront( 49 );
		if( negative && rhs.negative )
			newLongInt.negative = true;
	}
	else if( !negative && rhs.negative )
	{
		LongInt anotherLongInt = *this;
		newLongInt = rhs;
		newLongInt.negative = false;
		LongInt thirdLongInt = anotherLongInt - newLongInt;
		if( newLongInt > anotherLongInt )
			thirdLongInt.negative = true;
		newLongInt = thirdLongInt;
	}
	else if( negative && !rhs.negative )
	{
		newLongInt = *this;
		newLongInt.negative = false;
		newLongInt = rhs - newLongInt;
	}
	return newLongInt;
}

/// <summary>
/// Operator- subtracts the specified RHS from this LongInt.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
///  A LongInt equivalent to this LongInt - the param
///</returns>
LongInt LongInt::operator-( const LongInt &rhs ) const
{
	LongInt newLongInt;
	Deque<char> lhs = digits;
	Deque<char> rhsD = rhs.digits;
	int borrow = 0;
	int nextDigit = 0;
	int minLength = 
		( digits.size( ) <= rhs.digits.size( ) ) ? digits.size( ) : rhs.digits.size( );

	if( !negative && !rhs.negative )
	{
		if( *this < rhs )
		{
			lhs = rhs.digits;
			rhsD = digits;
			newLongInt.negative = true;
		}
		for ( int length = minLength; length > 0; length-- )
		{
			nextDigit = ( (lhs.removeBack() - 48 ) - ( rhsD.removeBack() - 48 ) - borrow );
			borrow = 0;
			if( nextDigit < 0 )
			{
				if( lhs.isEmpty() || lhs.getBack() == 0 )
				{
					nextDigit = -nextDigit;
					newLongInt.negative = true;
				}
				else
				{
					nextDigit += 10;
					borrow = 1;
				}
			}
			newLongInt.digits.addFront( nextDigit + 48 );
		}
		while( !lhs.isEmpty() )
		{
			if( (lhs.getBack() - borrow) < 0 )
			{
				borrow = 1; 
				newLongInt.digits.addFront( 48 );
				lhs.removeBack();
			}
			else
			{
				newLongInt.digits.addFront( (lhs.removeBack() - borrow) );
				borrow = 0;
			}
		}
		while( !rhsD.isEmpty() )
		{
			if(( rhsD.getBack() + borrow ) >= 10 )
			{
				borrow = 1; 
				newLongInt.digits.addFront( 48 );
				rhsD.removeBack();
			}
			else
			{	
				newLongInt.digits.addFront( ( rhsD.removeBack() + borrow ) );
				borrow = 0;
			}
			newLongInt.negative = true;
		}
		if( borrow >= 1 )
		{
			newLongInt.digits.addFront( 49 );
		}
	}
	else if( negative && rhs.negative )
	{
		LongInt anotherLongInt = rhs;
		anotherLongInt.negative = false;
		newLongInt = *this;
		newLongInt.negative = false;
		newLongInt = anotherLongInt - newLongInt;
	}
	else if( !negative && rhs.negative )
	{
		newLongInt = rhs;
		LongInt anotherLongInt = *this;
		newLongInt.negative = false;
		newLongInt = newLongInt + anotherLongInt;
	}
	else if( negative && !rhs.negative )
	{
		newLongInt = *this;
		newLongInt.negative = false;
		newLongInt = rhs + newLongInt;
		newLongInt.negative = true;
	}
	return newLongInt;
}


// assignment operators
/// <summary>
/// Operator=s creates a deep copy of the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// A deep copy of the LongInt passed as a param
///</returns>
const LongInt& LongInt::operator=( const LongInt &rhs )
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
bool LongInt::operator< ( const LongInt & rhs ) const
{
	LongInt newLongInt = *this;
	LongInt anotherLongInt = rhs;
	if( negative && !rhs.negative )
		return true;
	else if( !negative && rhs.negative )
		return false;
	else if( !negative && !rhs.negative )
	{
		newLongInt.remove0s();
		anotherLongInt.remove0s();
		if( newLongInt.digits.size() != anotherLongInt.digits.size() )
			return ( newLongInt.digits.size() < anotherLongInt.digits.size() );
		else
		{
			LongInt templ = newLongInt;
			LongInt tempr = anotherLongInt;
			while( !templ.digits.isEmpty() )
			{
				const int left = templ.digits.removeFront();
				const int right = tempr.digits.removeFront();
				if( left != right )
					return ( left < right );
			}
		}
	}
	else if( negative && rhs.negative )
	{
		if( digits.size() != rhs.digits.size() )
			return ( digits.size() > rhs.digits.size() );
		else
		{
			LongInt templ = *this;
			LongInt tempr = rhs;
			while( !templ.digits.isEmpty() )
			{
				int left = templ.digits.removeFront();
				int right = tempr.digits.removeFront();
				if( left != right )
					return ( left > right );
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
bool LongInt::operator<=( const LongInt & rhs ) const
{
	return ( *this == rhs || *this < rhs );
}

/// <summary>
/// Operator> checks if this LongInt is greater than the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// true if this LongInt is greater than the param, false otherwise
///</returns>
bool LongInt::operator> ( const LongInt & rhs ) const
{
	if( negative && !rhs.negative )
		return false;
	else if( !negative && rhs.negative )
		return true;
	else if( !negative && !rhs.negative )
	{
		if( digits.size() != rhs.digits.size() )
			return ( digits.size() > rhs.digits.size() );
		else
		{
			LongInt templ = *this;
			LongInt tempr = rhs;
			while( !templ.digits.isEmpty() )
			{
				int left = templ.digits.removeFront();
				int right = tempr.digits.removeFront();
				if( left != right )
					return ( left > right );
			}
		}
	}
	else if( negative && rhs.negative )
	{
		if( digits.size() != rhs.digits.size() )
			return ( digits.size() < rhs.digits.size() );
		else
		{
			LongInt templ = *this;
			LongInt tempr = rhs;
			while( !templ.digits.isEmpty() )
			{
				int left = templ.digits.removeFront();
				int right = tempr.digits.removeFront();
				if( left != right )
					return ( left < right );
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
bool LongInt::operator>=( const LongInt & rhs ) const
{
	return ( *this == rhs || *this > rhs );
}

/// <summary>
/// Operator== checks if this LongInt equals the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>
/// true if this LongInt is equal to the param, false otherwise
///</returns>
bool LongInt::operator==( const LongInt & rhs ) const
{
	if( negative && !rhs.negative )
		return false;
	else if( !negative && rhs.negative )
		return false;
	else if( !negative && !rhs.negative || negative && rhs.negative )
	{
		if( digits.size() != rhs.digits.size() )
			return false;
		else
		{
			LongInt templ = *this;
			LongInt tempr = rhs;
			while( !templ.digits.isEmpty() )
			{
				int left = templ.digits.getFront();
				int right = tempr.digits.getFront();
				templ.digits.removeFront();
				tempr.digits.removeFront();
				if( left != right )
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
bool LongInt::operator!=( const LongInt & rhs ) const
{
	return !( *this == rhs );
}

/// <summary>
/// Remove0s removes any zeros that occur before
/// the first significant digit in this instance
/// </summary>
void LongInt::remove0s()
{
	while( !digits.isEmpty() && digits.getFront() == '0' )
		digits.removeFront();
	if( digits.isEmpty() )
{
	digits.addFront(48);
	negative = false;
}
}