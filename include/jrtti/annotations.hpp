#ifndef jrttiannotationsH
#define jrttiannotationsH


namespace jrtti {

/**
 * \brief Base class for annotations
 * \sa Annotations
 */
class Annotation {
public:
	virtual ~Annotation(){};
};

/**
 * \brief Provides a mechanism for custom annotations
 *
 * Custom annotations provide metadata to elements of jrtti. This class is a container
 * of Anotation instances wich is stored with the jrtti element. You can retrieve
 * the metadata container by calling annotations method of each jrtti element.
 * jrtti provides the standart Annotation NonStreamable to note that a property
 * is not streamable.
 * You can create your own annotations by creating a class derived from Annotation
 * See sample.h for an example of use
 */
class Annotations
{
public:
	typedef std::vector< boost::shared_ptr< Annotation > > Container;
	typedef Container::const_iterator iterator;

	/**
	 * \brief Adds an Annotations to the container
	 * \param annotation the instance of the annotation to add
	 * \return *this to chain calls
	 */
	Annotations &
	operator << ( Annotation * annotation ) {
		m_annotations.push_back( boost::shared_ptr< Annotation >( annotation ) );
		return *this;
	}

	/**
	 * \brief Get the first occurrence of annotation of type T
	 *
	 * \tparam T indicates the type of annotation to retrieve
	 * \return the first occurrence of annotation of type T
	 */
	template< typename T >
	T *
	getFirst() {
		for ( Container::iterator it = m_annotations.begin(); it != m_annotations.end(); ++it ) {
			T * p = dynamic_cast< T* >( it->get() );
			if ( p )
				return p;
		}
		return NULL;
	}

	/**
	 * \brief Get all instances of Annotations of type T
	 *
	 * \tparam T indicates the type of annotations to retrieve
	 * \return a vector with the instances of annotations of type T
	 */
	template< typename T >
	std::vector< T * >
	getAll() {
		std::vector< T * > v;
		for ( Container::iterator it = m_annotations.begin(); it != m_annotations.end(); ++it ) {
			T * p = dynamic_cast< T* >( it->get() );
			if ( p )
				v.push_back( p );
		}
		return v;
	}

	/**
	 * \brief Check for the existence of annotations of type T
	 *
	 * \tparam T indicates the type of annotations to check for
	 * \return true if an annotation of type T exists
	 */
	template< typename T >
	bool
	has() {
		return getFirst< T >() != NULL;
	}

private:
	Container m_annotations;
};

/**
 * \brief Annotation for non streamable properties
 *
 * A property with annotation NoStreamable will not be streamed or returned by
 * method Metatype::toStr with parameter formatForStreaming set to true
 */
class NoStreamable : public Annotation {
};

}; //namespace jrtti
#endif // jrttiannotationsH
