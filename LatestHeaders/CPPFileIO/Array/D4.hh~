    namespace Array /* 4 D Array */ {
        template <
            size_t	S_4	,	size_t S_3	,
            size_t	S_2	,	size_t S_1	,
            typename	TF
        > class N4D_ARRAY : public
        ND_ARRAY <
            S_4	,
            N3D_ARRAY <
                S_3,S_2,S_1,TF
            >
        > {
        public:

            using TYPE_DATA = TF ;

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
				N4D_ARRAY <
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
