namespace CPPFileIO {

    class ForkMe {
        private:

            std::vector <pid_t> kids  ;
            bool                child ;
            size_t              kid_count ;

        public:

            inline bool
            WaitOnKids () {
                if (child) {
					exit(0);
					return false;
				} else if (kids.size()>0) {
					waitonall(kids);
					kids.clear();
					return true;
				} else {
					return false;
				}
            }

            ForkMe () {
				kids.clear();
				child = false;
				kid_count = 1;
			}

            ~ForkMe(){
				if (child) {
					exit (0) ;
				} else {
					WaitOnKids () ;
				}
			}

            inline bool
            InKid () {
                if (!child){
                    pid_t const tmp_pid = fork () ;
                    if (tmp_pid==0) {
						child = true;
						return true;
					} else {
						kids.push_back (tmp_pid) ;
						kid_count++ ;
						return false ;
					}
                } else {
					exit (0) ;
					return false ;
				}
            }

            inline void
            operator () (
				size_t _count=1
			) {
                if(kid_count>_count) { WaitOnKids () ; }
            }
    };

    template <bool DoFork=false>
	class ExternalStarter {
	public:

		using TYPE_SELF =
			ExternalStarter
				<DoFork>
		; //

		using TYPE_NAMES =
			std::vector <std::string>
		; //

		ExternalStarter (
			TYPE_NAMES const &
				in
		) {
			prog.reserve(in.size());
			for(size_t i=0;i<in.size();i++){
				prog.push_back(in[i]);
			}
		}

		inline TYPE_SELF &
		operator () (
			TYPE_NAMES const &
				in
		) {
			for(size_t i=0;i<in.size();i++){
				prog.push_back(in[i]);
			}
			return this[0] ;
		}

		ExternalStarter  (
			std::string const
				in
		) {
			prog.push_back(in);
		}

		inline TYPE_SELF &
		operator () (
			std::string const
				in
		) {
			prog.push_back(in);
			return this[0];
		}

        ~ExternalStarter () {
			if(DoFork){
				ForkMe forker ;
				if(forker.InKid()){
					starter_self(prog);
				}
			} else {
				starter_self(prog);
			}
		}

		static inline TYPE_SELF
		GET (
			std::string const
				in
		) {
			TYPE_SELF ret(in);
			return ret ;
		}

	private:
		TYPE_NAMES prog ; //
    };

}

