namespace NewHEPHeaders /* Fastjet easy jet cluster: */ {

	namespace JET_ALGO {
		long KT  =  1 ;
		long CA  =  0 ;
		long AKT = -1 ;
	}

	class JetClusterInclusive : public pseudojets {
	public:
		typedef JetClusterInclusive TYPE_Self   ;
		typedef pseudojets          TYPE_Parent ;
	private:
		fastjet::JetAlgorithm      algo      ;
		pseudojets               & invectors ;
		fastjet::JetDefinition   * jet_def   ;
		fastjet::ClusterSequence * clust_seq ;
	public:
		JetClusterInclusive
		(pseudojets & _invectors, long _algo=-1, double const _Pt=20, double const _R=0.4) :
		invectors(_invectors) {
			TYPE_Parent & SELF = this[0] ;
			if      ( _algo == -1 ) { algo = fastjet::antikt_algorithm    ; }
			else if ( _algo ==  0 ) { algo = fastjet::cambridge_algorithm ; }
			else if ( _algo ==  1 ) { algo = fastjet::kt_algorithm        ; }
			jet_def   = new fastjet::JetDefinition   (      algo ,         _R ) ;
			clust_seq = new fastjet::ClusterSequence ( invectors , jet_def[0] ) ;
			SELF      = sorted_by_pt ( clust_seq->inclusive_jets (_Pt) ) ;
		}
		~JetClusterInclusive () { delete jet_def ; delete clust_seq ; }
	} ;

	class JetClusterExclusive : public pseudojets {
	public:
		typedef JetClusterInclusive TYPE_Self   ;
		typedef pseudojets          TYPE_Parent ;
	private:
		fastjet::JetAlgorithm      algo      ;
		pseudojets const         & invectors ;
		fastjet::JetDefinition   * jet_def   ;
		fastjet::ClusterSequence * clust_seq ;
	public:
		JetClusterExclusive
		(pseudojets const & _invectors, long _algo=-1, double _Pt=20, int const _N=1) :
		invectors(_invectors) {
			TYPE_Parent & SELF = this[0] ;
			if      ( _algo == -1 ) { algo = fastjet::antikt_algorithm    ; }
			else if ( _algo ==  0 ) { algo = fastjet::cambridge_algorithm ; }
			else if ( _algo ==  1 ) { algo = fastjet::kt_algorithm        ; }
			jet_def   = new fastjet::JetDefinition   (      algo ,      100.0 ) ;
			clust_seq = new fastjet::ClusterSequence ( invectors , jet_def[0] ) ;
			SELF      = fastjet::sorted_by_pt ( clust_seq->exclusive_jets (_N) ) ;
		}
		~JetClusterExclusive () { delete jet_def ; delete clust_seq ; }
	} ;

}
