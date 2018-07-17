#include "Real.h"

namespace VEFM
{
	Real Real::Max()
	{
		Real r;
		r.integerPart = 2147483647;
		r.fractionPart = 999999;
		return r;
	}

	Real Real::Min()
	{
		Real r;
		r.integerPart = 0;
		r.fractionPart = 000001;
		return r;
	}

	Real::Real()
	{
	}

	Real::~Real()
	{
	}

	Real::Real(int i)
	{
		double d = (double)i;
		fill(d);
	}

	Real::Real(float f)
	{
		double d = (double)f;
		fill(d);
	}

	Real::Real(double d)
	{
		fill(d);
	}

	void Real::fill(double d)
	{
		integerPart = ((int)abs(d));
		double fraction = d - (double)integerPart;
		fractionPart = (int)(fraction * 1000000);
	}

	float Real::Float() const
	{
		return (float)(integerPart) + ((float)fractionPart / 1000000.0f);
	}

	double Real::Double() const
	{
		return (double)(integerPart) + ((double)fractionPart / 1000000.0);
	}

	bool Real::operator == (float other) const
	{
		return (integerPart == other.integerPart) && (fractionPart == other.fractionPart);
	}

	bool Real::operator != (float other) const
	{
		return (integerPart != other.integerPart) || (fractionPart != other.fractionPart);
	}

	bool Real::operator < (float other) const
	{
		if (integerPart > other.integerPart) return false;
		else if (integerPart < other.integerPart) return true;
		else
		{
			if (fractionPart > other.fractionPart) return false;
			else if (fractionPart < other.fractionPart) return true;
			else return false;
		}
	}

	bool Real::operator > (float other) const
	{
		if (integerPart < other.integerPart) return false;
		else if (integerPart > other.integerPart) return true;
		else
		{
			if (fractionPart < other.fractionPart) return false;
			else if (fractionPart > other.fractionPart) return true;
			else return false;
		}
	}

	bool Real::operator <= (float other) const
	{
		if (integerPart > other.integerPart) return false;
		else if (integerPart < other.integerPart) return true;
		else
		{
			if (fractionPart > other.fractionPart) return false;
			else if (fractionPart < other.fractionPart) return true;
			else return true;
		}
	}

	bool Real::operator >= (float other) const
	{
		if (integerPart < other.integerPart) return false;
		else if (integerPart > other.integerPart) return true;
		else
		{
			if (fractionPart < other.fractionPart) return false;
			else if (fractionPart > other.fractionPart) return true;
			else return true;
		}
	}

	Real Real::operator - ()
	{
		fractionPart = -fractionPart;
		return *this;
	}

	Real Real::operator + (float other) const
	{
		return Real(Double() + other.Double());
	}

	Real Real::operator - (float other) const
	{
		return Real(Double() - other.Double());
	}

	Real Real::operator * (float other) const
	{
		return Real(Double() * other.Double());
	}

	Real Real::operator / (float other) const
	{
		return Real(Double() / other.Double());
	}

	Real Real::operator += (float other)
	{
		double d = Double() + other.Double();
		fill(d);
		return *this;
	}

	Real Real::operator -= (float other)
	{
		double d = Double() - other.Double();
		fill(d);
		return *this;
	}

	Real Real::operator *= (float other)
	{
		double d = Double() * other.Double();
		fill(d);
		return *this;
	}

	Real Real::operator /= (float other)
	{
		double d = Double() / other.Double();
		fill(d);
		return *this;
	}

	//bool operator == (float a, float b)
	//{
	//	return a.operator==(b);
	//}

	//bool operator != (float a, float b)
	//{
	//	return a != b;
	//}

	//bool operator < (float a, float b)
	//{
	//	return a < b;
	//}

	//bool operator > (float a, float b)
	//{
	//	return a > b;
	//}

	//bool operator <= (float a, float b)
	//{
	//	return a <= b;
	//}

	//bool operator >= (float a, float b)
	//{
	//	return a >= b;
	//}

	//Real operator + (float a, float b)
	//{
	//	return a + b;
	//}

	//Real operator - (float a, float b)
	//{
	//	return a - b;
	//}

	//Real operator * (float a, float b)
	//{
	//	return a * b;
	//}

	//Real operator / (float a, float b)
	//{
	//	return a / b;
	//}
}
