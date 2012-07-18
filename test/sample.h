//---------------------------------------------------------------------------

#ifndef sampleH
#define sampleH
//---------------------------------------------------------------------------

#define BOOST_MEM_FN_ENABLE_FASTCALL    	//Enables __fastcall calling convention. See boost::bind documentaion
														//use this macro before including jrtti.hpp
#include "../include/jrtti.hpp"

#include <iostream>
#include <vector>


struct Point
{
	double x;
	double y;

	Point()
	{
		x = -1;
		y = -1;
	}

	bool operator == (const Point & other) const
	{
		return (x == other.x) &&
		(y == other.y);
	}
};

struct Date
{
	int d, m, y;
	Point place;
	bool operator == (const Date & other) const
	{
		return (d == other.d) &&
		(m == other.m) &&
		(y == other.y);
	}
};

class SampleBase
{
public:
	virtual int getIntAbstract() = 0;
	virtual int getIntOverloaded() {return 99;}
};

class Sample : public SampleBase
{
public:
	Sample(){}

	int intMember;


	virtual int getIntAbstract() { return 34; }
	virtual int getIntOverloaded() {return 87;}

	void __fastcall setDoubleProp(double d) { test = d; }
	double getDoubleProp() { return test; }

	std::string getStdStringProp(){ return _s; }
	void	setStdStringProp(std::string str) { _s = str; }

	Point * getByPtrProp() {
		return _point;
	}
	void setByPtrProp(Point * p) { _point = p; }

	Date getByValProp() { return _date; }
	void setByValProp( const Date&  d) { _date = d; }     // Although property is defined as Value, seter can be reference

	Date& getByRefProp(){ return _date; }
//	void setByRefProp(Date& d) { _date = d; }

	bool getBool() { return boolVal; }
	void setBool( bool val ) { boolVal = val; }

	// exposed methods
	void testFunc(){ std::cout << "Test works ok" << std::endl;}
	int testIntFunc(){return 23;}
	double testSquare(double val){return val*val;}
	double testSum(int a, double b){return (double)a+b;}

	typedef std::vector< Date > Collection;
	Collection& getCollection(){ return _collection; }
	void setCollection( Collection& col ){ _collection = col; }

private:	// User declarations
	double test;
	Point * _point;
	Date	_date;
	std::string	_s;
	bool boolVal;
	Collection _collection;
};

class SampleDerived : public Sample
{
	virtual int getIntOverloaded() {return 43;}
};

class CustomPropertyCategories : public jrtti::PropertyCategories {
public:
	static const int inMenu	 		= jrtti::PropertyCategories::lastCategory << 1;
	static const int inToolBar		= jrtti::PropertyCategories::lastCategory << 2;
	static const int lastCategory	= inToolBar;

	bool
	showInMenu() {
		return categories() & inMenu;
	}

	bool
	showInToolBar() {
		return categories() & inToolBar;
	}
};

void declare()
{

	jrtti::declare<Point>()
						.property("x", &Point::x)
						.property("y", &Point::y);

	jrtti::declare<Date>()
						.property("d", &Date::d)
						.property("m", &Date::m)
						.property("y", &Date::y)
						.property("place", &Date::place);

	jrtti::declareAbstract<SampleBase>()
						.property("intAbstract", &SampleBase::getIntAbstract)
						.property("intOverloaded", &SampleBase::getIntOverloaded);

	jrtti::declare<Sample>()
				.inheritsFrom<SampleBase>()
						.property("intMember", &Sample::intMember, jrtti::PropertyCategories() << jrtti::PropertyCategories::nonstreamable )
						.property("testDouble", &Sample::setDoubleProp, &Sample::getDoubleProp, CustomPropertyCategories() << CustomPropertyCategories::inMenu )
						.property("point", &Sample::setByPtrProp, &Sample::getByPtrProp)
						.property("date", &Sample::setByValProp, &Sample::getByValProp )
						.property("refToDate", &Sample::getByRefProp)
						.property("testStr", &Sample::setStdStringProp,&Sample::getStdStringProp)
						.property("testRO", &Sample::testIntFunc)
						.property("testBool", &Sample::setBool, &Sample::getBool)
						.property("collection", &Sample::setCollection, &Sample::getCollection )

						.method<void>("testMethod", &Sample::testFunc)
						.method<int>("testIntMethod", &Sample::testIntFunc)
						.method<double,double>("testSquare", &Sample::testSquare)
						.method<double,int,double>("testSum", &Sample::testSum);

	jrtti::declare<SampleDerived>()
				.inheritsFrom("Sample");

	jrtti::declareCollection< Sample::Collection >();
}

void useCase() {
	Sample s;

	//set the property intMember of s object to 15
	jrtti::getType( "Sample" ).property( "intMember" ).set( &s, 15 );

	//retrieve the value of intMember from s object
	int i = jrtti::getType( "Sample" ).property( "intMember" ).get<int>( &s );

	//same as above using braket operator
	i = jrtti::getType("Sample")[ "intMember" ].get<int>( &s );

	//getting a Metatype object
	jrtti::Metatype & mt = jrtti::getType("Sample");
	//and working with it
	mt[ "intMember" ].set( &s, 23 );

	//call a method returning int and having two parameters
	double d = mt.call<double,Sample,int,double>( "testSum", &s, 3, 8 );
	//or
	d = mt.call<double>( "testSum", &s, 3, 8.0 );

	//set value from a fully qualified path
	mt.apply( &s, "date.place.x", 25.0 );
	//get value from a fully qualified path
	d = mt.eval<double>( &s, "date.place.x" );

	//get a string representation of s object
	std::string contens = mt.toStr( &s );
	//and set the s object from a string representation
	mt.fromStr( &s, contens );

	d=i+d;
}

#endif
