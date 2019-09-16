namespace NewHEPHeaders /* Fastjet easy jet cluster: */ {

	template <
		size_t 		N 				,
		typename	Tf		= float	,
		bool		boxify	= true
	> class BoxImageGen {
	public:
		//
		using TYPE_DATA =
			Tf
		; //
		using vector4  =
			fastjet::PseudoJet
		; //
		using vector4s =
			std::vector
				<fastjet::PseudoJet>
		; //
		//
		TYPE_DATA
			Image[N][N]
		; //
		//
		template <typename T>
		inline static T const
		mymod ( T const a ) {
			if (a<0) { return -a ; }
			else { return a ; }
		}
		//
		static inline double
		norm4 (
			vector4 const &
				a
		) {
			double ret =
				(a[3]*a[3]) -
				(a[2]*a[2]) -
				(a[1]*a[1]) -
				(a[0]*a[0])
			;
			return
				sqrt(ret)
			; //
		} //

		static inline double
		norm3 (
			vector4 const &
				a
		) {
			double ret =
				(a[2]*a[2]) +
				(a[1]*a[1]) +
				(a[0]*a[0])
			;
			return
				sqrt(ret)
			; //
		} //

		static inline double
		dot3 (
			vector4 const & a ,
			vector4 const & b
		) {
			return
				(a[0]*b[0]) +
				(a[1]*b[1]) +
				(a[2]*b[2])
			; //
		} //

		static inline double
		dot4 (
			vector4 const & a ,
			vector4 const & b
		) {
			return
				(a[3]*b[3]) -
				dot3(a,b)
			; //
		} //

		inline void
		ZeroImage () {
			for(size_t x=0;x<N;x++){
				for(size_t y=0;y<N;y++){
					Image[x][y] = 0.0 ;
				}
			}
		} //

		inline void
		Rescale2Box (
			double & X ,
			double & Y
		) {
			double ScalingFactor =
				(
					mymod(X) +
					mymod(Y)
				) /
				sqrt (
					(X*X) +
					(Y*Y)
				)
			;
			X *= ScalingFactor ;
			Y *= ScalingFactor ;
		} //

		inline vector4
		OrthoNormalize (
			vector4	&	V1	,
			vector4	&	V2	,
			vector4	&	V3
		) {
			vector4 ret =
				V1 + V2 + V3
			; /* Prepare the basis vectors: */ {
				vector4 B[3] =
					{ ret , V1 , V2 }
				; //
				/* The 1st basis vector: */ {
					B[0] /=
						norm3 (B[0])
					; //
					V1 = B[0] ;
				}
				/* The 2nd Basis Vector: */ {
					B[1] = B[1]
						- ( B[0] * dot3(B[0],B[1]) )
					; //
					B[1] /=
						norm3 (B[1])
					; //
					V2 = B[1] ;
				}
				/* The 3rd Basis Vector: */ {
					B[2] = B[2]
						- ( B[1] * dot3(B[2],B[1]) )
						- ( B[0] * dot3(B[2],B[0]) )
					; //
					B[2] /=
						norm3 (B[2])
					; //
					V3 = B[2] ;
				}
			}
			return
				ret
			; //
		}

		inline bool
		Eval (
			vector4s const &
				invectors  ,
			double const
				M0 = 0.5
		) {
			if(invectors.size()>2){

				ZeroImage();

				const double E0 =
					1.0
				; //
				const double P0 =
					sqrt(
						(E0*E0) -
						(M0*M0)
					)
				; //

				fastjet::JetAlgorithm algo =
					fastjet::kt_algorithm
				; //
				fastjet::JetDefinition
					jet_def (
						algo ,
						100.0
					)
				; //
				fastjet::ClusterSequence
					clust_seq (
						invectors ,
						jet_def
					)
				; //
				vector4s basis =
					fastjet::sorted_by_pt (
						clust_seq.exclusive_jets (3)
					)
				; //

				if(basis.size()>2){

					vector4
						eZ = basis[0] ,
						eX = basis[1] ,
						eY = basis[2]
					; //

					vector4 const P_MU_J =
						OrthoNormalize
							(eZ,eX,eY)
					;

					const double EJ =
						P_MU_J[3]
					; //
					const double PJ =
						norm3 (P_MU_J)
					; //
					const double MJ =
						norm4 (P_MU_J)
					; //
					const double Factor =
						(M0/MJ)
					; //

					double GM =
						( (EJ*E0) - (PJ*P0) ) /
						(MJ*M0)
					; //
					if(GM<1.0000000001){
						GM=1.0000000001;
					}

					double BT =
						sqrt(
							1.0 - (1.0/(GM*GM))
						)
					; //
					if ((EJ/MJ)<(E0/M0)) {
						BT = -BT ;
					}

					/* Orthonormalize the basis: */ if(false) {
						eZ /=
							norm3 (eZ)
						; //
						eX =
							basis[0] -
							(eZ*dot3(basis[0],eZ))
						; //
						eX /=
							norm3 (eX)
						; //
						eY =
							basis[1] -
							(eZ*dot3(basis[1],eZ)) -
							(eX*dot3(basis[1],eX))
						; //
						eY /=
							norm3 (eY)
						; //
					}

					if (false) {
						auto newtmp =
							sorted_by_E(invectors)
						; //
						eX =
							newtmp[0] -
							(eZ*dot3(newtmp[0],eZ))
						; //
						eX /=
							norm3 (eX)
						; //
						eY =
							newtmp[1] -
							(eZ*dot3(newtmp[1],eZ)) -
							(eX*dot3(newtmp[1],eX))
						; //
						eY /=
							norm3 (eY)
						; //
					}

					for(
						size_t i=0;
						i<invectors.size();
						i++
					) {

						vector4 tmp =
							invectors[i] *
							Factor
						; //

						double	cX	,	cY		;
						double	eI	=	tmp[3]	;

						/* Evaluate the components */ {
							double	const	pX	=
								dot3 (tmp,eX)
							;
							double	const	pY	=
								dot3 (tmp,eY)
							;
							double	const	pZ	=
								dot3 (tmp,eZ)
							;
							eI	=
								GM * ( eI - (BT*pZ) )
							;
							cX	=
								pX / eI
							;
							cY	=
								pY / eI
							;
						}

						/* Rescale to get a box: */ if (boxify) {
							Rescale2Box(cX,cY);
						}

						/* Rescale to get a box: */ if(false) {
							double ScalingFactor =
								(
									mymod(cX) +
									mymod(cY)
								) /
								sqrt (
									(cX*cX) +
									(cY*cY)
								)
							; //
							cX *= ScalingFactor ;
							cY *= ScalingFactor ;
						}

						/* Fill the Bins: */ {
							size_t N_X =
								static_cast<size_t>(
									static_cast<double>(N) *
									(cX+1.0) / 2.0
								)
							; //
							size_t N_Y =
								static_cast<size_t>(
									static_cast<double>(N) *
									(cY+1.0) / 2.0
								)
							;
							if(N_X>=N){N_X=N-1;}
							if(N_Y>=N){N_Y=N-1;}
							Image[N_Y][N_X] += eI ;
						}

					}
					return true;
				}
				else{return false;}
			}
			else{return false;}
		}
		//
		inline bool
		operator () (
			vector4s const &
				invectors  ,
			double const
				M0 = 0.5
		) {
			return
				Eval (
					invectors,
					M0
				)
			; //
		}
		//
		BoxImageGen (
			vector4s const &
				invectors  ,
			double const
				M0 = 0.5
		) {
			Eval(invectors,M0);
		}
		//
		BoxImageGen(){}
		//
		~BoxImageGen(){}
		//
	} ;

	template <
		size_t N ,
		typename Tf = float
	> class ImageGen {
	public:

		using vector4  =
			fastjet::PseudoJet
		; //

		using vector4s =
			std::vector
				<fastjet::PseudoJet>
		; //

		Tf Image[N][N] ;

		static inline double
		norm4 (
			vector4 const & a
		) {
			double ret =
				(a[3]*a[3]) -
				(a[2]*a[2]) -
				(a[1]*a[1]) -
				(a[0]*a[0])
			;
			return
				sqrt(ret)
			; //
		}

		static inline double
		norm3 (
			vector4 const & a
		) {
			double ret =
				(a[2]*a[2]) +
				(a[1]*a[1]) +
				(a[0]*a[0])
			;
			return
				sqrt(ret)
			; //
		}

		static inline double
		dot3 (
			vector4 const & a ,
			vector4 const & b
		) {
			return
				(a[0]*b[0]) +
				(a[1]*b[1]) +
				(a[2]*b[2])
			; //
		}

		static inline double
		dot4 (
			vector4 const & a ,
			vector4 const & b
		) {
			return
				(a[3]*b[3]) -
				dot3(a,b)
			; //
		}

		inline void ZeroImage () {
			for(size_t x=0;x<N;x++){
				for(size_t y=0;y<N;y++){
					Image[x][y] = 0.0 ;
				}
			}
		}

		inline bool Eval (
			vector4s const &
				invectors  ,
			double const
				M0 = 0.5
		) {
			if(invectors.size()>2){
				ZeroImage();
				const double E0 =
					1.0
				; //
				const double P0 =
					sqrt(
						(E0*E0) -
						(M0*M0)
					)
				; //
				fastjet::JetAlgorithm algo =
					fastjet::kt_algorithm
				; //
				fastjet::JetDefinition
					jet_def (
						algo ,
						100.0
					)
				; //
				fastjet::ClusterSequence
					clust_seq (
						invectors ,
						jet_def
					)
				; //
				vector4s basis =
					fastjet::sorted_by_pt (
						clust_seq.exclusive_jets (3)
					)
				; //
				if(basis.size()>2){
					vector4 eZ, eY, eX ;
					eZ =
						basis[0] +
						basis[1] +
						basis[2]
					; //
					const double EJ =
						eZ[3]
					; //
					const double PJ =
						norm3 (eZ)
					; //
					const double MJ =
						norm4 (eZ)
					; //
					const double Factor =
						(M0/MJ)
					; //
					double GM =
						( (EJ*E0) - (PJ*P0) ) /
						(MJ*M0)
					; //
					if(GM<1.0000000001){
						GM=1.0000000001;
					}
					double BT =
						sqrt(
							1.0 - (1.0/(GM*GM))
						)
					; //
					if ((EJ/MJ)<(E0/M0)) {
						BT = -BT ;
					}
					//
					eZ /=
						norm3 (eZ)
					; //
					//
					eX =
						basis[0] -
						(eZ*dot3(basis[0],eZ))
					; //
					eX /=
						norm3 (eX)
					; //
					//
					eY =
						basis[1] -
						(eZ*dot3(basis[1],eZ)) -
						(eX*dot3(basis[1],eX))
					; //
					eY /=
						norm3 (eY)
					; //
					//
					if (false) {
						auto newtmp =
							sorted_by_E(invectors)
						; //
						//
						eX =
							newtmp[0] -
							(eZ*dot3(newtmp[0],eZ))
						; //
						eX /=
							norm3 (eX)
						; //
						//
						eY =
							newtmp[1] -
							(eZ*dot3(newtmp[1],eZ)) -
							(eX*dot3(newtmp[1],eX))
						; //
						eY /=
							norm3 (eY)
						; //
					}
					//
					for(
						size_t i=0;
						i<invectors.size();
						i++
					) {
						//
						vector4 tmp =
							invectors[i] *
							Factor
						; //
						//
						double eI = tmp[3]        ;
						double pX = dot3 (tmp,eX) ;
						double pY = dot3 (tmp,eY) ;
						double pZ = dot3 (tmp,eZ) ;
						//
						eI = GM * ( eI - (BT*pZ) ) ;
						//
						double cX = pX / eI ;
						double cY = pY / eI ;
						//
						size_t N_X =
							static_cast<size_t>(
								static_cast<double>(N) *
								(cX+1.0) / 2.0
							)
						; //
						if(N_X>=N){N_X=N-1;}
						size_t N_Y =
							static_cast<size_t>(
								static_cast<double>(N) *
								(cY+1.0) / 2.0
							)
						;
						if(N_Y>=N){N_Y=N-1;}
						//
						Image[N_Y][N_X] += eI ;
						//
					}
					return true;
				}
				else{return false;}
			}
			else{return false;}
		}
		//
		inline bool operator () (
			vector4s const &
				invectors  ,
			double const
				M0 = 0.5
		) {
			return Eval(invectors,M0);
		}
		//
		ImageGen (
			vector4s const &
				invectors  ,
			double const
				M0 = 0.5
		) {
			Eval(invectors,M0);
		}
		//
		ImageGen(){}
		//
		~ImageGen(){}
		//
	} ;

	template <
		size_t N ,
		typename Tf = float
	> class ImageGenFlip {
	public:

		using vector4  =
			fastjet::PseudoJet
		; //

		using vector4s =
			std::vector
				<fastjet::PseudoJet>
		; //

		Tf Image[N][N] ;

		static inline double
		norm4 (
			vector4 const & a
		) {
			double ret =
				(a[3]*a[3]) -
				(a[2]*a[2]) -
				(a[1]*a[1]) -
				(a[0]*a[0])
			;
			return
				sqrt(ret)
			; //
		}

		static inline double
		norm3 (
			vector4 const & a
		) {
			double ret =
				(a[2]*a[2]) +
				(a[1]*a[1]) +
				(a[0]*a[0])
			;
			return
				sqrt(ret)
			; //
		}

		static inline double
		dot3 (
			vector4 const & a ,
			vector4 const & b
		) {
			return
				(a[0]*b[0]) +
				(a[1]*b[1]) +
				(a[2]*b[2])
			; //
		}

		static inline double
		dot4 (
			vector4 const & a ,
			vector4 const & b
		) {
			return
				(a[3]*b[3]) -
				dot3(a,b)
			; //
		}

		inline void ZeroImage () {
			for(size_t x=0;x<N;x++){
				for(size_t y=0;y<N;y++){
					Image[x][y] = 0.0 ;
				}
			}
		}

		inline bool Eval (
			vector4s const &
				invectors  ,
			double const
				M0 = 0.5
		) {
			if(invectors.size()>2){
				ZeroImage();
				const double E0 =
					1.0
				; //
				const double P0 =
					sqrt(
						(E0*E0) -
						(M0*M0)
					)
				; //
				fastjet::JetAlgorithm algo =
					fastjet::kt_algorithm
				; //
				fastjet::JetDefinition
					jet_def (
						algo ,
						100.0
					)
				; //
				fastjet::ClusterSequence
					clust_seq (
						invectors ,
						jet_def
					)
				; //
				vector4s basis =
					fastjet::sorted_by_pt (
						clust_seq.exclusive_jets (3)
					)
				; //
				if(basis.size()>2){
					vector4 eZ, eY, eX ;
					eZ =
						basis[0] +
						basis[1] +
						basis[2]
					; //
					const double EJ =
						eZ[3]
					; //
					const double PJ =
						norm3 (eZ)
					; //
					const double MJ =
						norm4 (eZ)
					; //
					const double Factor =
						(M0/MJ)
					; //
					double GM =
						( (EJ*E0) + (PJ*P0) ) /
						(MJ*M0)
					; //
					if(GM<1.0000000001){
						GM=1.0000000001;
					}
					double BT =
						sqrt(
							1.0 - (1.0/(GM*GM))
						)
					; //
					if ((EJ/MJ)<(E0/M0)) {
						BT = -BT ;
					}
					//
					eZ /=
						norm3 (eZ)
					; //
					//
					eX =
						basis[0] -
						(eZ*dot3(basis[0],eZ))
					; //
					eX /=
						norm3 (eX)
					; //
					//
					eY =
						basis[1] -
						(eZ*dot3(basis[1],eZ)) -
						(eX*dot3(basis[1],eX))
					; //
					eY /=
						norm3 (eY)
					; //
					//
					if (false) {
						auto newtmp =
							sorted_by_E(invectors)
						; //
						//
						eX =
							newtmp[0] -
							(eZ*dot3(newtmp[0],eZ))
						; //
						eX /=
							norm3 (eX)
						; //
						//
						eY =
							newtmp[1] -
							(eZ*dot3(newtmp[1],eZ)) -
							(eX*dot3(newtmp[1],eX))
						; //
						eY /=
							norm3 (eY)
						; //
					}
					//
					for(
						size_t i=0;
						i<invectors.size();
						i++
					) {
						//
						vector4 tmp =
							invectors[i] *
							Factor
						; //
						//
						double eI = tmp[3]        ;
						double pX = dot3 (tmp,eX) ;
						double pY = dot3 (tmp,eY) ;
						double pZ = dot3 (tmp,eZ) ;
						//
						eI = GM * ( eI - (BT*pZ) ) ;
						//
						double cX = pX / eI ;
						double cY = pY / eI ;
						//
						size_t N_X =
							static_cast<size_t>(
								static_cast<double>(N) *
								(cX+1.0) / 2.0
							)
						; //
						if(N_X>=N){N_X=N-1;}
						size_t N_Y =
							static_cast<size_t>(
								static_cast<double>(N) *
								(cY+1.0) / 2.0
							)
						;
						if(N_Y>=N){N_Y=N-1;}
						//
						Image[N_Y][N_X] += eI ;
						//
					}
					return true;
				}
				else{return false;}
			}
			else{return false;}
		}
		//
		inline bool operator () (
			vector4s const &
				invectors  ,
			double const
				M0 = 0.5
		) {
			return Eval(invectors,M0);
		}
		//
		ImageGenFlip (
			vector4s const &
				invectors  ,
			double const
				M0 = 0.5
		) {
			Eval(invectors,M0);
		}
		//
		ImageGenFlip(){}
		//
		~ImageGenFlip(){}
		//
	} ;

	template <
		size_t N,
		typename T
	> class MyJetImageGen {
	public:

		using TYPE_DATA = T ;

		using vector4 =
			VECTORS::lorentz4vector
				<TYPE_DATA>
		; //

		using vector3 =
			VECTORS::euclid3vector
				<TYPE_DATA>
		; //

		using vector4s =
			std::vector
				<vector4>
		; //

		using InputArrayType =
			Tensors::Array::N2D_ARRAY
				<N,N,TYPE_DATA>
		; //

		using FlatArrayType =
			Tensors::Array::ND_ARRAY <
				InputArrayType::SIZE(),
				TYPE_DATA
			>
		; //

		InputArrayType MainStore  ;
		vector4        FullJet    ;
		vector4        BoostedJet ;
		bool           eventpass  ;

	private:
		inline InputArrayType
		Process (
			fastjet::PseudoJet const &
				injet
		) {

			MainStore = 0.0   ;
			FullJet   = injet ;
			eventpass = false ;

			TYPE_DATA constexpr E_0 = 1.0 ;
			TYPE_DATA constexpr M_0 = 0.5 ;

			TYPE_DATA constexpr P_0 =
				sqrt((E_0*E_0)-(M_0*M_0))
			; //

			TYPE_DATA const
				E_J = injet.e()
			; //
			TYPE_DATA const
				M_J = injet.m()
			; //
			TYPE_DATA const
				P_J = FullJet.p ()
			; //

			TYPE_DATA const
				GAMMA =
					((E_J*E_0)-(P_J*P_0)) /
					(M_J*M_0)
			; //

			vector3 dir =
				FullJet.xyz.dir() * GAMMA
			; //
			if((E_J/M_J)<(E_0/M_0)){
				dir = dir.flip () ;
				printf("Found 1...\n");
			}

			pseudojets jetvectors =
				injet.constituents ()
			; //

			vector4s JetVectors ;
			JetVectors.resize
				(jetvectors.size())
			; //

//            vector4 trysum (0,0,0,0) ;
			for(size_t i=0;i<jetvectors.size();i++){
				vector4 tmp =
					jetvectors[i]
				; //
				tmp = tmp * (M_0/M_J) ;
				JetVectors[i] =
					tmp
					.LorentzBoostGamma(dir)
				; //
//                trysum =
//                    trysum +
//                    JetVectors[i]
//                ; //
			}
//            printf("DEBUG: E:%e M:%e\n",trysum[3],trysum.m());

			eventpass =
				(JetVectors.size()>2)
			; //

			if(eventpass){
				JetClusterExclusive
					jets (
						jetvectors,
						1,20,3
					)
				; //
				vector4 base [3] ;
				vector3 e    [3] ;
				base[0] = injet   ;
				base[1] = jets[0] ;
				base[2] = jets[1] ;
				for(size_t i=0;i<3;i++){
					base[i] =
						base[i] * (M_0/M_J)
					; //
					base[i] =
						base[i]
						.LorentzBoostGamma(dir)
					; //
					e[i] =
						base[i].xyz.dir()
					; //
				}
				e[1] = ( e[1] - (e[0]*(e[1]*e[0])) ).dir() ;
				e[2] = ( e[2] - (e[0]*(e[2]*e[0])) ).dir() ;
				e[2] = ( e[2] - (e[1]*(e[2]*e[1])) ).dir() ;
				for(size_t i=0;i<JetVectors.size();i++){
					TYPE_DATA X =
						JetVectors[i].xyz * e[1] /
						JetVectors[i][3]
					; //
					if ( X < -1.0 ) { X = -1.0 ; }
					if ( X >  1.0 ) { X =  1.0 ; }
					long IX =
						(long) N * ((X+1.0)/2.0)
					; //
					if ( IX <     0 ) { IX =     0 ; }
					if ( IX > N - 1 ) { IX = N - 1 ; }

					TYPE_DATA Y =
						JetVectors[i].xyz * e[2] /
						JetVectors[i][3]
					; //
					if ( Y < -1.0 ) { Y = -1.0 ; }
					if ( Y >  1.0 ) { Y =  1.0 ; }
					long IY =
						(long) N * ((Y+1.0)/2.0)
					; //
					if ( IY <     0 ) { IY =     0 ; }
					if ( IY > N - 1 ) { IY = N - 1 ; }

					MainStore[IY][IX] +=
						JetVectors[i][3]
					; //
				}
			}
			return MainStore ;
		}
	public:

		inline InputArrayType operator ()
		(fastjet::PseudoJet const & injet) {
			return Process(injet);
		}

		MyJetImageGen(){}
		~MyJetImageGen(){}

	} ;

}
