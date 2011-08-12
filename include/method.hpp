#ifndef methodH
#define methodH

namespace jrtti {

//------------------------------------------------------------------------------

template <class ClassType, class ReturnType, class Param1=void, class Param2=void>
class Method
{
	typedef boost::function<ReturnType (ClassType*, Param1, Param2)> 	FunctionType;
	typedef Method<ClassType, ReturnType, Param1, Param2 >					MethodType;

public:
	MethodType&
	name(std::string name)
	{
		m_name=name;
		return *this;
	}

	MethodType&
	function(FunctionType f)
	{
		m_functor=f;
		return *this;
	}

	ReturnType
	call(ClassType * instance, Param1 p1, Param2 p2)
	{
		return (ReturnType)m_functor(instance,p1,p2);
	}

private:
	FunctionType 	m_functor;
	std::string		m_name;
};

template <class ClassType, class ReturnType>
class Method<ClassType, ReturnType, void, void>
{
	typedef boost::function<ReturnType (ClassType*)> 	FunctionType;
	typedef Method<ClassType, ReturnType, void, void >	MethodType;

public:
	MethodType&
	name(std::string name)
	{
		m_name=name;
		return *this;
	}

	MethodType&
	function(FunctionType f)
	{
		m_functor=f;
		return *this;
	}

	ReturnType
	call(ClassType * instance)
	{
		return (ReturnType)m_functor(instance);
	}

private:
	FunctionType 	m_functor;
	std::string		m_name;
};

template <class ClassType, class ReturnType, class Param1>
class Method<ClassType, ReturnType, Param1, void>
{
	typedef boost::function<ReturnType (ClassType*, Param1)> 	FunctionType;
	typedef Method<ClassType, ReturnType, Param1, void >		MethodType;

public:
	MethodType&
	name(std::string name)
	{
		m_name=name;
		return *this;
	}

	MethodType&
	function(FunctionType f)
	{
		m_functor=f;
		return *this;
	}

	ReturnType
	call(ClassType * instance, Param1 p)
	{
		return (ReturnType)m_functor(instance,p);
	}

private:
	FunctionType 	m_functor;
	std::string		m_name;
};

//------------------------------------------------------------------------------
}; //namespace jrtti
#endif
