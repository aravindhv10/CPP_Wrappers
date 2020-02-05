
#define _MACRO_CLASS_NAME_ ClusterSquares

class _MACRO_CLASS_NAME_ {

private:

	TYPE_SQUARES const & in ;

	TYPE_SQUARES out ;

public:

	inline TYPE_DATA
	DIST (
		size_t const i ,
		size_t const j
	) const {
		if (
			out[i].IsValid() &&
			out[j].IsValid()
		) {
			return
				out[i](out[j])
			; //
		} else {
			return 1000000 ;
		}
	}

	using TYPE_PAIR =
		struct {
			long		I		;
			long		J		;
			TYPE_DATA	dist	;

			inline bool
			IsGood () {
				return
					(I>=0) &&
					(J>=0)
				; //
			}
		}
	; //

	inline TYPE_PAIR
	DIST () const {

		TYPE_PAIR
			ret = {
				-100,
				-100,
				1000000
			}
		; //

		for (size_t i=1;i<out.size();i++)	if (out[i].IsValid())
		for (size_t j=0;j<i;j++)			if (out[j].IsValid()) {
			auto const dist	= DIST(i,j)	;
			if (dist<ret.dist) {
				ret.I		= i			;
				ret.J		= j			;
				ret.dist	= dist		;
			}
		}

		return ret ;

	}

	inline bool
	DO_MERGE (
		TYPE_DATA const
			dist = 0.2
	) {
		bool ret = false ;

		auto const info = DIST () ;
		//printf("CAME HERE: %ld %ld\n",info.I,info.J);
		if (info.dist < dist) {
			auto bigsquare = out[info.I].MergeWith(out[info.J]) ;
			out[info.I].InValidate();
			out[info.J].InValidate();
			out.push_back(bigsquare);
			ret = true ;
			//printf("DEBUG: MERGED %ld %ld\n",info.I,info.J);
		}

		return ret ;
	}

	inline void
	MERGE_ALL (
		TYPE_DATA const
			dist = 0.2
	) {
		while(DO_MERGE(dist)){}
	}

	inline void
	CleanUp () {

		TYPE_SQUARES ret ;

		for (size_t i=0;i<out.size();i++)
		if (out[i].IsValid())
		{ ret.push_back(out[i]); }

		out = ret ;

	}

	inline TYPE_SQUARES const &
	operator () () {
		CleanUp () ;
		return out ;
	}

	_MACRO_CLASS_NAME_(
		TYPE_SQUARES const &
			_in
	) :
	in(_in) {out=in;}

	~_MACRO_CLASS_NAME_(){}

} ;

using TYPE_CLUSTER_SQUARES =
	_MACRO_CLASS_NAME_
; //

#undef _MACRO_CLASS_NAME_
