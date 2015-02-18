

#include <iostream>
#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#define prepend	__declspec(dllexport)
#else
#define prepend
#endif

#ifdef __cplusplus
extern "C"
{
#endif

using namespace std;

namespace BigIntegerLib
{
	class BigInt
	{
	public:
		prepend BigInt();
		prepend BigInt(const BigInt&);
		prepend BigInt(string);
		prepend BigInt(uint64_t);
		prepend ~BigInt();

		prepend void add(const BigInt&);
		prepend void sub(const BigInt&);
		prepend void mul(const BigInt&);
		prepend void div(const BigInt&);
		prepend void mod(const BigInt&);

		prepend string toStr();
		prepend int64_t toInt();
		prepend uint64_t toUInt();

		//Operators
		prepend BigInt& operator=(string);
		prepend BigInt& operator=(BigInt);

		prepend BigInt operator+(string&);
		prepend BigInt operator+(BigInt&);
		prepend BigInt operator+(uint64_t);
		prepend BigInt& operator+=(string&);
		prepend BigInt& operator+=(BigInt&);
		prepend BigInt& operator+=(uint64_t);

		prepend BigInt operator-(string&);
		prepend BigInt operator-(BigInt&);
		prepend BigInt operator-(uint64_t);
		prepend BigInt& operator-=(string&);
		prepend BigInt& operator-=(BigInt&);
		prepend BigInt& operator-=(uint64_t);

		prepend BigInt operator*(string&);
		prepend BigInt operator*(BigInt&);
		prepend BigInt operator*(uint64_t);
		prepend BigInt& operator*=(string&);
		prepend BigInt& operator*=(BigInt&);
		prepend BigInt& operator*=(uint64_t);

		prepend BigInt operator/(string&);
		prepend BigInt operator/(BigInt&);
		prepend BigInt operator/(uint64_t);
		prepend BigInt& operator/=(string&);
		prepend BigInt& operator/=(BigInt&);
		prepend BigInt& operator/=(uint64_t);

		prepend BigInt operator%(string&);
		prepend BigInt operator%(BigInt&);
		prepend BigInt operator%(uint64_t);
		prepend BigInt& operator%=(string&);
		prepend BigInt& operator%=(BigInt&);
		prepend BigInt& operator%=(uint64_t);

		prepend BigInt operator<<(BigInt&);
		prepend BigInt& operator<<=(BigInt&);
		prepend BigInt operator>>(BigInt&);
		prepend BigInt& operator>>=(BigInt&);

		prepend bool operator==(BigInt&);
		prepend bool operator==(string&);
		prepend bool operator==(uint64_t);
		prepend bool operator!=(BigInt&);
		prepend bool operator!=(string&);
		prepend bool operator!=(uint64_t);

		prepend bool operator<(BigInt&);
		prepend bool operator<(string&);
		prepend bool operator<(uint64_t);

		prepend bool operator<=(BigInt&);
		prepend bool operator<=(string&);
		prepend bool operator<=(uint64_t);

		prepend bool operator>(BigInt&);
		prepend bool operator>(string&);
		prepend bool operator>(uint64_t);

		prepend bool operator>=(BigInt&);
		prepend bool operator>=(string&);
		prepend bool operator>=(uint64_t);

	private:
		void setNum(string);
		void convertNum(string);
		void divHelp(BigInt&, BigInt&, BigInt&, BigInt&);

		struct{
			int8_t sign;
			uint64_t *number;
			size_t size;
		} data;
	};
}

#ifdef __cplusplus
}
#endif
