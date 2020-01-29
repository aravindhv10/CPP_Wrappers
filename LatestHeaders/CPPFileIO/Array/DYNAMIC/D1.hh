namespace DYNAMIC_ARRAYS {

	template <typename T>
	class Dynamic1DArray {
	public:

		using TYPE_DATA = T ;

		using TYPE_SELF =
			Dynamic1DArray<TYPE_DATA>
		; //

		inline size_t const
		operator () () const {
			return SIZE ;
		}

		inline T &
		operator () (
			size_t const
				i
		) {
			return
				STORE[i]
			; //
		}

		inline T const &
		operator () (
			size_t const
				i
		) const {
			return
				STORE[i]
			; //
		}

		inline TYPE_SELF
		CloneRange (
			size_t const begin ,
			size_t const end
		) const {

			if (end<begin) {

				return
					CloneRange(end,begin)
				; //

			} else if (end<SIZE) {

				size_t const
					NEW_SIZE =
						end - begin + 1
				; //

				TYPE_SELF
					ret (NEW_SIZE)
				; //

				for (
					size_t i=0;
					i<NEW_SIZE;
					i++
				) {
					ret(i) =
						STORE[i+begin]
					; //
				}

				return ret ;

			} else {

				return
					CloneRange(begin,SIZE-1)
				; //

			}

		}

		inline TYPE_SELF
		ViewRange (
			size_t const begin ,
			size_t const end
		) {

			if (end<begin) {

				return
					ViewRange(
						end	,
						begin
					)
				; //

			} else if (end<SIZE) {

				size_t const
					NEW_SIZE =
						end - begin + 1
				; //

				TYPE_SELF
					ret (
						&(STORE[begin]) ,
						NEW_SIZE
					)
				; //

				return ret ;

			} else {

				return
					ViewRange (
						begin	,
						SIZE - 1
					)
				; //

			}

		}

		inline void
		operator = (
			TYPE_SELF const &
				other
		) {

			size_t I =
				CPPFileIO::mymin (
					SIZE	,
					other()
				)
			; //

			for (size_t i=0;i<I;i++)
			{ STORE[i]=other(i); }

		}

		Dynamic1DArray (
			size_t const
				_SIZE
		) : SIZE(_SIZE) ,
		allocated(SIZE>0) {

			if (allocated) {
				STORE =
					new TYPE_DATA[SIZE]
				; //
			}

		}

		Dynamic1DArray (
			TYPE_DATA * _STORE ,
			size_t const _SIZE
		) : SIZE(_SIZE) ,
		allocated(false)
		{ STORE = _STORE ; }

		~Dynamic1DArray(){
			if(allocated){
				delete [] STORE ;
			}
		}

	private:

		TYPE_DATA * STORE ;
		size_t const SIZE ;
		bool const allocated ;

	} ;
}
