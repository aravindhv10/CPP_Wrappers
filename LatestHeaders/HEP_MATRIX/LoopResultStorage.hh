	template <typename T>
	class LoopResultStorage {
	public:
		//
		using TYPE_DATA =
			T
		; //
		using TYPE_COMPLEX_DATA =
			std::complex <TYPE_DATA>
		; //
		using TYPE_RESULT_ELEMENT =
			Tensors::NN::ND_ARRAY
			<6,TYPE_COMPLEX_DATA>
		; //
		using TYPE_SELF =
			LoopResultStorage
			<TYPE_DATA>
		; //
		static inline size_t constexpr
		SIZE () {
			return
				TYPE_RESULT_ELEMENT::SIZE()
			; //
		} //
		static inline size_t const
		INTERPRET_EPSILON
		(long const i) {
			if((-2<=i)&&(i<=2)){
				return (i+2) ;
			} else {
				return 5 ;
			}
		}
	private:
		TYPE_RESULT_ELEMENT
			RESULT
		; //
		inline void
		READ_FROM (
			TYPE_COMPLEX_DATA const
				other
		) {
			RESULT =
				TYPE_COMPLEX_DATA(0,0)
			; //
			RESULT[INTERPRET_EPSILON(0)] =
				other
			; //
		} //
		inline void
		READ_FROM (
			TYPE_DATA const
				other = 0
		) {
			READ_FROM (
				TYPE_COMPLEX_DATA (
					other , 0
				)
			) ;
		} //
		inline void
		READ_FROM (
			TYPE_COMPLEX_DATA const other1 ,
			TYPE_COMPLEX_DATA const other2 ,
			TYPE_COMPLEX_DATA const other3 ,
			TYPE_COMPLEX_DATA const other4 ,
			TYPE_COMPLEX_DATA const other5
		) {
			READ_FROM (other3) ;
			RESULT[INTERPRET_EPSILON(-2)] = other1 ;
			RESULT[INTERPRET_EPSILON(-1)] = other2 ;
			RESULT[INTERPRET_EPSILON(1)]  = other4 ;
			RESULT[INTERPRET_EPSILON(2)]  = other5 ;
		} //
		inline void
		READ_FROM (
			TYPE_DATA const other1 ,
			TYPE_DATA const other2 ,
			TYPE_DATA const other3 ,
			TYPE_DATA const other4 ,
			TYPE_DATA const other5
		) {
			READ_FROM (
				TYPE_COMPLEX_DATA(other1,0),
				TYPE_COMPLEX_DATA(other2,0),
				TYPE_COMPLEX_DATA(other3,0),
				TYPE_COMPLEX_DATA(other4,0),
				TYPE_COMPLEX_DATA(other5,0)
			) ;
		} //
		inline TYPE_COMPLEX_DATA &
		GET_TERM (
			long const i
		) {
			return
				RESULT[
					INTERPRET_EPSILON(i)
				]
			; //
		} //
		inline TYPE_COMPLEX_DATA const
		GET_TERM (
			long const i
		) const {
			return
				RESULT[
					INTERPRET_EPSILON(i)
				]
			; //
		} //
	public:
		//
		inline TYPE_COMPLEX_DATA &
		operator [] (size_t const i)
		{ return RESULT[i] ; }
		//
		inline TYPE_COMPLEX_DATA const
		operator [] (size_t const i) const
		{ return RESULT[i] ; }
		//
		inline TYPE_COMPLEX_DATA &
		operator () (long const i) {
			return
				GET_TERM(i)
			; //
		} //
		inline TYPE_COMPLEX_DATA const
		operator () (long const i) const {
			return
				GET_TERM(i)
			; //
		} //
		inline void operator =
		(TYPE_SELF const & other) {
			RESULT =
				other.RESULT
			; //
		} //
		inline void operator =
		(TYPE_COMPLEX_DATA const other) {
			READ_FROM (other) ;
		} //
		inline void operator =
		(TYPE_DATA const other) {
			READ_FROM (other) ;
		} //
		//
//////////////////////////////////////////////////
#define _MACRO_DEFINE_SCALAR_OPERATOR_(SIGN)	\
	inline TYPE_SELF operator SIGN				\
	(TYPE_COMPLEX_DATA const other) const {		\
		TYPE_SELF ret (RESULT) ;				\
		ret(0) =								\
			ret(0) SIGN							\
			other								\
		;										\
		return ret ;							\
	}											\
	inline TYPE_SELF operator SIGN				\
	(TYPE_DATA const other) const {				\
		TYPE_SELF ret (RESULT) ;				\
		ret(0) =								\
			ret(0) SIGN							\
			TYPE_COMPLEX_DATA(other,0)			\
		;										\
		return ret ;							\
	}											//
//////////////////////////////////////////////////
#define _MACRO_DEFINE_OPERATOR_(SIGN)		\
	inline TYPE_SELF operator SIGN			\
	(TYPE_SELF const & other) const {		\
		TYPE_SELF							\
			ret (							\
				RESULT SIGN other.RESULT	\
			)								\
		;									\
		return ret ;						\
	}										\
	_MACRO_DEFINE_SCALAR_OPERATOR_(SIGN)	//
//////////////////////////////////////////////
_MACRO_DEFINE_OPERATOR_(+)					//
_MACRO_DEFINE_OPERATOR_(-)					//
#undef _MACRO_DEFINE_OPERATOR_				//
#undef _MACRO_DEFINE_SCALAR_OPERATOR_		//
//////////////////////////////////////////////
		//
//////////////////////////////////////////////////
#define _MACRO_DEFINE_SCALAR_OPERATOR_(SIGN)	\
		inline TYPE_SELF						\
		operator SIGN (							\
			TYPE_COMPLEX_DATA const				\
				other							\
		) const {								\
			TYPE_SELF							\
				ret (							\
					RESULT SIGN					\
					other						\
				)								\
			; 									\
			return								\
				ret								\
			;									\
		}										\
		inline TYPE_SELF						\
		operator SIGN (							\
			TYPE_DATA const						\
				other							\
		) const {								\
			TYPE_SELF							\
				ret (							\
					RESULT SIGN					\
					TYPE_COMPLEX_DATA(other,0)	\
				)								\
			;									\
			return								\
				ret								\
			;									\
		}										//
//////////////////////////////////////////////////
_MACRO_DEFINE_SCALAR_OPERATOR_(*)				//
_MACRO_DEFINE_SCALAR_OPERATOR_(/)				//
#undef _MACRO_DEFINE_SCALAR_OPERATOR_			//
//////////////////////////////////////////////////
		//
		inline TYPE_SELF
		operator * (
			TYPE_SELF const &
				other
		) const {
			TYPE_SELF
				ret
			; /* Evaluate the return value */ {
				for(long x=-2;x<=2;x++)
				for(long y=-2;y<=2;y++){
					long const z = x + y ;
					if((-2<=z)&&(z<=2)){
						ret(z) +=
							GET_TERM(x) *
							other(y)
						; //
					}
				}
			}
			return
				ret
			; //
		} //
		//
		template <typename TB>
		inline void DimExp (
			TYPE_COMPLEX_DATA	const	A	,
			TB					const	B	,
			TYPE_COMPLEX_DATA	const	C
		) {
			TYPE_COMPLEX_DATA const
				BA =
					std::pow(A,B)
			; //
			TYPE_COMPLEX_DATA const
				CA =
					std::log(A)*C
			; //
			TYPE_COMPLEX_DATA const
				BACA =
					BA * CA
			;
			RESULT =
				TYPE_COMPLEX_DATA(0,0)
			; //
			GET_TERM(0) =
				BA
			; //
			GET_TERM(1) =
				BACA
			; //
			GET_TERM(2) =
				BACA * CA /
				2.0
			; //
		} //
		//
		LoopResultStorage
		( TYPE_SELF const & other )
		{ RESULT = other.RESULT ; }
		//
		LoopResultStorage
		( TYPE_RESULT_ELEMENT const & other )
		{ RESULT = other ; }
		//
		LoopResultStorage
		( TYPE_COMPLEX_DATA const other )
		{ READ_FROM(other) ; }
		//
		LoopResultStorage
		( TYPE_DATA const other )
		{ READ_FROM(other) ; }
		//
		LoopResultStorage (
			TYPE_COMPLEX_DATA const other1 ,
			TYPE_COMPLEX_DATA const other2 ,
			TYPE_COMPLEX_DATA const other3 ,
			TYPE_COMPLEX_DATA const other4 ,
			TYPE_COMPLEX_DATA const other5
		) {
			READ_FROM (
				other1,other2,
				other3,other4,
				other5
			) ;
		}
		//
		LoopResultStorage (
			TYPE_DATA const other1 ,
			TYPE_DATA const other2 ,
			TYPE_DATA const other3 ,
			TYPE_DATA const other4 ,
			TYPE_DATA const other5
		) {
			READ_FROM (
				other1,other2,
				other3,other4,
				other5
			) ;
		}
		//
		LoopResultStorage () {
			READ_FROM
				(0,0,0,0,0)
			; //
		}
		//
		~LoopResultStorage(){}
		//
		template <typename TB>
		inline static TYPE_SELF
		DIM_EXP (
			TYPE_COMPLEX_DATA	const	A	,
			TB					const	B	,
			TYPE_COMPLEX_DATA	const	C
		) {
			TYPE_SELF ret ;
			ret.DimExp(A,B,C);
			return ret ;
		}
		template <typename TB>
		inline static TYPE_SELF
		DIM_EXP (
			TYPE_DATA	const	A	,
			TB			const	B	,
			TYPE_DATA	const	C
		) {
			return
				DIM_EXP (
					TYPE_COMPLEX_DATA	(A,0)	,
					B							,
					TYPE_COMPLEX_DATA	(C,0)
				)
			; //
		}
		//
	} ;
