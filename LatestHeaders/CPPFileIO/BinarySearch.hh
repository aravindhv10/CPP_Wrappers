namespace CPPFileIO {

#define _MACRO_CLASS_NAME_ BinarySearch

template <typename rd=char>
class _MACRO_CLASS_NAME_ {

public:

	using TYPE_READER = rd ;
	using TYPE_SELF = _MACRO_CLASS_NAME_ <rd> ;

private:

	TYPE_READER & reader ;
	size_t const limit ;

	using TYPE_RETURN =
		struct {
			size_t begin ;
			size_t end ;
			int status ;
		}
	; //

	template <typename T>
	inline TYPE_RETURN
	find (T const & in) {
		
		size_t begin = 0 ;
		size_t end = limit - 1 ;
		size_t mid ;
		int status = 0 ;
		TYPE_RETURN ret ;

		/* The main binary search loop: */ {

			start:

			mid = ( begin + end ) / 2 ;

			status =
				  ( 1  * ( in < reader(begin) ) )
				+ ( 2  * ( in == reader(begin) ) )
				+ ( 3  * ( ( in > reader(begin) ) && ( in < reader(mid) ) ) )
				+ ( 4  * ( in == reader(mid) ) )
				+ ( 5  * ( ( in > reader(mid) ) && ( in < reader(end) ) ) )
				+ ( 6  * ( in == reader(end) ) )
				+ ( 7  * ( in > reader(end) ) )
				+ ( 10 * ( end < (begin+2) ) )
			; //

			switch (status) {

				case 1:
				case 7:
					ret.begin = begin ;
					ret.end = end ;
					ret.status = -1 ;
					return ret ;

				case 2:
					ret.begin = begin ;
					ret.end = begin ;
					ret.status = 0 ;
					return ret ;

				case 3:
					end = mid ;
					goto start ;

				case 4:
					ret.begin = mid ;
					ret.end = mid ;
					ret.status = 0 ;
					return ret ;

				case 5:
					begin = mid ;
					goto start ;

				case 6:
					ret.begin = end ;
					ret.end = end ;
					ret.status = 0 ;
					return ret ;

				default:
					break ;

			}

			status =
				  ( 1 * ( in < reader(begin) ) )
				+ ( 2 * ( in == reader(begin) ) )
				+ ( 3 * ( ( in > reader(begin) ) && ( in < reader(end) ) ) )
				+ ( 4 * ( in == reader(end) ) )
				+ ( 5 * ( in > reader(end) ) )
			; //

			switch (status) {

				case 2:
					ret.begin = begin ;
					ret.end = begin ;
					ret.status = 0 ;
					return ret ;

				case 3:
					ret.begin = begin ;
					ret.end = end ;
					ret.status = 1 ;
					return ret ;

				case 4:
					ret.begin = end ;
					ret.end = end ;
					ret.status = 0 ;
					return ret ;

				case 1:
				case 5:
				default:
					ret.begin = begin ;
					ret.end = end ;
					ret.status = -1 ;
					return ret ;

			}

		}

	}

public:

	template <typename T>
	inline TYPE_RETURN
	operator () (T const & in) {
		return find(in) ;
	}

	_MACRO_CLASS_NAME_ (
		TYPE_READER & _reader
	) :	reader(_reader)
	,	limit(reader())
	{}

	~_MACRO_CLASS_NAME_ () {}

	template <typename T>
	static inline _MACRO_CLASS_NAME_ <T>
	GET (T & in) {
		_MACRO_CLASS_NAME_ <T> ret (in) ;
		return ret ;
	}

} ;

#undef _MACRO_CLASS_NAME_

}
