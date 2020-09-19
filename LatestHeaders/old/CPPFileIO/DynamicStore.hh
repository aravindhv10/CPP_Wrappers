namespace CPPFileIO {

#define _MACRO_TYPEDEF_ \
	using TYPE_ELEMENT = T ; \
	using TYPE_SELF = _MACRO_CLASS_NAME_ <TYPE_ELEMENT> ; \
	using TYPE_WRITTEN = DYNAMIC_ARRAYS::Dynamic1DArray <TYPE_ELEMENT> ; \
	using TYPE_READING = DYNAMIC_ARRAYS::Dynamic1DArray <TYPE_ELEMENT const> ;

#define _MACRO_FILENAMES_ \
	static inline std::string \
	indexname( \
		std::string const \
			_dirname \
	) { \
		mkdir(&(_dirname[0]),0755); \
		return (_dirname+"/index"); \
	} \
	static inline std::string \
	storename( \
		std::string const \
			_dirname \
	) { \
		mkdir(&(_dirname[0]),0755); \
		return (_dirname+"/store"); \
	}

#define _MACRO_CLASS_NAME_ DynamicStorageReader

template <typename T>
class _MACRO_CLASS_NAME_ {

public:

	_MACRO_TYPEDEF_

private:

	std::string const dirname ;
	FileArray <size_t const> reader_index ;
	FileArray <TYPE_ELEMENT const> reader_store ;

	_MACRO_FILENAMES_

	inline TYPE_READING
	read (
		size_t const i
	) {
		size_t const * indices
			= & ( reader_index(i,2) )
		; //
		size_t const length
			= indices[1] - indices[0]
		; //
		TYPE_ELEMENT const * buffer
			= & ( reader_store(indices[0],length) )
		; //
		TYPE_READING
			ret(buffer,length)
		; //
		return ret ;
	}

	inline size_t
	size () {
		return (
			reader_index() - 1
		) ;
	}

public:

	inline size_t
	operator () () {
		return size();
	}

	inline TYPE_READING
	operator () (size_t const i) {
		return read(i);
	}

	_MACRO_CLASS_NAME_(
		std::string const _dirname
	) :	dirname(_dirname)
	,	reader_index(indexname(dirname))
	,	reader_store(storename(dirname))
	{}

	~_MACRO_CLASS_NAME_(){}

} ;

#undef _MACRO_CLASS_NAME_

#define _MACRO_CLASS_NAME_ DynamicStorageWriter

template <typename T>
class _MACRO_CLASS_NAME_ {

public:

	_MACRO_TYPEDEF_

private:

	std::string const dirname ;
	FileWriter <size_t> writer_index ;
	FileWriter <TYPE_ELEMENT> writer_store ;

	_MACRO_FILENAMES_

	inline size_t
	write () {
		size_t const ret
			= writer_store()
		; //
		writer_index(ret);
		return ret ;
	}

	inline size_t
	write (
		TYPE_WRITTEN const &
			in
	) {
		size_t const ret
			= writer_store()
		; //
		for(size_t i=0;i<in();i++){
			writer_store(in(i));
		}
		writer_index(ret);
		return ret ;
	}

public:

	inline size_t
	operator () (
		TYPE_WRITTEN const &
			in
	) {
		return write(in);
	}

	_MACRO_CLASS_NAME_(
		std::string const _dirname
	) :	dirname(_dirname)
	,	writer_index(indexname(dirname))
	,	writer_store(storename(dirname))
	{}

	~_MACRO_CLASS_NAME_(){
		write();
	}

} ;

#undef _MACRO_CLASS_NAME_

#undef _MACRO_FILENAMES_
#undef _MACRO_TYPEDEF_

}
