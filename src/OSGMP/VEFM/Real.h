#pragma once

#include <float.h>
#include <math.h>

namespace VEFM
{
	class Real
	{
	public:
		Real();
		Real(int i);
		Real(float f);
		Real(double d);
		~Real();

		float Float() const;
		double Double() const;

		bool operator == (const Real& other) const;
		bool operator != (const Real& other) const;

		bool operator < (const Real& other) const;
		bool operator > (const Real& other) const;
		bool operator <= (const Real& other) const;
		bool operator >= (const Real& other) const;

		Real operator - ();

		Real operator + (const Real& other) const;
		Real operator - (const Real& other) const;
		Real operator * (const Real& other) const;
		Real operator / (const Real& other) const;

		Real operator += (const Real& other);
		Real operator -= (const Real& other);
		Real operator *= (const Real& other);
		Real operator /= (const Real& other);

		//friend bool operator == (const Real& a, const Real& b);
		//friend bool operator != (const Real& a, const Real& b);

		//friend bool operator < (const Real& a, const Real& b);
		//friend bool operator > (const Real& a, const Real& b);
		//friend bool operator <= (const Real& a, const Real& b);
		//friend bool operator >= (const Real& a, const Real& b);

		//friend Real operator + (const Real& a, const Real& b);
		//friend Real operator - (const Real& a, const Real& b);
		//friend Real operator * (const Real& a, const Real& b);
		//friend Real operator / (const Real& a, const Real& b);

		static Real Max();
		static Real Min();

	private:
		int integerPart;
		int fractionPart;
		void fill(double d);
	};
}
