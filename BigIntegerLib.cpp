
#include "BigIntegerLib.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define UINT63_MAX		(UINT64_MAX >> 1)
#define UINTDEC_MAX	(uint64_t)1e19
#define UINTDECH_MAX	(uint64_t)(1e19 / 2)

#define MACRO(i, x)		((i >= (x)->data.size) ? 0 : (x)->data.number[i])
#ifndef MAX
#define MAX(x, y)	((x > y) ? x : y)
#endif

namespace BigIntegerLib
{
	BigInt::BigInt()
	{
		data.number = 0;
		data.sign = 1;
		data.size = 0;
	}

	BigInt::BigInt(const BigInt &number)
	{
		memcpy(&data, &number.data, sizeof(data));
		data.number = (uint64_t*)memcpy(malloc(number.data.size * sizeof(uint64_t)), number.data.number, number.data.size * sizeof(uint64_t));
	}

	BigInt::BigInt(string number)
	{
		data.number = 0;
		data.sign = 1;
		data.size = 0;

		convertNum(number);
	}

	BigInt::BigInt(uint64_t number)
	{
		data.number = (uint64_t*)malloc(sizeof(uint64_t));
		data.number[0] = number;
		data.sign = 1;
		data.size = 1;
	}

	BigInt::~BigInt()
	{
		free(data.number);
	}


	/*void BigInt::add(const BigInt &number)
	{
		if (number.data.sign < 0)
		{
			BigInt tmp(number);
			tmp.data.sign = 1;
			sub(tmp);
		}
		else
		{
			if (data.sign < 0)
			{
				data.sign = 1;
				sub(number);
				data.sign = -1;
				return;
			}

			int of = 0;		//Overflow
			bool c1, c2;
			size_t i;
			for (i = 0; i < number.data.size; i++)
			{
				c1 = c2 = false;
				if (i == data.size)
				{
					data.number = (uint64_t*)realloc(data.number, (++data.size) * sizeof(uint64_t));
					data.number[i] = 0;
				}

				int overflow = 0;

				data.number[i] += of;

				if (data.number[i] >= UINTDEC_MAX - number.data.number[i])
				{
					if ((c1 = (data.number[i] >= UINTDECH_MAX)))
						data.number[i] -= UINTDECH_MAX;
					if ((c2 = (number.data.number[i] >= UINTDECH_MAX)))
						number.data.number[i] -= UINTDECH_MAX;
					overflow = 1;
				}

				data.number[i] += number.data.number[i];

				if ((c1 || c2) && !(c1 && c2))
					data.number[i] -= UINTDECH_MAX;

				of = overflow;
			}
			if (of)
			{
				for (; i < data.size; i++)
				{
					int overflow = 0;
					c1 = false;
					if (data.number[i] >= UINTDEC_MAX - of)
					{
						if ((c1 = (data.number[i] >= UINTDECH_MAX)))
							data.number[i] -= UINTDECH_MAX;
						overflow = 1;
					}
					
					data.number[i] += of;

					if (c1)
						data.number[i] -= UINTDECH_MAX;

					of = overflow;
				}
				if (of)
				{
					data.number = (uint64_t*)realloc(data.number, (++data.size) * sizeof(uint64_t));
					data.number[data.size - 1] = of;
				}
			}
		}
	}*/

	void BigInt::add(const BigInt &number)
	{
		if(number.data.sign < 0)
		{
			BigInt tmp(number);
			tmp.data.sign = 1;
			sub(tmp);
		}
		else
		{
			if(data.sign < 0)
			{
				data.sign = 1;
				sub(number);
				data.sign = -1;
				return;
			}

			int overflow = 0;
			for(size_t i = 0; i < MAX(data.size, number.data.size); i++)
			{
				uint64_t tmp = MACRO(i, this) + MACRO(i, &number) + overflow;
				if(tmp - MACRO(i, this) - overflow != MACRO(i, &number)) overflow = 1;
				else overflow = 0;
				if (i == data.size) data.number = (uint64_t*)realloc(data.number, ++data.size * sizeof(uint64_t));
				this->data.number[i] = tmp;
			}

			size_t j = 0;
			while(overflow)
			{
				int nextOverflow = 0;
				if(j == data.size) data.number = (uint64_t*)realloc(data.number, ++data.size * sizeof(uint64_t));
				if(data.number[j] + overflow < data.number[j]) nextOverflow = 1;
				data.number[j] += overflow;
				j++;
				overflow = nextOverflow;
			}
		}
	}


	void BigInt::sub(const BigInt &number)
	{
		if (number.data.sign < 0)
		{
			BigInt tmp(number);
			tmp.data.sign = 1;
			add(tmp);
		}
		else
		{
			if (data.sign < 0)
			{
				data.sign = 1;
				add(number);
				data.sign = -1;
				return;
			}

			int of = 0;		//Overflow
			//bool c1, c2;
			for (size_t i = 0; i < number.data.size; i++)
			{
				//c1 = c2 = false;
				if (i == data.size)
				{
					data.number = (uint64_t*)realloc(data.number, (++data.size) * sizeof(uint64_t));
					data.number[i] = 0;
				}

				int overflow = 0;

				data.number[i] -= of;

				if (number.data.number[i] > data.number[i])
				{
					uint64_t diff = number.data.number[i] - data.number[i];
					data.number[i] = diff;
					overflow = 1;
				}

				/*if (data.number[i] >= UINTDEC18_MAX - number->data.number[i])
				{
					if (c1 = (data.number[i] >= UINTDEC18H_MAX))
						data.number[i] -= UINTDEC18H_MAX;
					if (c2 = (number->data.number[i] >= UINTDEC18H_MAX))
						number->data.number[i] -= UINTDEC18H_MAX;
					overflow = 1;
				}*/
				else
					data.number[i] -= number.data.number[i];

				/*if ((c1 || c2) && !(c1 && c2))
					data.number[i] -= UINTDEC18H_MAX;*/

				of = overflow;
			}
			if (of)
			{
				data.sign *= -1;
			}

			//Free the memory which we don't use
			size_t oldSize = data.size;
			for (size_t i = data.size; i > 0; i--)
			{
				if (data.number[i - 1] == 0)
					data.size--;
			}
			if (oldSize != data.size)
				data.number = (uint64_t*)realloc(data.number, data.size);
		}
	}


	void BigInt::mul(const BigInt &number)
	{

		//Make a copy
		BigInt tmp(*this);

		//Reset local
		for (size_t i = 0; i < data.size; i++)
			data.number[i] = 0;
		
		//Set sign
		data.sign = tmp.data.sign * number.data.sign;

		//Multiply each block with each other
		for (size_t i = 0; i < tmp.data.size; i++)
		{
			for (size_t j = 0; j < number.data.size; j++)
			{
				uint64_t z, overflow;
				if (i + j >= data.size)
				{
					data.number = (uint64_t*)realloc(data.number, ++data.size * sizeof(uint64_t));
					data.number[data.size - 1] = 0;
				}
				z = _umul128(tmp.data.number[i], number.data.number[j], &overflow);
				if (data.number[i + j] > UINT64_MAX - z)
				{
					overflow++;
				}
				data.number[i + j] += z;
				if (MACRO(i + j + 1, this) > UINT64_MAX - overflow)
				{
					if (i + j + 2 >= data.size)
					{
						data.number = (uint64_t*)realloc(data.number, ++data.size * sizeof(uint64_t));
						data.number[data.size - 1] = 0;
					}
					data.number[i + j + 2]++;
				}
				if (i + j + 1 >= data.size && overflow)
				{
					data.number = (uint64_t*)realloc(data.number, ++data.size * sizeof(uint64_t));
					data.number[data.size - 1] = 0;
				}
				data.number[i + j + 1] += overflow;
			}
		}
	}


	void BigInt::div(const BigInt &number)
	{
		//Mul sign
		data.sign *= number.data.sign;
		uint64_t overflow = 0;
		size_t i;
		for (i = MAX(data.size, number.data.size); i > 0; i--)
		{
			if (MACRO(i - 1, &number) != 0)
			{
				uint64_t tmp = MACRO(i - 1, this) / MACRO(i - 1, &number);
				overflow = MACRO(i - 1, this) % MACRO(i - 1, &number);
			}
			else
			{

			}
		}
	}

	void BigInt::mod(const BigInt &number)
	{

	}


	string BigInt::toStr()
	{
		string number;

		for (size_t i = 0; i < data.size; i++)
		{
			size_t nc = 0;
			for (BigInt tmp(*this); tmp.data.size == 0 || (tmp.data.size == 1 && tmp.data.number[0] == 0); tmp /= (uint64_t)10, nc++)
			{
				uint8_t n = (uint8_t)(tmp % 10).toUInt();
				char c = n + '0';
				number.insert(0, 1, c);
			}
			if (i != data.size - 1)
				number.insert(0, 19 - nc, '0');
		}

		if (number.length() == 0)
			number.insert(0, 1, '0');

		if (data.sign < 0 && data.size != 0 && !(data.size == 1 && data.number[0] == 0))
			number.insert(0, 1, '-');
		
		return number;
	}

	int64_t BigInt::toInt()
	{
		if (data.size > 0)
			return (int64_t)data.number[0] * data.sign;
		else
			return 0;
	}

	uint64_t BigInt::toUInt()
	{
		if (data.size > 0)
			return (uint64_t)data.number[0];
		else
			return 0;
	}


	//===========OPERATOREN=================================
	BigInt& BigInt::operator=(string number)
	{
		convertNum(number);
		return *this;
	}
	BigInt& BigInt::operator=(BigInt number)
	{
		memcpy(&data, &number.data, sizeof(data));
		data.number = (uint64_t*)memcpy(malloc(number.data.size * sizeof(uint64_t)), number.data.number, number.data.size * sizeof(uint64_t));
		return *this;
	}

	BigInt BigInt::operator+(string &number)
	{
		BigInt tmp(*this);
		tmp += number;
		return tmp;
	}
	BigInt BigInt::operator+(BigInt &number)
	{
		BigInt tmp(*this);
		tmp += number;
		return tmp;
	}
	BigInt BigInt::operator+(uint64_t number)
	{
		BigInt tmp(*this);
		tmp += number;
		return tmp;
	}

	BigInt& BigInt::operator+=(string &number)
	{
		BigInt tmp(number);
		add(tmp);
		return *this;
	}
	BigInt& BigInt::operator+=(BigInt &number)
	{
		add(number);
		return *this;
	}
	BigInt& BigInt::operator+=(uint64_t number)
	{
		BigInt tmp(number);
		add(tmp);
		return *this;
	}

	BigInt BigInt::operator-(string &number)
	{
		BigInt tmp(*this);
		tmp -= number;
		return tmp;
	}
	BigInt BigInt::operator-(BigInt &number)
	{
		BigInt tmp(*this);
		tmp -= number;
		return tmp;
	}
	BigInt BigInt::operator-(uint64_t number)
	{
		BigInt tmp(*this);
		tmp -= number;
		return tmp;
	}

	BigInt& BigInt::operator-=(string &number)
	{
		BigInt tmp(number);
		sub(tmp);
		return *this;
	}
	BigInt& BigInt::operator-=(BigInt &number)
	{
		sub(number);
		return *this;
	}
	BigInt& BigInt::operator-=(uint64_t number)
	{
		BigInt tmp(number);
		sub(tmp);
		return *this;
	}

	BigInt BigInt::operator*(string &number)
	{
		BigInt tmp(*this);
		tmp *= number;
		return tmp;
	}
	BigInt BigInt::operator*(BigInt &number)
	{
		BigInt tmp(*this);
		tmp *= number;
		return tmp;
	}
	BigInt BigInt::operator*(uint64_t number)
	{
		BigInt tmp(*this);
		tmp *= number;
		return tmp;
	}

	BigInt& BigInt::operator*=(string &number)
	{
		BigInt tmp(number);
		mul(tmp);
		return *this;
	}
	BigInt& BigInt::operator*=(BigInt &number)
	{
		mul(number);
		return *this;
	}
	BigInt& BigInt::operator*=(uint64_t number)
	{
		BigInt tmp(number);
		mul(tmp);
		return *this;
	}

	BigInt BigInt::operator/(string &number)
	{
		BigInt tmp(*this);
		tmp /= number;
		return tmp;
	}
	BigInt BigInt::operator/(BigInt &number)
	{
		BigInt tmp(*this);
		tmp /= number;
		return tmp;
	}
	BigInt BigInt::operator/(uint64_t number)
	{
		BigInt tmp(*this);
		tmp /= number;
		return tmp;
	}

	BigInt& BigInt::operator/=(string &number)
	{
		BigInt tmp(number);
		div(tmp);
		return *this;
	}
	BigInt& BigInt::operator/=(BigInt &number)
	{
		div(number);
		return *this;
	}
	BigInt& BigInt::operator/=(uint64_t number)
	{
		BigInt tmp(number);
		div(tmp);
		return *this;
	}

	BigInt BigInt::operator%(string &number)
	{
		BigInt tmp(*this);
		tmp %= number;
		return tmp;
	}
	BigInt BigInt::operator%(BigInt &number)
	{
		BigInt tmp(*this);
		tmp %= number;
		return tmp;
	}
	BigInt BigInt::operator%(uint64_t number)
	{
		BigInt tmp(*this);
		tmp %= number;
		return tmp;
	}

	BigInt& BigInt::operator%=(string &number)
	{
		BigInt tmp(number);
		mod(tmp);
		return *this;
	}
	BigInt& BigInt::operator%=(BigInt &number)
	{
		mod(number);
		return *this;
	}
	BigInt& BigInt::operator%=(uint64_t number)
	{
		BigInt tmp(number);
		mod(tmp);
		return *this;
	}

	bool BigInt::operator==(BigInt &number)
	{
		if (data.sign != number.data.sign)
			return false;
		if (data.size != data.size)
			return false;

		for (size_t i = 0; i < data.size; i++)
		{
			if (data.number[i] != number.data.number[i])
				return false;
		}
		return true;
	}
	bool BigInt::operator==(string &number)
	{
		BigInt tmp(number);

		return *this == tmp;
	}
	bool BigInt::operator==(uint64_t number)
	{
		BigInt tmp(number);

		return *this == tmp;
	}

	bool BigInt::operator!=(BigInt &number)
	{
		return !(*this == number);
	}
	bool BigInt::operator!=(string &number)
	{
		BigInt tmp(number);

		return *this != tmp;
	}
	bool BigInt::operator!=(uint64_t number)
	{
		BigInt tmp(number);

		return *this != tmp;
	}

	bool BigInt::operator<(BigInt &number)
	{
		//Vorzeichen überprüfen
		if (data.sign < number.data.sign)
			return true;
		//Grösse überprüfen
		if (data.size < number.data.size)
			return true;
		if (data.size > number.data.size)
			return false;
		
		for (size_t i = data.size; i > 0; i--)
		{
			if (data.number[i - 1] < number.data.number[i - 1])
				return true;
		}

		return false;
	}
	bool BigInt::operator<(string &number)
	{
		BigInt tmp(number);
		return (*this < tmp);
	}
	bool BigInt::operator<(uint64_t number)
	{
		BigInt tmp(number);
		return (*this < tmp);
	}

	bool BigInt::operator<=(BigInt &number)
	{
		if (*this == number)
			return true;
		return (*this < number);
	}
	bool BigInt::operator<=(string &number)
	{
		BigInt tmp(number);
		return (*this <= tmp);
	}
	bool BigInt::operator<=(uint64_t number)
	{
		BigInt tmp(number);
		return (*this <= tmp);
	}

	bool BigInt::operator>(BigInt &number)
	{
		//Vorzeichen überprüfen
		if (data.sign > number.data.sign)
			return true;
		//Grösse überprüfen
		if (data.size > number.data.size)
			return true;
		if (data.size < number.data.size)
			return false;

		for (size_t i = data.size; i > 0; i--)
		{
			if (data.number[i - 1] > number.data.number[i - 1])
				return true;
		}

		return false;
	}
	bool BigInt::operator>(string &number)
	{
		BigInt tmp(number);
		return (*this > tmp);
	}
	bool BigInt::operator>(uint64_t number)
	{
		BigInt tmp(number);
		return (*this > tmp);
	}

	bool BigInt::operator>=(BigInt &number)
	{
		if (*this == number)
			return true;
		return (*this > number);
	}
	bool BigInt::operator>=(string &number)
	{
		BigInt tmp(number);
		return (*this >= tmp);
	}
	bool BigInt::operator>=(uint64_t number)
	{
		BigInt tmp(number);
		return (*this >= tmp);
	}
	//================ENDE OPERATOREN================================

	/*void BigInt::setNum(string number)
	{
		uint64_t pos = 1;

		for (size_t i = number.size(); i != 0; i--)
		{
			char c = number[i - 1];
			if (i == 1 && c == '-')
				data.sign = -1;
			else if (c >= '0' && c <= '9')
			{
				uint8_t n = c - '0';
				if (pos == 1)
				{
					data.number = (uint64_t*)realloc(data.number, (++data.size) * sizeof(uint64_t));
					data.number[data.size - 1] = 0;
				}

				data.number[data.size - 1] += n * pos;
				if (pos >= 1e18)
					pos = 1;
				else
					pos *= 10;
			}
		}
	}*/

	void BigInt::convertNum(string number)
	{
		size_t i;
		//Reset
		free(data.number);
		data.size = 0;
		data.sign = 1;
		for (i = 0; i < number.length(); i++)
		{
			if(number[i] != '-' && (number[i] > '9' || number[i] < '0')) continue;
			if(i == 0 && number[i] == '-') data.sign = -1;
			else if (number[i] != '-')
			{
				*this *= (uint64_t)10;
				*this += number[i] - '0';
			}
		}
	}
}
