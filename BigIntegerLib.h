
#include <iostream>
#include <stdint.h>

#ifdef win32
#define dllexport	dllexport
#else
#define dllexport
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
		dllexport BigInt();
		dllexport BigInt(const BigInt&);
		dllexport BigInt(string);
		dllexport BigInt(uint64_t);
		dllexport ~BigInt();

		dllexport void add(const BigInt&);
		dllexport void sub(const BigInt&);
		dllexport void mul(const BigInt&);
		dllexport void div(const BigInt&);

		dllexport string toStr();

		//Operators
		dllexport BigInt& operator=(string);
		dllexport BigInt& operator=(BigInt);

		dllexport BigInt operator+(string&);
		dllexport BigInt operator+(BigInt&);
		dllexport BigInt operator+(uint64_t);
		dllexport BigInt& operator+=(string&);
		dllexport BigInt& operator+=(BigInt&);
		dllexport BigInt& operator+=(uint64_t);

		dllexport BigInt operator-(string&);
		dllexport BigInt operator-(BigInt&);
		dllexport BigInt operator-(uint64_t&);
		dllexport BigInt& operator-=(string&);
		dllexport BigInt& operator-=(BigInt&);
		dllexport BigInt& operator-=(uint64_t&);

		dllexport BigInt operator*(string&);
		dllexport BigInt operator*(BigInt&);
		dllexport BigInt operator*(uint64_t);
		dllexport BigInt& operator*=(string&);
		dllexport BigInt& operator*=(BigInt&);
		dllexport BigInt& operator*=(uint64_t);

		dllexport BigInt operator/(string&);
		dllexport BigInt operator/(BigInt&);
		dllexport BigInt operator/(uint64_t&);
		dllexport BigInt& operator/=(string&);
		dllexport BigInt& operator/=(BigInt&);
		dllexport BigInt& operator/=(uint64_t&);

	private:
		void setNum(string);
		void convertNum(string);

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
