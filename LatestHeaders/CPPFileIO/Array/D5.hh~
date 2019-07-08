//////////////////////////////////////
#define _MACRO_TYPE_DATA_			\
									\
	N4D_ARRAY <						\
		S_4	,	S_3	,				\
		S_2	,	S_1	,				\
		TF							\
	>								\
									//
//////////////////////////////////////

	namespace Array /* 4 D Array */ {
		template <
			size_t	S_5	,	size_t	S_4	,
			size_t	S_3	,	size_t	S_2	,
			size_t	S_1	,	typename	TF
		> class N5D_ARRAY : public
		ND_ARRAY <
            S_5	,
            _MACRO_TYPE_DATA_
        > {
        public:

            using TYPE_DATA = TF ;

			using TYPE_ELEMENT =
				_MACRO_TYPE_DATA_
			;

			static inline size_t constexpr
            SIZE_5 () { return S_5 ; }

            static inline size_t constexpr
            SIZE_4 () { return S_4 ; }

            static inline size_t constexpr
            SIZE_3 () { return S_3 ; }

            static inline size_t constexpr
            SIZE_2 () { return S_2 ; }

            static inline size_t constexpr
            SIZE_1 () { return S_1 ; }

            static inline size_t constexpr
            SIZE () {
                return
                    SIZE_5()	*
                    SIZE_4()	*
                    SIZE_3()	*
                    SIZE_2()	*
                    SIZE_1()
                ; //
            }

            using TYPE_FLATTEN =
                ND_ARRAY <
                    SIZE(),
                    TYPE_DATA
                >
            ; //

			using TYPE_SELF =
				N5D_ARRAY <
					SIZE_5()  ,
					SIZE_4()  ,
					SIZE_3()  ,
					SIZE_2()  ,
					SIZE_1()  ,
					TYPE_DATA
				>
			; //

            inline TYPE_FLATTEN &
            FLATTEN() {
                return
                    reinterpret_cast
                        <TYPE_FLATTEN &>
                        (this[0])
                ; //
            }

            inline TYPE_FLATTEN const &
            FLATTEN() const {
                return
                    reinterpret_cast
                        <TYPE_FLATTEN const &>
                        (this[0])
                ; //
            }

            _MACRO_REGISTER_ALL_OPERATORS_

        } ;
    }

//////////////////////////////
#undef _MACRO_TYPE_DATA_	//
//////////////////////////////
