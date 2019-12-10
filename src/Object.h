#ifndef OBJECT_H
#define OBJECT_H

#include <utility>
#include <iostream>
#include <iomanip>
#include <string>
#include "BigInt.h"
#include <vector>

class Object;

typedef std::vector<Object> List;
enum flow_t
{
	BREAK,
	CONTINUE,
	RETURN,
	NORMAL
};
enum type_t
{
	NONE,
	INT,
	FLOAT,
	BOOL,
	STRING,
	LIST,
	FLOW
};

class Object
{
	friend std::ostream &operator<<(std::ostream &os, const Object b)
	{
		os << b.toString();
		return os;
	}

private:
	BigInt int_v;
	double float_v;
	bool bool_v;
	std::string string_v;
	List list_v;

public:
	type_t type;
	flow_t flow_v;

public:
	Object(): type(NONE)
	{
	}

	Object(type_t _type): type(_type)
	{
	}

	Object(BigInt val): type(INT), int_v(val)
	{
	}

	Object(double val): type(FLOAT), float_v(val)
	{
	}

	Object(bool val): type(BOOL), bool_v(val)
	{
	}

	Object(std::string val): type(STRING), string_v(val)
	{
	}

	Object(List val): type(LIST), list_v(val)
	{
	}

public:
	BigInt toInt() const
	{
		switch(type)
		{
		case INT:
			return int_v;
		case FLOAT:
			return BigInt((long long)float_v);
		case BOOL:
			return BigInt((long long)bool_v);
		case STRING:
			return BigInt(string_v);
		default:
			return BigInt();
		}
	}

	double toFloat() const
	{
		switch(type)
		{
		case INT:
			return (double)(int_v);
		case FLOAT:
			return float_v;
		case BOOL:
			return (double)bool_v;
		case STRING:
			return std::stod(string_v);
		default:
			return (float)0;
		}
	}

	bool toBool() const
	{
		switch(type)
		{
		case INT:
			return int_v != 0;
		case FLOAT:
			return float_v != 0;
		case BOOL:
			return bool_v;
		case STRING:
			return string_v == std::string("True");
		default:
			return false;
		}
	}

	std::string toString() const
	{
		switch(type)
		{
		case NONE:
			return std::string("None");
		case INT:
			return int_v.toString();
		case FLOAT:
			return std::to_string(float_v);
		case BOOL:
			return std::string(bool_v ? "True" : "False");
		case STRING:
			return string_v;
		default:
			return std::string("");
		}
	}

public:
	Object operator<(const Object &b) const
	{
		if(type == NONE || b.type == NONE)
		{
			std::cerr << "Error: Cannot compare a variable with a None type." << std::endl;
			return Object(false);
		}
		if(type == STRING && b.type == STRING)
		{
			return Object(toString() < b.toString());
		}
		if(type != STRING && b.type != STRING)
		{
			return Object(toFloat() < b.toFloat());
		}
		return Object(type == STRING);
	}

	Object operator>(const Object &b) const
	{
		return b < *this;
	}

	Object operator<=(const Object &b) const
	{
		return !(*this > b);
	}

	Object operator>=(const Object &b) const
	{
		return !(*this < b);
	}

	Object operator==(const Object &b) const
	{
		if(type == b.type)
		{
			if(type != NONE)
			{
				return Object(toString() == b.toString());
			}
			return Object(true);
		}
		return Object(false);
	}

	Object operator!=(const Object &b) const
	{
		return !(*this == b);
	}

	Object operator-() const
	{
		return Object(BigInt(0)) - *this;
	}

	Object operator!() const
	{
		return Object(!this->toBool());
	}

	Object operator||(const Object &b) const
	{
		if(type == NONE || b.type == NONE)
		{
			std::cerr << "Error: Cannot OR a variable and a None type.";
			return Object();
		}
		return toBool() || b.toBool();
	}

	Object operator&&(const Object &b) const
	{
		if(type == NONE || b.type == NONE)
		{
			std::cerr << "Error: Cannot AND a variable and a None type.";
			return Object();
		}
		return toBool() && b.toBool();
	}

	Object operator+(const Object &b) const
	{
		if(type == NONE || b.type == NONE)
		{
			std::cerr << "Error: Cannot add a variable and a None type." << std::endl;
			return Object(NONE);
		}
		if(type == STRING && b.type == STRING)
		{
			return Object(toString() + b.toString());
		}
		if((type == INT || type == BOOL) && (b.type == INT || b.type == BOOL))
		{
			return Object(toInt() + b.toInt());
		}
		if(type == FLOAT || b.type == FLOAT)
		{
			return Object(toFloat() + b.toFloat());
		}
		std::cerr << "Error: Cannot add a string type and a numeric type." << std::endl;
		return Object(NONE);
	}


	Object operator-(const Object &b) const
	{
		if(type == NONE || b.type == NONE)
		{
			std::cerr << "Error: Cannot subtract a variable and a None type." << std::endl;
			return Object(NONE);
		}
		if(type == STRING || b.type == STRING)
		{
			std::cerr << "Error: Cannot subtract a variable and a string type." << std::endl;
			return Object(NONE);
		}
		if(type == FLOAT || b.type == FLOAT)
		{
			return Object(toFloat() - b.toFloat());
		}
		return Object(toInt() - b.toInt());
	}


	Object operator*(const Object &b) const
	{
		if(type == NONE || b.type == NONE)
		{
			std::cerr << "Error: Cannot multiply a variable and a None type." << std::endl;
			return Object(NONE);
		}
		if(type == STRING && b.type == STRING)
		{
			std::cerr << "Error: Cannot multiply two string type." << std::endl;
			return Object(NONE);
		}
		if((type == INT || type == BOOL) && (b.type == INT || b.type == BOOL))
		{
			return Object(toInt() * b.toInt());
		}
		if(type == FLOAT || b.type == FLOAT)
		{
			return Object(toFloat() * b.toFloat());
		}
		if(type == STRING && (b.type == INT || b.type == BOOL))
		{
			Object ret(*this);
			for(BigInt i = 1; i < b.toInt(); i++)
			{
				ret += *this;
			}
			return ret;
		}
		if((type == INT || type == BOOL) && b.type == STRING)
		{
			return b * (*this);
		}
		std::cerr << "Error: Cannot multiply a string type and a float type." << std::endl;
		return Object(NONE);
	}


	Object operator/(const Object &b) const
	{
		if(type == NONE || b.type == NONE)
		{
			std::cerr << "Error: Cannot divide a variable and a None type." << std::endl;
			return Object(NONE);
		}
		if(type == STRING || b.type == STRING)
		{
			std::cerr << "Error: Cannot divide a variable and a string type." << std::endl;
			return Object(NONE);
		}
		if(type == FLOAT || b.type == FLOAT)
		{
			return Object(toFloat() / b.toFloat());
		}
		return Object(BigInt::div(toInt(), b.toInt()));
	}


	Object operator%(const Object &b) const
	{
		if((type != INT && type != BOOL) || (b.type != INT && b.type != BOOL))
		{
			std::cerr << "Error: Cannot mod a variable and a non-integer type." << std::endl;
			return Object(NONE);
		}
		return Object(toInt() % b.toInt());
	}


	Object idiv(const Object &b) const
	{
		if((type != INT && type != BOOL) || (b.type != INT && b.type != BOOL))
		{
			std::cerr << "Error: Cannot int-divide a variable and a non-integer type." << std::endl;
			return Object(NONE);
		}
		return Object(toInt() / b.toInt());
	}


	Object &operator|=(const Object &b)
	{
		return *this = (*this || b);
	}


	Object &operator&=(const Object &b)
	{
		return *this = (*this && b);
	}


	Object &operator+=(const Object &b)
	{
		return *this = (*this + b);
	}


	Object &operator-=(const Object &b)
	{
		return *this = (*this - b);
	}


	Object &operator*=(const Object &b)
	{
		return *this = (*this * b);
	}


	Object &operator/=(const Object &b)
	{
		return *this = (*this / b);
	}


	Object &operator%=(const Object &b)
	{
		return *this = (*this % b);
	}
};

#endif //OBJECT_H