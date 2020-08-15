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
			char status ;
		}
	; //

	inline TYPE_RETURN
	FAILED () const {
		TYPE_RETURN ret ;
		ret.begin=0;
		ret.end=limit-1;
		ret.status = -1 ;
		return ret ;
	}

	inline TYPE_RETURN
	FOUND () const {
		return FAILED() ;
	}

	inline TYPE_RETURN
	FOUND (
		size_t const val
	) const {
		TYPE_RETURN ret ;
		ret.begin = val ;
		ret.end = val ;
		ret.status = 0 ;
		return ret ;
	}
	
	inline TYPE_RETURN
	FOUND (
		size_t const begin ,
		size_t const end
	) const {
		TYPE_RETURN ret ;
		ret.begin = begin ;
		ret.end = end ;
		ret.status = 1 ;
		return ret ;
	}

	template <typename T, typename F>
	inline TYPE_RETURN
	find (
		T const & in ,
		F & compare
	) {

		size_t begin = 0 ;
		size_t end = limit - 1 ;
		size_t mid ;
		int status ;

		if(end<begin){return FOUND();}

		char res_begin = compare(in,reader(begin)) ;
		char res_end   = compare(in,reader(end)) ;

		if(
			(res_begin<0)
			|| (res_end>0)
		) { return FOUND(); }

		/* Check if range is valid: */ {
			check_range:
			status =
				+ ( 1 * (end==begin)      ) 
				+ ( 2 * (end==(begin+1) ) )
				+ ( 3 * (end>(begin+1)  ) )
			; //
			switch(status) {
				case 1:
					if(res_begin==0)
					{return FOUND(begin) ;}
					else
					{return FOUND();}
				case 2:
					goto small_gap;
				case 3:
					goto large_gap;
			}
		}

		
		/* The main loop: */ {
			large_gap:
			mid = ( begin + end ) / 2 ;
			status =
				compare(in,reader(mid))
				+ ( 10 * ( end <  (begin+2)   ) )
			; //
			switch (status) {
				case -1:
					end = mid ;
					goto large_gap ;
				case 0:
				case 10:
					return FOUND(mid);
				case 1:
					begin = mid ;
					goto large_gap ;
				default:
					goto small_gap;
			}
		}

		/* Small Gap: */ {
			small_gap:
			res_begin = compare(in,reader(begin)) ;
			res_end   = compare(in,reader(end)) ;
			status =
				  ( 1 * ( res_begin == 0 ) )
				+ ( 2 * ( res_end == 0   ) )
				+ ( 3 * ( (res_begin>0) && (res_end<0) ) )
			; //
			switch (status) {
				case 1:
					return FOUND(begin);
				case 2:
					return FOUND(end) ;
				case 3:
					return FOUND(begin,end) ;
			}
		}

		/* Search Failed: */ {
			end:
			return FOUND() ;
		}

	}

	template <typename T>
	inline TYPE_RETURN
	find (T const & in) {

		size_t begin = 0 ;
		size_t end = limit - 1 ;
		size_t mid ;
		int status ;

		if(end<begin){return FOUND();}

		if(
			(in<reader(begin))
			|| (in>reader(end))
		) { return FOUND(); }

		/* Check if range is valid: */ {
			check_range:
			status =
				+ ( 1 * (end==begin)      ) 
				+ ( 2 * (end==(begin+1) ) )
				+ ( 3 * (end>(begin+1)  ) )
			; //
			switch(status) {
				case 1:
					if(in==reader(begin))
					{return FOUND(begin) ;}
					else
					{return FOUND();}
				case 2:
					goto small_gap;
				case 3:
					goto large_gap;
			}
		}

		
		/* The main loop: */ {
			large_gap:
			mid = ( begin + end ) / 2 ;
			status =
				+ ( -1 * ( in  <  reader(mid) ) )
				+ (  0 * ( in  == reader(mid) ) )
				+ (  1 * ( in  >  reader(mid) ) )
				+ ( 10 * ( end <  (begin+2)   ) )
			; //
			switch (status) {
				case -1:
					end = mid ;
					goto large_gap ;
				case 0:
				case 10:
					return FOUND(mid);
				case 1:
					begin = mid ;
					goto large_gap ;
				default:
					goto small_gap;
			}
		}

		/* Small Gap: */ {
			small_gap:
			status =
				+ ( 1  * ( in == reader(begin) ) )
				+ ( 2  * ( in == reader(end)   ) )
				+ ( 3  * ( ( in > reader(begin) ) && ( in < reader(end) ) ) )
			; //
			switch (status) {
				case 1:
					return FOUND(begin);
				case 2:
					return FOUND(end) ;
				case 3:
					return FOUND(begin,end) ;
			}
		}

		/* Search Failed: */ {
			end:
			return FOUND() ;
		}

	}

public:

	template <typename T>
	inline TYPE_RETURN
	operator () (T const & in) {
		return
			find(in)
		; //
	}

	template <typename T, typename F>
	inline TYPE_RETURN
	operator () (
		T const & in ,
		F & compare
	) {
		return
			find(in,compare)
		; //
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
