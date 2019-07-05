////////////////////////////////////////////////////////////////
namespace NoBoost {

    template <
        size_t		N				,
        typename	Tf		= float	,
		bool		boxify	= true
    > class BoxImageGen {
    public:
        //
        using TYPE_DATA =
            Tf
        ;
        //
        using vector4  =
            fastjet::PseudoJet
        ; //
        //
        using vector4s =
            std::vector
                <fastjet::PseudoJet>
        ; //
        //
        TYPE_DATA Image[N][N] ;
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
        }
        //
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
        }
        //
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
        //
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
        //
        inline void ZeroImage () {
            for(size_t x=0;x<N;x++){
                for(size_t y=0;y<N;y++){
                    Image[x][y] = 0.0 ;
                }
            }
        }
        //
        inline void
        Rescale2Box (
            double & X ,
            double & Y
        ) {
            double ScalingFactor =
                (
                    CPPFileIO::mymod(X) +
                    CPPFileIO::mymod(Y)
                ) /
                sqrt (
                    (X*X) +
                    (Y*Y)
                )
            ;
            X *= ScalingFactor ;
            Y *= ScalingFactor ;
        }
        //
        inline bool Eval (
            vector4s	const	& 	invectors
        ) {
            if(invectors.size()>2){
                ZeroImage();
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
                        eZ, eY, eX
                    ; //
                    eZ =
                        basis[0] +
                        basis[1] +
                        basis[2]
                    ; //
                    const double EJ =
                        eZ[3]
                    ; /* Orthonormalize the basis: */ {
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
                    for(
                        size_t i=0;
                        i<invectors.size();
                        i++
                    ) {
                        double
                            cX	,	cY	,	eI
                        ; /* Evaluate the values: */ {
                            vector4 tmp =
                                invectors[i]
                            ; //
                            eI = tmp[3] ;
                            /* Evaluate the projections: */ {
                                double pX =
                                    dot3 (tmp,eX)
                                ; //
                                double pY =
                                    dot3 (tmp,eY)
                                ; //
                                cX = pX / eI ;
                                cY = pY / eI ;
                            }
                            if (boxify) /* Rescale to get a box: */ {
                                Rescale2Box(cX,cY);
                            }
                        } /* Evaluate the bins: */ {
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
                            ; //
                            if(N_X>=N){N_X=N-1;}
                            if(N_Y>=N){N_Y=N-1;}
                            Image[N_Y][N_X] +=
                                eI / EJ
                            ; //
                        }
                    }

                    if(false){
                        double sum = 0 ;
                        for(size_t x=0;x<N;x++)
                        for(size_t y=0;y<N;y++){
                            sum += Image[y][x] ;
                        }
                        printf("The Norm of the image: %e\n",sum);
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
                invectors
        ) {
            return
                Eval (
                    invectors
                )
            ; //
        }
        //
        BoxImageGen (
            vector4s const &
                invectors
        ) {
            Eval(invectors);
        }
        //
        BoxImageGen(){}
        //
        ~BoxImageGen(){}
        //
    } ;

}
////////////////////////////////////////////////////////////////
namespace NoGramSchmidt {

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
                    CPPFileIO::mymod(X) +
                    CPPFileIO::mymod(Y)
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
                vector4 X (1,0,0,1) ;
                vector4 Y (0,1,0,1) ;
                vector4 B[3] =
                    { ret , X , Y }
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
                                    CPPFileIO::mymod(cX) +
                                    CPPFileIO::mymod(cY)
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

}
////////////////////////////////////////////////////////////////
namespace NoGramSchmidt_BAD {

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
                    CPPFileIO::mymod(X) +
                    CPPFileIO::mymod(Y)
                ) /
                sqrt (
                    (X*X) +
                    (Y*Y)
                )
            ; //

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
                vector4 X (1,0,0,1) ;
                vector4 Y (0,1,0,1) ;
                vector4 B[3] =
                    { ret , X , Y }
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
                    ; //

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
					; /* for numerical safety: */ {
						if(GM<1.0000000001){
							GM=1.0000000001;
						}
					}

					double BT =
						sqrt (
							1.0 -
							(1.0/(GM*GM))
						)
					; /* flip beta for the case: */ {
						if ((EJ/MJ)<(E0/M0)) {
							BT = -BT ;
						}
					}

					double
						PJ_X	,
						PJ_Y	,
						PJ_Z	,
						PJ_T
					; /* Evaluate the components: */ {

						double	const	eI	=
							P_MU_J[3] * Factor
						; //

						double	const	pX	=
							dot3 (P_MU_J,eX) * Factor
						; //

						double	const	pY	=
							dot3 (P_MU_J,eY) * Factor
						; //

						double	const	pZ	=
							dot3 (P_MU_J,eZ) * Factor
						; //

						PJ_X =
							pX
						; //

						PJ_Y =
							pY
						; //

						PJ_T =
							GM * (
								eI - (BT*pZ)
							)
						; //

						PJ_Z =
							GM * (
								pZ - (BT*eI)
							)
						; //

					}

					vector4 const
						BoostedJet (
							PJ_X	,	PJ_Y	,
							PJ_Z	,	PJ_T
						)
					; //

					for (
						size_t i=0;
						i<invectors.size();
						i++
					) {

						vector4 tmp =
							invectors[i]	*
							Factor
						; //

						vector4
							Boostedtmp
						; //

						double
							cX	,	cY	,
							cZ	,	cI
						;  /* Evaluate the components */ {

							double	const	eI	=
								tmp[3]
							; //

							double	const	pX	=
								dot3 (tmp,eX)
							; //

							double	const	pY	=
								dot3 (tmp,eY)
							; //

							double	const	pZ	=
								dot3 (tmp,eZ)
							; //

							cI	=
								GM * (
									eI - (BT*pZ)
								)
							; //

							cZ	=
								GM * (
									pZ - (BT*eI)
								)
							; //

							cX	=
								pX
							; //

							cY	=
								pY
							; //

							Boostedtmp =
								vector4 (
									cX	,	cY	,
									cZ	,	cI
								)
							; //

						}

						double

							DeltaEta =
								BoostedJet.rap() -
								Boostedtmp.rap()

								,

							DeltaPhi =
								BoostedJet.delta_phi_to
									(Boostedtmp)

						; //

						//printf("DEBUG: JET _ENERGY = %e\n",BoostedJet[3]);

						/* Rescale to get a box: */ {
							if (boxify) {
								printf (
									"Boxify here makes absolutely 0 sense...\n"
								) ;
							}
						}

						/* Fill the Bins: */ {

							size_t N_ETA =
								static_cast<size_t>(
									static_cast<double>(N) * (
										( DeltaEta - (-1.0) ) /
										( 1.2 )
									)
								)
							; //

							size_t N_PHI =
								static_cast<size_t>(
									static_cast<double>(N) * (
										( DeltaPhi - (-1.0) ) /
										( 1.2 )
									)
								)
							; //

							/* Add the energies: */ {
								if(N_ETA>=N){N_ETA=N-1;}
								if(N_PHI>=N){N_PHI=N-1;}
								Image[N_ETA][N_PHI] += cI ;
							} //

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

			Eval (
				invectors,
				M0
			) ;

        }
        //
        BoxImageGen(){}
        //
        ~BoxImageGen(){}
        //
    } ;

}
////////////////////////////////////////////////////////////////
