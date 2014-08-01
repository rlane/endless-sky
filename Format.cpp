/* Format.h
Copyright (c) 2014 by Michael Zahniser

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "Format.h"

#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;



string Format::Number(double value)
{
	if(!value)
		return "0";
	
	int power = floor(log10(fabs(value)));
	if(power >= 15 || power <= -5)
	{
		// Fall back to scientific notation if the number is outside the range
		// we can format "nicely".
		ostringstream out;
		out.precision(3);
		out << value;
		return out.str();
	}
	
	string result;
	result.reserve(8);
	
	bool isNegative = (value < 0.);
	bool nonzero = false;
	
	if(power >= 6)
	{
		nonzero = true;
		int place = (power - 6) / 3;
		
		static const char suffix[3] = {'M', 'B', 'T'};
		static const double multiplier[3] = {1e-6, 1e-9, 1e-12};
		result += suffix[place];
		value *= multiplier[place];
		power %= 3;
	}
	
	// The number of digits to the left of the decimal is max(0, power + 1).
	// e.g. if power = 0, 10 > value >= 1.
	int left = max(0, power + 1);
	int right = max(0, 5 - left);
	if(nonzero)
		right = min(right, 3);
	nonzero |= !right;
	int rounded = round(fabs(value) * pow(10., right));
	
	while(rounded | right)
	{
		int digit = rounded % 10;
		if(nonzero | digit)
		{
			result += digit + '0';
			nonzero = true;
		}
		rounded /= 10;
		
		if(right)
		{
			--right;
			if(!right)
			{
				if(nonzero)
					result += '.';
				nonzero = true;
			}
		}
	}
	
	// Add the negative sign if needed.
	if(isNegative)
		result += '-';
	
	// Reverse the string.
	reverse(result.begin(), result.end());
	
	return result;
}