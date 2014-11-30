
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

		setNum(number);
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
				uint64_t tmp;
				if(i == data.size - 1) data.number = (uint64_t*)realloc(data.number, ++data.size + sizeof(uint64_t));
				tmp = MACRO(i, this) + MACRO(i, &number) + overflow;
				if(tmp - MACRO(i, this) - overflow != MACRO(i, &number)) overflow = 1;
				else overflow = 0;
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
		//Multiply sign
		data.sign *= number.data.sign;

		uint64_t overflow = 0;
		size_t i;
		for (i = 0; i < number.data.size; i++)
		{
			if (i >= data.size)
			{
				data.number = (uint64_t*)realloc(data.number, (++data.size) * sizeof(uint64_t));
				data.number[i] = 0;
			}
			data.number[i] += overflow;
			data.number[i] = _umul128(data.number[i], number.data.number[i], &overflow);
		}
		if (overflow)
		{
			if ( i < data.size)
			{
				data.number[i] += i;
				overflow = 0;
			}
			if (overflow)
			{
				data.number = (uint64_t*)realloc(data.number, (++data.size) * sizeof(uint64_t));
				data.number[i] = overflow;
			}
		}
	}


	void BigInt::div(const BigInt &number)
	{
		//Mul sign
		data.sign *= number.data.sign;
	}


	string BigInt::toStr()
	{
		string number;

		for (size_t i = 0; i < data.size; i++)
		{
			size_t nc = 0;
			for (uint64_t tmp = data.number[i]; tmp != 0; tmp /= 10, nc++)
			{
				uint8_t n = tmp % 10;
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


	BigInt& BigInt::operator=(string number)
	{
		setNum(number);
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


	void BigInt::setNum(string number)
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
	}

	void BigInt::convertNum(string number)
	{
		size_t i;
		for(i = number.length(); i > 0; i++)
		{
			if(number[i - 1] != '-' && (number[i - 1] > '9' || number[i - 1] < '0')) continue;
			if(i == 1 && number[i - 1] == '-') data.sign = -1;
			else if (number[i - 1] != '-')
			{
				*this * (uint64_t)10 + number[i - 1];
			}
		}
	}
}
