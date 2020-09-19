#ifndef _HEADER_GUARD_CPPFileIO_D1_
#define _HEADER_GUARD_CPPFileIO_D1_

////////////////////
// Headers BEGIN: //
////////////////////
#include "./Basic.hh"
//////////////////
// Headers END. //
//////////////////

#define _MACRO_CLASS_NAME_ Dynamic1DArray

	template <typename T>
	class _MACRO_CLASS_NAME_ {

	public:

		////////////////////////
		// DEFINITIONS BEGIN: //
		////////////////////////

		using TYPE_DATA = T ;

		using TYPE_SELF
		=	_MACRO_CLASS_NAME_
			<	TYPE_DATA
			>
		; //

		//////////////////////
		// DEFINITIONS END. //
		//////////////////////

	private:

		/////////////////
		// DATA BEGIN: //
		/////////////////

		TYPE_DATA * STORE ;
		size_t const SZ ;
		size_t const DS ;
		bool const allocated ;

		///////////////
		// DATA END. //
		///////////////

	public:

		//////////////////////////////
		// INTERFACE TO DATA BEGIN: //
		//////////////////////////////

		inline TYPE_DATA *
		GET_DATA ()
		{	return STORE ;
		}

		inline size_t
		DIST () const
		{	return DS ;
		}

		inline size_t
		SIZE () const
		{	return SZ ;
		}

		inline bool
		ALLOCATED() const
		{	return allocated ;
		}

		////////////////////////////
		// INTERFACE TO DATA END. //
		////////////////////////////

	private:

		///////////////////////
		// RETRIEVING BEGIN: //
		///////////////////////

		inline TYPE_DATA &
		GET_ELEMENT
		(	size_t const i
		) {	return STORE[i*DIST()] ;
		}

		inline TYPE_DATA const &
		GET_ELEMENT
		(	size_t const i
		) const
		{	return STORE[i*DIST()] ;
		}

		/////////////////////
		// RETRIEVING END. //
		/////////////////////

	public:

		////////////////////
		// SLICING BEGIN: //
		////////////////////

		inline TYPE_SELF
		CloneRange
		(	size_t const begin
		,	size_t const end
		) const {

			if (end<begin) {

				return
					CloneRange(end,begin)
				; //

			} else if (end<SIZE()) {

				size_t const NEW_SIZE
				=	end - begin + 1
				; //

				TYPE_SELF ret (NEW_SIZE) ;

				for
				(	size_t i=0
				;	i<NEW_SIZE
				;	i++
				) {	ret(i)
					=	GET_ELEMENT
						(	i + begin
						)
					; //
				}

				return ret ;

			} else {

				return
					CloneRange
					(	begin
					,	SIZE() - 1
					)
				;

			}

		}

		inline TYPE_SELF
		ViewRange
		(	size_t const begin
		,	size_t const end
		) {

			if (end<begin) {

				return
					ViewRange
					(	end
					,	begin
					)
				;

			} else if (end<SIZE()) {

				size_t const NEW_SIZE
				=	end - begin + 1
				;

				TYPE_SELF
					ret
					(	&(STORE[begin])
					,	NEW_SIZE
					,	DIST()
					)
				; //

				return ret ;

			} else {

				return
					ViewRange
					(	begin
					,	SIZE() - 1
					)
				; //

			}

		}

		//////////////////
		// SLICING END. //
		//////////////////

		////////////////////////
		// CONSTRUCTOR BEGIN: //
		////////////////////////

		_MACRO_CLASS_NAME_
		(	size_t const _SIZE
		) :	SZ(_SIZE)
		,	DS(1)
		,	allocated((SZ*DS)>0)
		{	if (allocated) {
				STORE
				=	new
						TYPE_DATA
						[	SIZE()
						*	DIST()
						]
				; //
			}
		}

		_MACRO_CLASS_NAME_
		(	TYPE_DATA * _STORE
		,	size_t const _SIZE
		,	size_t const _DIST = 1
		) :	SZ ( _SIZE )
		,	DS ( _DIST )
		,	allocated (false)
		{	STORE = _STORE ;
		}

		~_MACRO_CLASS_NAME_()
		{	if(allocated)
			{	delete [] STORE ;
			}
		}

		static inline TYPE_SELF
		CLONE
		(	TYPE_SELF const & in
		) {	TYPE_SELF ret(in.SIZE()) ;
			ret = in ;
			return ret ;
		}

		static inline TYPE_SELF
		CLONE
		(	std::vector <TYPE_DATA> const & in
		) {	TYPE_SELF ret(in.size()) ;
			ret = &(in[0]) ;
			return ret ;
		}

		//////////////////////
		// CONSTRUCTOR END. //
		//////////////////////

	public:

		///////////////////////
		// CONVINENCE BEGIN: //
		///////////////////////

		inline size_t
		operator () () const
		{	return SIZE() ;
		}

		inline TYPE_DATA &
		operator ()
		(	size_t const i
		) {	return
				GET_ELEMENT(i)
			; //
		}

		inline TYPE_DATA const &
		operator ()
		(	size_t const i
		) const {
			return
				GET_ELEMENT(i)
			; //
		}

		inline void
		operator =
		(	TYPE_SELF const & other
		) {	size_t I
			=	mymin
				(	SIZE()
				,	other()
				)
			; //

			for
			(	size_t i = 0
			;	i < I
			;	i++
			) {	GET_ELEMENT(i)
				=	other(i)
				; //
			}

		}

		inline void
		operator =
		(	TYPE_DATA const * in
		) {	for
			(	size_t i = 0
			;	i < SIZE()
			;	i++
			) {	GET_ELEMENT(i)
				=	in[i]
				; //
			}
		}

		/////////////////////
		// CONVINENCE END. //
		/////////////////////

	} ;

#undef _MACRO_CLASS_NAME_

#endif
