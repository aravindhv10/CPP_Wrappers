#define _MACRO_CLASS_NAME_ SymmetricMatrix

template <typename T>
class _MACRO_CLASS_NAME_ {

public:

	using TYPE_SELF = _MACRO_CLASS_NAME_ ;
	using TYPE_DATA = T ;

private:

	TYPE_DATA * data ;
	const size_t n ;

	inline size_t
	MakeMap (
		size_t const y ,
		size_t const x
	) const {
		return x + (y*(y+1)/2) ;
	}

	inline TYPE_DATA &
	Get (
		size_t const y,
		size_t const x
	) {
		return data[MakeMap(y,x)] ;
	}

	inline TYPE_DATA const
	Get (
		size_t const y,
		size_t const x
	) const {
		return data[MakeMap(y,x)] ;
	}

public:
	
	inline TYPE_DATA &
	operator () (
		size_t const y,
		size_t const x
	) {
		if(y>=x){
			return Get(y,x);
		} else {
			return Get(x,y);
		}
	}

	inline TYPE_DATA const
	operator () (
		size_t const y,
		size_t const x
	) const {
		if(y>=x){
			return Get(y,x);
		} else {
			return Get(x,y);
		}
	}

	inline size_t
	SIZE_Y () const {return n;}

	inline size_t
	SIZE_X () const {return n;}

	_MACRO_CLASS_NAME_(size_t const _n) : n(_n) {
		if(n>0){
			data = new TYPE_DATA[(n+1)*(n+2)/2] ;
		}
	}

	~_MACRO_CLASS_NAME_(){
		if(n>0) {
			delete [] data ;
		}
	}

} ;

#undef _MACRO_CLASS_NAME_
