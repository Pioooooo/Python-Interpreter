#ifndef OBJECT_H
#define OBJECT_H
#pragma once

#include <utility>
#include <iostream>
#include <iomanip>
#include <string>
#include <type_traits>
#include "BigInt.h"
#include <stack>
#include <vector>
#include <unordered_map>

class Object;

typedef std::vector<Object> List;

class None
{
public:
	template<typename __T>
	friend bool operator==(const __T &a, const None &b)
	{
		return typeid(__T) == typeid(None);
	}

	template<typename __T>
	friend bool operator==(const None &a, const __T &b)
	{
		return typeid(__T) == typeid(None);
	}
};

namespace _detail_
{
	class Data
	{
	public:
		virtual ~Data()
		{
		}

	public:
		virtual const std::type_info &type() const = 0;

		virtual Data *clone() const = 0;

		virtual BigInt toInt() const
		{
			return BigInt();
		}

		virtual double toFloat() const
		{
			return 0;
		}

		virtual bool toBool() const
		{
			return false;
		}

		virtual std::string toString() const
		{
			return std::string("");
		}


		virtual explicit operator BigInt() const
		{
			return toInt();
		}


		virtual explicit operator double() const
		{
			return toFloat();
		}


		virtual explicit operator bool() const
		{
			return toBool();
		}


		virtual explicit operator std::string() const
		{
			return toString();
		}

	};

	template<typename __T>
	class DataHolder_impl: public Data
	{
	public:
		DataHolder_impl(const __T &a): held(a)
		{
		}

	public:
		virtual const std::type_info &type() const
		{
			return typeid(__T);
		}

		virtual Data *clone() const override
		{
			return new DataHolder_impl(held);
		}

	public:
		__T held;

	public:
		DataHolder_impl &operator=(const DataHolder_impl &) = delete;

	public:
		BigInt toInt() const override
		{
			return BigInt();
		}

		double toFloat() const override
		{
			return 0;
		}

		bool toBool() const override
		{
			return false;
		}

		std::string toString() const override
		{
			return std::string("");
		}

		__T value() const
		{
			return held;
		}
	};

	template<>
	class DataHolder_impl<BigInt>: public Data
	{
	public:
		DataHolder_impl(const BigInt &a): held(a)
		{
		}

	public:
		virtual const std::type_info &type() const
		{
			return typeid(BigInt);
		}

		virtual Data *clone() const override
		{
			return new DataHolder_impl(held);
		}

	public:
		BigInt held;

	public:
		DataHolder_impl &operator=(const DataHolder_impl &) = delete;

	public:
		BigInt toInt() const
		{
			return held;
		}

//TODO: higher precision?
		double toFloat() const
		{
			return (double)held;
		}

		bool toBool() const
		{
			return held != 0;
		}

		std::string toString() const
		{
			return held.toString();
		}


		BigInt value() const
		{
			return held;
		}
	};


	template<>
	class DataHolder_impl<double>: public Data
	{
	public:
		DataHolder_impl(const double &a): held(a)
		{
		}

	public:
		virtual const std::type_info &type() const
		{
			return typeid(double);
		}

		virtual Data *clone() const override
		{
			return new DataHolder_impl(held);
		}

	public:
		double held;

	public:
		DataHolder_impl &operator=(const DataHolder_impl &) = delete;

	public:
		BigInt toInt() const override
		{
			return BigInt();
		}

		double toFloat() const
		{
			return held;
		}

		bool toBool() const
		{
			return held != 0;
		}

		std::string toString() const
		{
			std::stringstream buf;
			buf << std::ios::fixed << std::setprecision(6) << held;
			return buf.str();
		}

		double value() const
		{
			return held;
		}
	};


	template<>
	class DataHolder_impl<bool>: public Data
	{
	public:
		DataHolder_impl(const bool &a): held(a)
		{
		}

	public:
		virtual const std::type_info &type() const
		{
			return typeid(bool);
		}

		virtual Data *clone() const override
		{
			return new DataHolder_impl(held);
		}

	public:
		bool held;

	public:
		DataHolder_impl &operator=(const DataHolder_impl &) = delete;

	public:
		BigInt toInt() const
		{
			return BigInt(held);
		}

		double toFloat() const
		{
			return (double)held;
		}

		bool toBool() const
		{
			return held;
		}

		std::string toString() const
		{
			return std::string(held ? "True" : "False");
		}

		bool value() const
		{
			return held;
		}
	};

	template<>
	class DataHolder_impl<std::string>: public Data
	{
	public:
		DataHolder_impl(const std::string &a): held(a)
		{
		}

	public:
		virtual const std::type_info &type() const
		{
			return typeid(std::string);
		}

		virtual Data *clone() const override
		{
			return new DataHolder_impl(held);
		}

	public:
		std::string held;

	public:
		DataHolder_impl &operator=(const DataHolder_impl &) = delete;

	public:
		BigInt toInt() const
		{
			return BigInt(held);
		}

		double toFloat() const
		{
			return std::stod(held);
		}

		bool toBool() const
		{
			return held != std::string("");
		}

		std::string toString() const
		{
			return held;
		}

		std::string value() const
		{
			return held;
		}
	};

	template<>
	class DataHolder_impl<None>: public Data
	{
	public:
		DataHolder_impl(const None &a): held(a)
		{
		}

	public:
		virtual const std::type_info &type() const
		{
			return typeid(None);
		}

		virtual Data *clone() const override
		{
			return new DataHolder_impl(held);
		}

	public:
		None held;

	public:
		DataHolder_impl &operator=(const DataHolder_impl &) = delete;

	public:
		BigInt toInt() const
		{
			return BigInt();
		}

		double toFloat() const
		{
			return (double)0;
		}

		bool toBool() const
		{
			return false;
		}

		std::string toString() const
		{
			return std::string("None");
		}

		None value() const
		{
			return held;
		}
	};

}


class Object
{
	friend std::ostream &operator<<(std::ostream &os, const Object b)
	{
		os << b.toString();
		return os;
	}

public:
	Object(): dat(new DataHolder<None>(None()))
	{
	}

	Object(const Object &other): dat(other.dat ? other.dat->clone() : nullptr)
	{
	}


	template<typename __T, typename = std::enable_if_t<std::is_same<std::decay_t<__T>, BigInt>::value || std::is_same<std::decay_t<__T>, double>::value || std::is_same<std::decay_t<__T>, bool>::value || std::is_same<std::decay_t<__T>, std::string>::value || std::is_same<std::decay_t<__T>, None>::value || std::is_same<std::decay_t<__T>, List>::value>>

	Object(const __T &a): dat(new DataHolder<__T>(a))
	{
	}


	~Object()
	{
		delete dat;
	}

public:
	Object &swap(Object &b)
	{
		std::swap(dat, b.dat);
		return *this;
	}

	template<typename __T>
	Object &operator=(const __T &b)
	{
		Object(b).swap(*this);
		return *this;
	}


	Object &operator=(Object b)
	{
		Object(b).swap(*this);
		return *this;
	}

	template<typename __T>
	class DataHolder: public _detail_::DataHolder_impl<__T>
	{
	public:
		DataHolder(const __T &a): _detail_::DataHolder_impl<__T>(a)
		{
		}
	};

public:
	bool empty() const
	{
		return !dat;
	}


	const std::type_info &type() const
	{
		return dat ? dat->type() : typeid(void);
	}

public:
	_detail_::Data *dat{};

public:
	template<typename __T>
	bool is() const
	{
		return dat ? typeid(__T) == dat->type() : typeid(__T) == typeid(void);
	}


	template<typename __T>
	__T as() const
	{
		return static_cast<Object::DataHolder<__T> *>(dat)->held;
	}


	BigInt toInt() const
	{
		return dat->toInt();
	}


	double toFloat() const
	{
		return dat->toFloat();
	}


	bool toBool() const
	{
		return dat->toBool();
	}


	std::string toString() const
	{
		return dat->toString();
	}


public:
	Object operator<(const Object &b) const
	{
		if(type() == typeid(None) || b.type() == typeid(None))
		{
			std::cerr << "Error: Cannot compare a variable with a None type." << std::endl;
			return Object(false);
		}
		if(type() == typeid(std::string) && b.type() == typeid(std::string))
		{
			return Object(toString() < b.toString());
		}
		if(type() != typeid(std::string) && b.type() != typeid(std::string))
		{
			return Object(toFloat() < b.toFloat());
		}
		return Object(type() == typeid(std::string));
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
		if(type() == b.type())
		{
			if(type() != typeid(None))
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
		return Object(!as<bool>());
	}


	Object operator||(const Object &b) const
	{
		if(type() == typeid(None) || b.type() == typeid(None))
		{
			std::cerr << "Error: Cannot OR a variable and a None type.";
			return Object();
		}
		return toBool() || b.toBool();
	}


	Object operator&&(const Object &b) const
	{
		if(type() == typeid(None) || b.type() == typeid(None))
		{
			std::cerr << "Error: Cannot AND a variable and a None type.";
			return Object();
		}
		return toBool() && b.toBool();
	}


	Object operator+(const Object &b) const
	{
		if(type() == typeid(None) || b.type() == typeid(None))
		{
			std::cerr << "Error: Cannot add a variable and a None type." << std::endl;
			return Object(None());
		}
		if(type() == typeid(std::string) && b.type() == typeid(std::string))
		{
			return Object(toString() + b.toString());
		}
		if((type() == typeid(BigInt) || type() == typeid(bool)) && (b.type() == typeid(BigInt) || b.type() == typeid(bool)))
		{
			return Object(toInt() + b.toInt());
		}
		if(type() == typeid(double) || b.type() == typeid(double))
		{
			return Object(toFloat() + b.toFloat());
		}
		std::cerr << "Error: Cannot add a string type and a numeric type." << std::endl;
		return Object(None());
	}


	Object operator-(const Object &b) const
	{
		if(type() == typeid(None) || b.type() == typeid(None))
		{
			std::cerr << "Error: Cannot subtract a variable and a None type." << std::endl;
			return Object(None());
		}
		if(type() == typeid(std::string) || b.type() == typeid(std::string))
		{
			std::cerr << "Error: Cannot subtract a variable and a string type." << std::endl;
			return Object(None());
		}
		if(type() == typeid(double) || b.type() == typeid(double))
		{
			return Object(toFloat() - b.toFloat());
		}
		return Object(toInt() - b.toInt());
	}


	Object operator*(const Object &b) const
	{
		if(type() == typeid(None) || b.type() == typeid(None))
		{
			std::cerr << "Error: Cannot multiply a variable and a None type." << std::endl;
			return Object(None());
		}
		if(type() == typeid(std::string) && b.type() == typeid(std::string))
		{
			std::cerr << "Error: Cannot multiply two string type." << std::endl;
			return Object(None());
		}
		if((type() == typeid(BigInt) || type() == typeid(bool)) && (b.type() == typeid(BigInt) || b.type() == typeid(bool)))
		{
			return Object(toInt() * b.toInt());
		}
		if(type() == typeid(double) || b.type() == typeid(double))
		{
			return Object(toFloat() * b.toFloat());
		}
		if(type() == typeid(std::string) && (b.type() == typeid(BigInt) || b.type() == typeid(bool)))
		{
			Object ret(*this);
			for(BigInt i = 1; i < b.toInt(); i++)
			{
				ret += *this;
			}
			return ret;
		}
		if((type() == typeid(BigInt) || type() == typeid(bool)) && b.type() == typeid(std::string))
		{
			return b * *this;
		}
		std::cerr << "Error: Cannot multiply a string type and a float type." << std::endl;
		return Object(None());
	}


	Object operator/(const Object &b) const
	{
		if(type() == typeid(None) || b.type() == typeid(None))
		{
			std::cerr << "Error: Cannot divide a variable and a None type." << std::endl;
			return Object(None());
		}
		if(type() == typeid(std::string) || b.type() == typeid(std::string))
		{
			std::cerr << "Error: Cannot divide a variable and a string type." << std::endl;
			return Object(None());
		}
		if(type() == typeid(double) || b.type() == typeid(double))
		{
			return Object(toFloat() / b.toFloat());
		}
		return Object(BigInt::div(toInt(), b.toInt()));
	}


	Object operator%(const Object &b) const
	{
		if((type() != typeid(BigInt) && type() != typeid(bool)) || (b.type() != typeid(BigInt) && b.type() != typeid(bool)))
		{
			std::cerr << "Error: Cannot mod a variable and a non-integer type." << std::endl;
			return Object(None());
		}
		return Object(toInt() % b.toInt());
	}


	Object idiv(const Object &b) const
	{
		if((type() != typeid(BigInt) && type() != typeid(bool)) || (b.type() != typeid(BigInt) && b.type() != typeid(bool)))
		{
			std::cerr << "Error: Cannot int-divide a variable and a non-integer type." << std::endl;
			return Object(None());
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