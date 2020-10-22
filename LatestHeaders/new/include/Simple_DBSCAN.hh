#ifndef _HEADER_GUARD_Simple_DBSCAN_
#define _HEADER_GUARD_Simple_DBSCAN_

/////////////////////
// Headers BEGIN:{ //
/////////////////////
#include "./CPPFileIO.hh"
///////////////////
// Headers END.} //
///////////////////

#define _MACRO_CLASS_NAME_ Simple_DBSCAN

template <typename TF = double, typename TI = long> class _MACRO_CLASS_NAME_ {
  public:
    using TYPE_FLOAT     = TF;
    using TYPE_INT       = TI;
    using TYPE_SELF      = _MACRO_CLASS_NAME_<TYPE_FLOAT, TYPE_INT>;
    using TYPE_DISTANCES = CPPFileIO::SymmetricMatrix<TYPE_FLOAT, TYPE_INT>;
    using TYPE_COUNTS    = CPPFileIO::Dynamic1DArray<TYPE_INT, TYPE_INT>;

  private:
    TYPE_DISTANCES const &DISTANCES;
    TYPE_COUNTS           NUM_NEIGHBOURS;
    TYPE_FLOAT const      EPSILON;
    TYPE_INT const        MIN_PTS;

  private:
	inline void COUNT_NEIGHBOURS () {
		for(TYPE_INT i=0;i<NUM_NEIGHBOURS();i++){
			NUM_NEIGHBOURS(i)=0;
		}

		for(TYPE_INT y=1;y<DISTANCES.SIZE_Y();y++) {
			for(TYPE_INT x=0;x<y;x++) {
				if(DISTANCES(y,x) < EPSILON){
					NUM_NEIGHBOURS(y)++;
					NUM_NEIGHBOURS(x)++;
				}
			}
		}
	}

  public:
    _MACRO_CLASS_NAME_(TYPE_DISTANCES const &distances, TYPE_INT const epsilon,
                       TYPE_INT const min_pts)
      : DISTANCES(distances), NUM_NEIGHBOURS(DISTANCES.SIZE_X()),
        EPSILON(epsilon), MIN_PTS(min_pts) {}

    ~_MACRO_CLASS_NAME_() {}
};

#undef _MACRO_CLASS_NAME_

#endif
