#ifndef BIG_INT_H
#define BIG_INT_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>

class BigInt
{
	friend std::ostream &operator<<(std::ostream &os, const BigInt a);
	friend std::istream &operator>>(std::istream &is, BigInt &a);
	friend bool operator<(const BigInt &a, const BigInt &b);
	friend bool operator>(const BigInt &a, const BigInt &b);
	friend bool operator<=(const BigInt &a, const BigInt &b);
	friend bool operator>=(const BigInt &a, const BigInt &b);
	friend bool operator==(const BigInt &a, const BigInt &b);
	friend bool operator!=(const BigInt &a, const BigInt &b);
	friend BigInt operator+(const BigInt &a, const BigInt &b);
	friend BigInt operator-(const BigInt &a, const BigInt &b);
	friend BigInt operator*(const BigInt &a, const BigInt &b);
	friend BigInt operator/(const BigInt &a, const BigInt &b);
	friend BigInt operator%(const BigInt &a, const BigInt &b);
	friend BigInt operator-(const BigInt &a);

private:
	std::vector<char> number;
	bool positive;
	const static int base = 10;

public:
	BigInt();
	BigInt(long long a);
	BigInt(std::string a);
	BigInt(const BigInt &a) = default;
	BigInt(BigInt &&a) = default;

	BigInt &operator=(const BigInt &b);

	BigInt &operator+=(const BigInt &b);
	BigInt &operator-=(const BigInt &b);
	BigInt &operator*=(const BigInt &b);
	BigInt &operator/=(const BigInt &b);
	BigInt &operator%=(const BigInt &b);
	BigInt &operator++();
	const BigInt operator++(int b);
	BigInt &operator--();
	const BigInt operator--(int b);

	static double div(const BigInt &a, const BigInt &b);
	static BigInt abs(const BigInt &a);

	explicit operator double() const;

	std::string toString() const;

private:
	static bool greater(const BigInt &a, const BigInt &b, unsigned lastDigit);
};

#endif //BIG_INT_H