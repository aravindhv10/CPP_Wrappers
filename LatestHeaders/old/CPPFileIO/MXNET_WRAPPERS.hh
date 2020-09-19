#ifdef MXNET_CPP_MXNETCPP_H_

namespace CPP_MXNET {

	using namespace mxnet::cpp ;

	inline void
	WRITE (
		NDArray const		& in		,
		CPPFileIO::FileFD	& filefd
	) {

		filefd.multiwrite2file(
			in.GetData()[0] ,
			in.Size()
		) ; //

	}

	inline void
	READ (
		NDArray				& in		,
		CPPFileIO::FileFD	& filefd
	) {

		using TYPE_DATA =
			float
		;

		size_t const
			bufsize =
				in.Size()
		; //

		std::vector <TYPE_DATA>
			buf (bufsize)
		; //

		filefd.multiread2file (
			buf[0]	,
			bufsize
		) ; //

		in.SyncCopyFromCPU (
			&(buf[0])	,
			bufsize
		) ; //

		in.WaitToRead() ;

	}

	inline void
	WRITE (

		std::map <
			std::string ,
			NDArray
		> & args_map ,

		std::string
			dirname

	) {

		mkdir (

			static_cast <const char *>
				(&(dirname[0])) ,

			static_cast <mode_t>
				(0755)

		) ;

		for (
			auto & x :
				args_map
		) {

			std::string filename =
				dirname + "/" + x.first
			; //

			CPPFileIO::FileFD
				writer (
					&(filename[0])
				)
			; //

			writer.writefile () ;

			WRITE(x.second,writer);

		}

	}

	inline void
	READ (

		std::map <
			std::string ,
			NDArray
		> & args_map ,

		std::string
			dirname

	) {

		mkdir (

			static_cast <const char *>
				(&(dirname[0])) ,

			static_cast <mode_t>
				(0755)

		) ;

		for (
			auto & x :
				args_map
		) {

			std::string filename =
				dirname + "/" + x.first
			; //

			CPPFileIO::FileFD
				reader (
					&(filename[0])
				)
			; //

			reader.readfile () ;

			READ(x.second,reader);

		}

	}


}

#endif
