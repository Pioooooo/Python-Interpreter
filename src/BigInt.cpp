#ifndef BIG_INT_CPP
#define BIG_INT_CPP

#include "BigInt.h"

BigInt::BigInt()
{
	number.push_back(0);
	positive = false;
}

BigInt::BigInt(long long a)
{
	positive = a < 0;
	if(positive)
		a *= -1;
	while(a)
	{
		number.push_back(a % base);
		a /= base;
	}
	if(number.empty())
		number.push_back(0);
}

BigInt::BigInt(std::string a)
{
	int val = 0;
	positive = a[0] == '-';
	int endpos = 0;
	if(positive)
		endpos = 1;
	int i = a.length() - 1, tmp = a.find('.');
	if(tmp != std::string::npos)
	{
		for(unsigned j = tmp + 1; j < a.size(); j++)
			if(a[j] != '0')
			{
				i = tmp - 1;
				break;
			}
	}
	for(int t = 1; i >= endpos; i--)
	{
		val = val * 10 + a[i] - '0';
		t *= 10;
		if(t >= base)
		{
			number.push_back(val % base);
			val /= base;
			t = 1;
		}
	}
	if(val)
		number.push_back(val);
	if(number.empty())
	{
		positive = false;
		number.push_back(0);
	}
}

BigInt &BigInt::operator=(const BigInt &b) = default;

BigInt &BigInt::operator+=(const BigInt &b)
{
	if(positive != b.positive)
		return (*this) -= -b;
	int x = 0;
	if(number.size() <= b.number.size())
	{
		number.resize(std::max(number.size(), b.number.size() + 1), 0);
		for(unsigned i = 0; i < number.size() - 1; i++)
		{
			number[i] += i < b.number.size() ? b.number[i] : 0;
			number[i + 1] += number[i] / base;
			number[i] %= base;
		}
		if(number.back() == 0)
			number.pop_back();
		return *this;
	}
	*this = b + *this;
	return *this;
}

BigInt &BigInt::operator-=(const BigInt &b)
{
	if(positive != b.positive)
		return (*this) += -b;
	BigInt tmp;
	if(abs(*this) < abs(b))
	{
		tmp = *this;
		*this = b;
		positive ^= true;
	}
	else
		tmp = b;
	for(unsigned i = 0; i < tmp.number.size(); i++)
	{
		number[i] -= tmp.number[i];
		if(number[i] < 0)
		{
			number[i] += base;
			number[i + 1]--;
		}
	}
	for(unsigned i = tmp.number.size(); i < number.size() - 1; i++)
	{
		if(number[i] < 0)
		{
			number[i] += base;
			number[i + 1]--;
		}
		else
			break;
	}
	while(!number.empty() && number.back() == 0)
		number.pop_back();
	if(number.empty())
	{
		number.push_back(0);
		positive = false;
	}
	return *this;
}

BigInt &BigInt::operator*=(const BigInt &b)
{
	return *this = *this * b;
}

BigInt &BigInt::operator/=(const BigInt &b)
{
	BigInt ret;
	ret.positive = positive ^ b.positive;
	if(b.number.size() == 1 && b.number[0] == 0)
	{
		std::cerr << "Error: divisor cannot be 0.\n";
		return *this;
	}
	ret.number.resize(number.size() + 1 > b.number.size() ? number.size() - b.number.size() + 1 : 0);
	for(int i = number.size() - b.number.size(); i >= 0; i--)
	{
		while(greater(*this, b, i))
		{
			for(unsigned j = 0; j < b.number.size(); j++)
			{
				number[i + j] -= b.number[j];
				if(number[i + j] < 0)
				{
					number[i + j] += base;
					number[i + j + 1]--;
				}
			}
			ret.number[i]++;
		}
	}
	while(!number.empty() && number.back() == 0)
		number.pop_back();
	if(!number.empty())
	{
		ret -= ret.positive;
	}
	while(!ret.number.empty() && ret.number.back() == 0)
		ret.number.pop_back();
	if(ret.number.empty())
	{
		ret.number.push_back(0);
		ret.positive = false;
	}
	return *this = ret;
}

BigInt &BigInt::operator%=(const BigInt &b)
{
	BigInt ret;
	ret.positive = positive ^ b.positive;
	ret.number.resize(number.size() + 1 > b.number.size() ? number.size() - b.number.size() + 1 : 0);
	if(b.number.size() == 1 && b.number[0] == 0)
	{
		std::cerr << "Error: divisor cannot be 0.\n";
		return *this;
	}
	for(int i = number.size() - b.number.size(); i >= 0; i--)
	{
		while(greater(*this, b, i))
		{
			for(unsigned j = 0; j < b.number.size(); j++)
			{
				number[i + j] -= b.number[j];
				if(number[i + j] < 0)
				{
					number[i + j] += base;
					number[i + j + 1]--;
				}
			}
			ret.number[i]++;
		}
	}
	while(!ret.number.empty() && ret.number.back() == 0)
		ret.number.pop_back();
	if(ret.number.empty())
	{
		ret.number.push_back(0);
		ret.positive = false;
	}
	if(*this != 0)
		ret -= ret.positive;
	while(!number.empty() && number.back() == 0)
		number.pop_back();
	if(number.empty())
	{
		number.push_back(0);
		positive = false;
	}
	return *this;
}

BigInt &BigInt::operator++()
{
	return *this += 1;
}

BigInt BigInt::operator++(int b)
{
	BigInt tmp = *this;
	*this += 1;
	return tmp;
}

BigInt &BigInt::operator--()
{
	return *this -= 1;
}

BigInt BigInt::operator--(int b)
{
	BigInt tmp = *this;
	*this -= 1;
	return tmp;
}

double BigInt::div(const BigInt &a, const BigInt &b)
{
	BigInt rem(a);
	BigInt ret;
	ret.positive = rem.positive ^ b.positive;
	ret.number.resize(rem.number.size() - b.number.size() + 1);
	if(b.number.size() == 1 && b.number[0] == 0)
	{
		std::cerr << "Error: divisor cannot be 0.\n";
		return 0;
	}
	for(int i = rem.number.size() - b.number.size(); i >= 0; i--)
	{
		while(greater(rem, b, i))
		{
			for(unsigned j = 0; j < b.number.size(); j++)
			{
				rem.number[i + j] -= b.number[j];
				if(rem.number[i + j] < 0)
				{
					rem.number[i + j] += base;
					rem.number[i + j + 1]--;
				}
			}
			ret.number[i]++;
		}
	}
	while(!ret.number.empty() && ret.number.back() == 0)
		ret.number.pop_back();
	if(ret.number.empty())
	{
		ret.number.push_back(0);
		ret.positive = false;
	}
	std::stringstream ans;
	ans << ret;
	if(rem != 0)
		ans << '.' << rem * 10000000 / b;
	double tmp;
	ans >> tmp;
	return tmp;
}

std::ostream &operator<<(std::ostream &os, const BigInt a)
{
	os << a.toString();
	return os;
}

std::istream &operator>>(std::istream &is, BigInt &a)
{
	std::string tmp;
	is >> tmp;
	a = BigInt(tmp);
	return is;
}

bool operator<(const BigInt &a, const BigInt &b)
{
	if(a.positive != b.positive)
		return a.positive - b.positive;
	bool flag = false;
	if(a.positive == 1 && b.positive == 1)
		flag = true;
	if(a.number.size() != b.number.size())
		return (a.number.size() < b.number.size()) ^ flag;
	for(int i = a.number.size() - 1; i >= 0; i--)
		if(a.number[i] != b.number[i])
			return (a.number[i] < b.number[i]) ^ flag;
	return false;
}

bool operator>(const BigInt &a, const BigInt &b)
{
	return b < a;
}

bool operator<=(const BigInt &a, const BigInt &b)
{
	return !(b < a);
}

bool operator>=(const BigInt &a, const BigInt &b)
{
	return !(a < b);
}

bool operator==(const BigInt &a, const BigInt &b)
{
	return a.positive == b.positive && a.number == b.number;
}

bool operator!=(const BigInt &a, const BigInt &b)
{
	return !(a == b);
}

BigInt operator+(const BigInt &a, const BigInt &b)
{
	BigInt ret(a);
	return ret += b;
}

BigInt operator-(const BigInt &a, const BigInt &b)
{
	BigInt ret(a);
	return ret -= b;
}

BigInt operator*(const BigInt &a, const BigInt &b)
{
	BigInt ret;
	ret.positive = a.positive ^ b.positive;
	ret.number.resize(a.number.size() * b.number.size() + 1, 0);
	for(unsigned i = 0; i < a.number.size(); i++)
	{
		int carry = 0;
		for(unsigned j = 0; j < b.number.size(); j++)
		{
			int temp = a.number[i] * b.number[j] + ret.number[i + j] + carry;
			carry = temp / BigInt::base;
			ret.number[i + j] = temp % BigInt::base;
		}
		ret.number[i + b.number.size()] += carry;
	}
	while(!ret.number.empty() && ret.number.back() == 0)
		ret.number.pop_back();
	if(ret.number.empty())
	{
		ret.number.push_back(0);
		ret.positive = false;
	}
	return ret;
}

BigInt operator/(const BigInt &a, const BigInt &b)
{
	BigInt ret(a);
	return ret /= b;
}

BigInt operator%(const BigInt &a, const BigInt &b)
{
	BigInt ret(a);
	return ret %= b;
}

BigInt operator-(const BigInt &a)
{
	BigInt ret(a);
	ret.positive ^= true;
	return ret;
}

BigInt BigInt::abs(const BigInt &a)
{
	BigInt ret(a);
	ret.positive = false;
	return ret;
}

BigInt::operator double() const
{
	double ret = 0;
	for(int i = number.size() - 1; i >= 0; i--)
		(ret *= base) += number[i];
	if(positive)
		ret *= -1;
	return ret;
}

std::string BigInt::toString() const
{
	std::ostringstream oss;
	if(positive)
		oss << "-";
	for(int i = number.size() - 1; i >= 0; i--)
		oss << (int)number[i];
	return oss.str();
}

bool BigInt::greater(const BigInt &a, const BigInt &b, unsigned lastDigit)
{
	unsigned len = b.number.size();
	if(lastDigit + len < a.number.size() && a.number[lastDigit + len])
		return true;
	for(int i = len - 1; i >= 0; i--)
		if(a.number[lastDigit + i] != b.number[i])
			return a.number[lastDigit + i] > b.number[i];
	return true;
}

#endif //BIG_INT_CPP