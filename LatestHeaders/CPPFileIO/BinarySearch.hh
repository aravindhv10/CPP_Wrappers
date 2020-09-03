namespace CPPFileIO {

#define _MACRO_CLASS_NAME_ BinarySearch

template <typename rd=char>
class _MACRO_CLASS_NAME_ {

////////////////////////
// DEFINITIONS BEGIN: //
////////////////////////

public:

	using TYPE_READER = rd ;
	using TYPE_SELF = _MACRO_CLASS_NAME_ <rd> ;

	using TYPE_RETURN =
		struct {
			size_t begin ;
			size_t end ;
			char status ;
		}
	; //

//////////////////////
// DEFINITIONS END. //
//////////////////////


/////////////////
// DATA BEGIN: //
/////////////////

private:

	TYPE_READER & reader ;
	size_t const limit ;

///////////////
// DATA END. //
///////////////

//////////////////////////
// RETURN STATUS BEGIN: //
//////////////////////////

private:

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

////////////////////////
// RETURN STATUS END. //
////////////////////////

//////////////////
// RANGE BEGIN: //
//////////////////

private:

	template <typename F>
	inline size_t
	find_start (
		size_t end
		, F & in
	) {

		size_t start = 0 ;
		if(start>=end){return start;}
		auto const val=reader(end);
		char cmp=in(reader(start),val);
		if(cmp>=0){return start;}

		MainLoop: {

			size_t const mid
				= (start+end)/2
			; //

			int const stat
				= ( 10 * ( start >= mid ) )
				+ in(reader(mid),val)
			; //

			switch (stat) {

				case  9 :
				case 11 : return end;

				case 10 : return start;

				case -1 :
					start=mid;
					goto MainLoop ;

				case  0 :
					end=mid;
					goto MainLoop ;

				case  1 : return end ;

				default : break;

			}

		}
		return end;

		for(
			size_t mid=(start+end)/2;
			start<mid;
			mid=(start+end)/2
		) {
			cmp=in(reader(mid),val);
			if(cmp>=0){end=mid;}
			else{start=mid;}
		}
		return end;

	}

	inline size_t
	find_start (
		size_t end
	) {

		size_t start = 0 ;
		if(start>=end){return start;}
		auto const val=reader(end);
		if(reader(start)>=val){return start;}

		MainLoop: {

			size_t const mid
				= (start+end)/2
			; //

			int const stat
				= ( 10 * ( start       >= mid ) )
				+ (  1 * ( reader(mid) >= val ) )
			; //

			switch (stat) {

				case 10: return end;

				case 11: return start;

				case 1:
					end=mid;
					goto MainLoop ;

				default:
					start=mid;
					goto MainLoop ;

			}

		}
		return end;

		for(
			size_t mid=(start+end)/2;
			start<mid;
			mid=(start+end)/2
		) {
			if(reader(mid)>=val){end=mid;}
			else{start=mid;}
		}
		return end;

	}

	template <typename F>
	inline size_t
	find_end (
		size_t start
		, F & in
	) {
		size_t end=reader()-1;
		if(start>=end){return end;}
		auto const val=reader(start);
		char cmp=in(reader(end),val);
		if(cmp<=0){return end;}

		MainLoop: {

			size_t const mid
				= (start+end)/2
			; //

			int const stat
				= ( 10 * ( start >= mid ) )
				+ in(reader(mid),val)
			; //

			switch (stat) {
				case 9:
				case 11:
				case 10:
				case -1:
					return start;

				case 0:
					start=mid;
					goto MainLoop;

				case 1:
					end=mid;
					goto MainLoop;

			}

		}
		return start;

		for(
			size_t mid=(start+end)/2;
			start<mid;
			mid=(start+end)/2
		) {
			cmp=in(reader(mid),val);
			if(cmp<=0){start=mid;}
			else{end=mid;}
		}
		return start;
	}

	inline size_t
	find_end (
		size_t start
	) {

		size_t end=reader()-1;
		if(start>=end){return end;}
		auto const val=reader(start);
		if(reader(end)<=val){return end;}

		MainLoop: {

			size_t const mid
				= (start+end)/2
			; //

			int const stat
				= ( 10 * ( start       >= mid ) )
				+ (  1 * ( reader(mid) <= val ) )
			; //

			switch (stat) {
				case 10:
				case 11:
					return start;

				case 1:
					start=mid;
					goto MainLoop ;

				default:
					end=mid;
					goto MainLoop ;

			}

		}
		return end;

		for(
			size_t mid=(start+end)/2;
			start<mid;
			mid=(start+end)/2
		) {
			if(reader(mid)<=val){start=mid;}
			else{end=mid;}
		}
		return start;

	}

public:

	inline TYPE_RETURN
	find_range
	(	size_t const index
	) {	return
			FOUND
			(	find_start(index)
			,	find_end(index)
			)
	; }

	template <typename T>
	inline TYPE_RETURN
	find_range
	(	size_t const index
	,	T & in
	) {	return
			FOUND
			(	find_start(index,in)
			,	find_end(index,in)
			)
	; }

////////////////
// RANGE END. //
////////////////

///////////////////////////////
// FIND WITH COMPARER BEGIN: //
///////////////////////////////

private:

	template <typename T, typename F>
	inline TYPE_RETURN
	find_new (
		T const & in
		, F & compare
	) {

		size_t begin = 0 ;
		size_t end = limit - 1 ;
		size_t mid ;
		int status ;

		status =
			  ( 1 * ( end < begin ) )
			+ ( 2 * ( end == begin ) )
		; //

		switch (status) {

			case 1 : return FOUND() ;

			case 2 : {
				int const res
					= compare(in,reader(begin))
				; //
				if(res==0){
					return FOUND(begin) ;
				} else {
					return FOUND() ;
				}
			}

			default : break ;

		}

		MainLoopBegin: {

			mid = ( begin + end ) / 2 ;

			status =
				compare(in,reader(mid))
				+ ( 10 * ( begin == mid ) )
			; //

			switch (status) {

				case -1 :
					end = mid ;
					goto MainLoopBegin ;

				case 1 :
					begin = mid ;
					goto MainLoopBegin ;

				case 0 :
				case 10 : return FOUND(mid) ;

				case 11 : return FOUND(mid,end) ;

				default : return FOUND() ;

			}

		}

	}

	template <typename T, typename F>
	inline TYPE_RETURN
	find_old (
		T const & in
		, F & compare
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
				case 1 : return FOUND(begin);
				case 2 : goto small_gap;
				case 3 : goto large_gap;
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
			; //
			switch (status) {
				case 1  : return FOUND(begin) ;
				case 2  : return FOUND(end) ;
				case 0  : return FOUND(begin,end) ;
				default : break ;
			}
		}

		/* Search Failed: */ {
			end:
			return FOUND() ;
		}

	}

/////////////////////////////
// FIND WITH COMPARER END. //
/////////////////////////////

///////////////////////
// Plain find BEGIN: //
///////////////////////

private:

	template <typename T>
	inline TYPE_RETURN
	find_new (
		T const & in
	) {
		size_t begin = 0 ;
		size_t end = limit - 1 ;
		size_t mid ;
		int status ;

		status =
			  ( 1 * ( end < begin ) )
			+ ( 2 * ( end == begin ) )
		; //

		switch (status) {

			case 1 : return FOUND() ;

			case 2 : {
				if(in==reader(begin)){
					return FOUND(begin) ;
				} else {
					return FOUND() ;
				}
			}

			default : break ;

		}

		MainLoopBegin: {

			mid = ( begin + end ) / 2 ;

			status =
				  ( -1 * ( in < reader(mid) ) )
				+ (  1 * ( in > reader(mid) ) )
				+ ( 10 * ( begin == mid ) )
			; //

			switch (status) {
				case -1 :
					end = mid ;
					goto MainLoopBegin ;

				case 1 :
					begin = mid ;
					goto MainLoopBegin ;

				case 0 :
				case 10 : return FOUND(mid) ;

				case 11 : return FOUND(mid,end) ;

				default : return FOUND() ;

			}

		}


	}

	template <typename T>
	inline TYPE_RETURN
	find_old (
		T const & in
	) {

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
				case 1 : return FOUND(begin);
				case 2 : goto small_gap;
				case 3 : goto large_gap;
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
				case -1 :
					end = mid ;
					goto large_gap ;
				case 0  :
				case 10 :
					return FOUND(mid);
				case 1  :
					begin = mid ;
					goto large_gap ;
				default :
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
				case 1 : return FOUND(begin);
				case 2 : return FOUND(end);
				case 3 : return FOUND(begin,end);
			}
		}

		/* Search Failed: */ {
			end:
			return FOUND();
		}

	}

/////////////////////
// Plain find END. //
/////////////////////

public:

	template <typename T>
	inline TYPE_RETURN
	operator () (
		T const & in
	) {
		return
			find_new(in)
		; //
	}

	template <typename T, typename F>
	inline TYPE_RETURN
	operator () (
		T const & in
		, F & compare
	) {
		return
			find_new(in,compare)
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
