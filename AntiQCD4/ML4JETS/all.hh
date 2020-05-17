#define USE_ALL
#include "./LatestHeaders/NewHEPHeaders/main.hh"
#include "/usr/local/include/TH2F.h"
#include "/usr/local/include/TCanvas.h"
#include "./Nsubjettiness/main.h"

namespace DEFINITIONS {

	using type_names = std::vector <std::string> ;
	using vector4 = fastjet::PseudoJet ;
	using vector4s = std::vector <vector4> ;
	using type_jetimage = NewHEPHeaders::BoxImageGen <40,float,true> ;
	using type_arrayimage = Tensors::Array::N2D_ARRAY <40,40,float> ;
	using type_writer = CPPFileIO::FileVector <type_jetimage> ;
	using type_imagereader = CPPFileIO::FullFileReader <type_arrayimage> ;
	using type_nsub = Tensors::Array::ND_ARRAY <5,float> ;

	struct element {

		float T, X, Y, Z ;

		inline vector4
		operator () () const {
			vector4 ret (X,Y,Z,T) ;
			return ret ;
		}

	} ;

	struct event {

		element jet_vectors[200] ;
		element top ;

		inline vector4s
		operator () () const {
			vector4s ret ;
			for (size_t i=0;i<200;i++) {
				if (jet_vectors[i].T > 0.0000001) {
					ret.push_back(jet_vectors[i]()) ;
				}
			}
			return ret ;
		}

	} ;

	using type_reader =
		CPPFileIO::FullFileReader <event>
	; //

	inline double
	get_mass (
		vector4s const &
			in
	) {
		vector4 jet(0,0,0,0) ;
		for (size_t i=0;i<in.size();i++) {
			jet += in[i] ;
		}
		return jet.m();
	}

	inline type_nsub
	get_nsub (
		vector4s const &
			in
	) {
		type_nsub ret ;
		ret = 0 ;

		using namespace std;
		using namespace fastjet;
		using namespace fastjet::contrib;

		JetAlgorithm algorithm = cambridge_aachen_algorithm;
		double jet_rad = 1.00; // jet radius for anti-kt algorithm
		JetDefinition jetDef = JetDefinition(algorithm,jet_rad,E_scheme,Best);
		ClusterSequence clust_seq(in,jetDef);
		vector<PseudoJet> ca_jets  = clust_seq.exclusive_jets(1);

		double constexpr beta = 2.0 ;

		Nsubjettiness nSub1_beta2(1, OnePass_KT_Axes(), UnnormalizedMeasure(beta));
		Nsubjettiness nSub2_beta2(2, OnePass_KT_Axes(), UnnormalizedMeasure(beta));
		Nsubjettiness nSub3_beta2(3, OnePass_KT_Axes(), UnnormalizedMeasure(beta));
		Nsubjettiness nSub4_beta2(4, OnePass_KT_Axes(), UnnormalizedMeasure(beta));
		Nsubjettiness nSub5_beta2(5, OnePass_KT_Axes(), UnnormalizedMeasure(beta));

		ret[0] =  nSub1_beta2(ca_jets[0]);
		ret[1] =  nSub2_beta2(ca_jets[0]);
		ret[2] =  nSub3_beta2(ca_jets[0]);
		ret[3] =  nSub4_beta2(ca_jets[0]);
		ret[4] =  nSub5_beta2(ca_jets[0]);

		return ret ;
	}
}

namespace IMAGE_FORMER {

	using namespace DEFINITIONS ;

	namespace TRAIN {

		inline void
		QCD () {
			/* For qcd: */ {
				std::string filename ("./S1/QCD/TRAIN/out.bin") ;
				CPPFileIO::FullFileReader <event> reader (filename) ;
				type_jetimage jetimage ;
				for (size_t i=0;i<reader();i+=100000) {
					size_t index = i/100000 ;
					char tmp[256] ;

					sprintf(tmp,"./S2/QCD/TRAIN/out.%ld.bin",index);
					CPPFileIO::FileVector <type_jetimage> writer (tmp) ;
					sprintf(tmp,"./S2/QCD/TRAIN/mass.%ld.bin",index);
					CPPFileIO::FileVector <float> writer_mass (tmp) ;
					sprintf(tmp,"./S2/QCD/TRAIN/nsub.%ld.bin",index);
					CPPFileIO::FileVector <type_nsub> writer_nsub (tmp) ;

					size_t limit =
						CPPFileIO::mymin (
							i + 100000 ,
							reader()
						)
					; //
					for(size_t j=i;j<limit;j++){
						if((j%10000)==0){
							printf("evaluated %ld events\n",j);
						}
						auto full_jet = reader(j)() ;
						if(jetimage(full_jet)){
							writer.push_back(jetimage);
							writer_mass.push_back(get_mass(full_jet));
							writer_nsub.push_back(get_nsub(full_jet));
						}
					}
				}
			}
		}

		inline void
		TOP () {
			/* For Top: */ {
				std::string filename ("./S1/TOP/TRAIN/out.bin") ;
				CPPFileIO::FullFileReader <event> reader (filename) ;
				type_jetimage jetimage ;
				for (size_t i=0;i<reader();i+=100000) {
					size_t index = i/100000 ;
					char tmp[256] ;
					sprintf(tmp,"./S2/TOP/TRAIN/out.%ld.bin",index);
					CPPFileIO::FileVector <type_jetimage> writer (tmp) ;
					sprintf(tmp,"./S2/TOP/TRAIN/mass.%ld.bin",index);
					CPPFileIO::FileVector <float> writer_mass (tmp) ;
					sprintf(tmp,"./S2/TOP/TRAIN/nsub.%ld.bin",index);
					CPPFileIO::FileVector <type_nsub> writer_nsub (tmp) ;
					size_t limit =
						CPPFileIO::mymin (
							i + 100000 ,
							reader()
						)
					; //
					for(size_t j=i;j<limit;j++){
						if((j%10000)==0){
							printf("evaluated %ld events\n",j);
						}
						auto full_jet = reader(j)() ;
						if(jetimage(full_jet)){
							writer.push_back(jetimage);
							writer_mass.push_back(get_mass(full_jet));
							writer_nsub.push_back(get_nsub(full_jet));
						}
					}

				}
			}
		}
	}

	namespace TEST {

		inline void
		QCD () {
			/* For qcd: */ {
				std::string filename ("./S1/QCD/TEST/out.bin") ;
				CPPFileIO::FullFileReader <event> reader (filename) ;
				type_jetimage jetimage ;
				for (size_t i=0;i<reader();i+=100000) {
					size_t index = i/100000 ;
					char tmp[256] ;
					sprintf(tmp,"./S2/QCD/TEST/out.%ld.bin",index);
					CPPFileIO::FileVector <type_jetimage> writer (tmp) ;
					sprintf(tmp,"./S2/QCD/TEST/mass.%ld.bin",index);
					CPPFileIO::FileVector <float> writer_mass (tmp) ;
					sprintf(tmp,"./S2/QCD/TEST/nsub.%ld.bin",index);
					CPPFileIO::FileVector <type_nsub> writer_nsub (tmp) ;
					size_t limit =
						CPPFileIO::mymin (
							i + 100000 ,
							reader()
						)
					; //
					for(size_t j=i;j<limit;j++){
						if((j%10000)==0){
							printf("evaluated %ld events\n",j);
						}
						auto full_jet = reader(j)() ;
						if(jetimage(full_jet)){
							writer.push_back(jetimage);
							writer_mass.push_back(get_mass(full_jet));
							writer_nsub.push_back(get_nsub(full_jet));
						}
					}
				}
			}
		}

		inline void
		TOP () {
			/* For Top: */ {
				std::string filename ("./S1/TOP/TEST/out.bin") ;
				CPPFileIO::FullFileReader <event> reader (filename) ;
				type_jetimage jetimage ;
				for (size_t i=0;i<reader();i+=100000) {
					size_t index = i/100000 ;
					char tmp[256] ;
					sprintf(tmp,"./S2/TOP/TEST/out.%ld.bin",index);
					CPPFileIO::FileVector <type_jetimage> writer (tmp) ;
					sprintf(tmp,"./S2/TOP/TEST/mass.%ld.bin",index);
					CPPFileIO::FileVector <float> writer_mass (tmp) ;
					sprintf(tmp,"./S2/TOP/TEST/nsub.%ld.bin",index);
					CPPFileIO::FileVector <type_nsub> writer_nsub (tmp) ;
					size_t limit =
						CPPFileIO::mymin (
							i + 100000 ,
							reader()
						)
					; //
					for(size_t j=i;j<limit;j++){
						if((j%10000)==0){
							printf("evaluated %ld events\n",j);
						}
						auto full_jet = reader(j)() ;
						if(jetimage(full_jet)){
							writer.push_back(jetimage);
							writer_mass.push_back(get_mass(full_jet));
							writer_nsub.push_back(get_nsub(full_jet));
						}
					}
				}
			}
		}
	}

	namespace VAL {

		inline void
		QCD () {
			/* For qcd: */ {
				std::string filename ("./S1/QCD/VAL/out.bin") ;
				CPPFileIO::FullFileReader <event> reader (filename) ;
				type_jetimage jetimage ;
				for (size_t i=0;i<reader();i+=100000) {
					size_t index = i/100000 ;
					char tmp[256] ;
					sprintf(tmp,"./S2/QCD/VAL/out.%ld.bin",index);
					CPPFileIO::FileVector <type_jetimage> writer (tmp) ;
					sprintf(tmp,"./S2/QCD/VAL/mass.%ld.bin",index);
					CPPFileIO::FileVector <float> writer_mass (tmp) ;
					sprintf(tmp,"./S2/QCD/VAL/nsub.%ld.bin",index);
					CPPFileIO::FileVector <type_nsub> writer_nsub (tmp) ;
					size_t limit =
						CPPFileIO::mymin (
							i + 100000 ,
							reader()
						)
					; //
					for(size_t j=i;j<limit;j++){
						if((j%10000)==0){
							printf("evaluated %ld events\n",j);
						}
						auto full_jet = reader(j)() ;
						if(jetimage(full_jet)){
							writer.push_back(jetimage);
							writer_mass.push_back(get_mass(full_jet));
							writer_nsub.push_back(get_nsub(full_jet));
						}
					}
				}
			}
		}

		inline void
		TOP () {
			/* For Top: */ {
				std::string filename ("./S1/TOP/VAL/out.bin") ;
				CPPFileIO::FullFileReader <event> reader (filename) ;
				type_jetimage jetimage ;
				for (size_t i=0;i<reader();i+=100000) {
					size_t index = i/100000 ;
					char tmp[256] ;
					sprintf(tmp,"./S2/TOP/VAL/out.%ld.bin",index);
					CPPFileIO::FileVector <type_jetimage> writer (tmp) ;
					sprintf(tmp,"./S2/TOP/VAL/mass.%ld.bin",index);
					CPPFileIO::FileVector <float> writer_mass (tmp) ;
					sprintf(tmp,"./S2/TOP/VAL/nsub.%ld.bin",index);
					CPPFileIO::FileVector <type_nsub> writer_nsub (tmp) ;
					size_t limit =
						CPPFileIO::mymin (
							i + 100000 ,
							reader()
						)
					; //
					for(size_t j=i;j<limit;j++){
						if((j%10000)==0){
							printf("evaluated %ld events\n",j);
						}
						auto full_jet = reader(j)() ;
						if(jetimage(full_jet)){
							writer.push_back(jetimage);
							writer_mass.push_back(get_mass(full_jet));
							writer_nsub.push_back(get_nsub(full_jet));
						}
					}
				}
			}
		}
	}

	inline void
	FormAllImages () {
		CPPFileIO::ForkMe forker ;
		if (forker.InKid()) {TRAIN::QCD();}
		if (forker.InKid()) {TRAIN::TOP();}
		if (forker.InKid()) {TEST::QCD();}
		if (forker.InKid()) {TEST::TOP();}
		if (forker.InKid()) {VAL::QCD();}
		if (forker.InKid()) {VAL::TOP();}
	}

}

namespace PLOTTING {

	using namespace DEFINITIONS ;

	namespace UNIFY {

		inline void
		unify_qcd () {
			CPPFileIO::ForkMe forker ;
			/* TRAIN */ if (forker.InKid()) {
				TH2F img("qcd_train","qcd_train",40,0,39,40,0,39);
				std::string names[7] = {
					#include "./S2/QCD/TRAIN.hh"
				} ;
				type_arrayimage outsum ;
				outsum = 0.0 ;
				for(size_t i=0;i<7;i++){
					type_imagereader reader (names[i]) ;
					for(size_t j=0;j<reader();j++){
						outsum += reader(j);
						auto diff = reader(j).L1_NORM()-1.0 ;
						if ((diff<-0.1)||(0.1<diff)) {
							printf("BIG FAT ISSUE!!! %e\n",diff);
						}
					}
				}
				CPPFileIO::FileVector <type_arrayimage> writer ("./S2/QCD/TRAIN.bin") ;
				writer.push_back(outsum);
			}
			/* TEST */ if (forker.InKid()) {
				std::string names[3] = {
					#include "./S2/QCD/TEST.hh"
				} ;
				type_arrayimage outsum ;
				outsum = 0.0 ;
				for(size_t i=0;i<3;i++){
					type_imagereader reader (names[i]) ;
					for(size_t j=0;j<reader();j++){
						outsum+=reader(j);
						auto diff = reader(j).L1_NORM()-1.0 ;
						if ((diff<-0.1)||(0.1<diff)) {
							printf("BIG FAT ISSUE!!! %e\n",diff);
						}
					}

				}
				CPPFileIO::FileVector <type_arrayimage> writer ("./S2/QCD/TEST.bin") ;
				writer.push_back(outsum);
			}
			/* VAL */ if (forker.InKid()) {
				std::string names[3] = {
					#include "./S2/QCD/VAL.hh"
				} ;
				type_arrayimage outsum ;
				outsum = 0.0 ;
				for(size_t i=0;i<3;i++){
					type_imagereader reader (names[i]) ;
					for(size_t j=0;j<reader();j++){
						outsum+=reader(j);
						auto diff = reader(j).L1_NORM()-1.0 ;
						if ((diff<-0.1)||(0.1<diff)) {
							printf("BIG FAT ISSUE!!! %e\n",diff);
						}
					}
				}
				CPPFileIO::FileVector <type_arrayimage> writer ("./S2/QCD/VAL.bin") ;
				writer.push_back(outsum);
			}
		}

		inline void
		unify_top () {
			CPPFileIO::ForkMe forker ;
			/* TRAIN */ if (forker.InKid()) {
				std::string names[7] = {
					#include "./S2/TOP/TRAIN.hh"
				} ;
				type_arrayimage outsum ;
				outsum = 0.0 ;
				for(size_t i=0;i<7;i++){
					type_imagereader reader (names[i]) ;
					for(size_t j=0;j<reader();j++){
						outsum+=reader(j);
						auto diff = reader(j).L1_NORM()-1.0 ;
						if ((diff<-0.1)||(0.1<diff)) {
							printf("BIG FAT ISSUE!!! %e\n",diff);
						}
					}
				}
				CPPFileIO::FileVector <type_arrayimage> writer ("./S2/TOP/TRAIN.bin") ;
				writer.push_back(outsum);
			}
			/* TEST */ if (forker.InKid()) {
				std::string names[3] = {
					#include "./S2/TOP/TEST.hh"
				} ;
				type_arrayimage outsum ;
				outsum = 0.0 ;
				for(size_t i=0;i<3;i++){
					type_imagereader reader (names[i]) ;
					for(size_t j=0;j<reader();j++){
						outsum+=reader(j);
						auto diff = reader(j).L1_NORM()-1.0 ;
						if ((diff<-0.1)||(0.1<diff)) {
							printf("BIG FAT ISSUE!!! %e\n",diff);
						}
					}
				}
				CPPFileIO::FileVector <type_arrayimage> writer ("./S2/TOP/TEST.bin") ;
				writer.push_back(outsum);
			}
			/* VAL */ if (forker.InKid()) {
				std::string names[3] = {
					#include "./S2/TOP/VAL.hh"
				} ;
				type_arrayimage outsum ;
				outsum = 0.0 ;
				for(size_t i=0;i<3;i++){
					type_imagereader reader (names[i]) ;
					for(size_t j=0;j<reader();j++){
						outsum+=reader(j);
						auto diff = reader(j).L1_NORM()-1.0 ;
						if ((diff<-0.1)||(0.1<diff)) {
							printf("BIG FAT ISSUE!!! %e\n",diff);
						}
					}
				}
				CPPFileIO::FileVector <type_arrayimage> writer ("./S2/TOP/VAL.bin") ;
				writer.push_back(outsum);
			}
		}

		inline void
		unify_all () {
			CPPFileIO::ForkMe forker ;
			if (forker.InKid()) {unify_qcd();}
			if (forker.InKid()) {unify_top();}
		}

	}

	inline void
	PlotAll () {
		/* QCD */ {
			/* TRAIN */ {
				type_imagereader reader ("S2/QCD/TRAIN.bin") ;
				type_arrayimage in = reader(0) ;
				in /= in.L1_NORM();
				TH2F img("img","img",40,0,39,40,0,39);
				for (size_t s2=0;s2<reader(0).SIZE_2();s2++) {
					for (size_t s1=0;s1<reader(0).SIZE_1();s1++) {
						img.Fill(s1,s2,in[s2][s1]);
					}
				}
				TCanvas outimg ;
				img.Draw("colz");
				outimg.SaveAs("qcd_train.pdf") ;
			}
			/* TEST */ {
				type_imagereader reader ("S2/QCD/TEST.bin") ;
				type_arrayimage in = reader(0) ;
				in /= in.L1_NORM();
				TH2F img("img","img",40,0,39,40,0,39);
				for (size_t s2=0;s2<reader(0).SIZE_2();s2++) {
					for (size_t s1=0;s1<reader(0).SIZE_1();s1++) {
						img.Fill(s1,s2,in[s2][s1]);
					}
				}
				TCanvas outimg ;
				img.Draw("colz");
				outimg.SaveAs("qcd_test.pdf") ;
			}
			/* VAL */ {
				type_imagereader reader ("S2/QCD/VAL.bin") ;
				type_arrayimage in = reader(0) ;
				in /= in.L1_NORM();
				TH2F img("img","img",40,0,39,40,0,39);
				for (size_t s2=0;s2<reader(0).SIZE_2();s2++) {
					for (size_t s1=0;s1<reader(0).SIZE_1();s1++) {
						img.Fill(s1,s2,in[s2][s1]);
					}
				}
				TCanvas outimg ;
				img.Draw("colz");
				outimg.SaveAs("qcd_val.pdf") ;
			}
		}
		/* TOP */ {
			/* TRAIN */ {
				type_imagereader reader ("S2/TOP/TRAIN.bin") ;
				type_arrayimage in = reader(0) ;
				in /= in.L1_NORM();
				TH2F img("img","img",40,0,39,40,0,39);
				for (size_t s2=0;s2<reader(0).SIZE_2();s2++) {
					for (size_t s1=0;s1<reader(0).SIZE_1();s1++) {
						img.Fill(s1,s2,in[s2][s1]);
					}
				}
				TCanvas outimg ;
				img.Draw("colz");
				outimg.SaveAs("top_train.pdf") ;
			}
			/* TEST */ {
				type_imagereader reader ("S2/TOP/TEST.bin") ;
				type_arrayimage in = reader(0) ;
				in /= in.L1_NORM();
				TH2F img("img","img",40,0,39,40,0,39);
				for (size_t s2=0;s2<reader(0).SIZE_2();s2++) {
					for (size_t s1=0;s1<reader(0).SIZE_1();s1++) {
						img.Fill(s1,s2,in[s2][s1]);
					}
				}
				TCanvas outimg ;
				img.Draw("colz");
				outimg.SaveAs("top_test.pdf") ;
			}
			/* VAL */ {
				type_imagereader reader ("S2/TOP/VAL.bin") ;
				type_arrayimage in = reader(0) ;
				in /= in.L1_NORM();
				TH2F img("img","img",40,0,39,40,0,39);
				for (size_t s2=0;s2<reader(0).SIZE_2();s2++) {
					for (size_t s1=0;s1<reader(0).SIZE_1();s1++) {
						img.Fill(s1,s2,in[s2][s1]);
					}
				}
				TCanvas outimg ;
				img.Draw("colz");
				outimg.SaveAs("top_val.pdf") ;
			}
		}
	}

	namespace PLOT_SLAVE {

		inline void
		plot_nsub (
			type_names const & in ,
			std::string const out
		) {
			NewHEPHeaders::Histograms::MyHistN <5,false> hists (out,100,0,10) ;
			for(size_t i=0;i<in.size();i++){
				CPPFileIO::FullFileReader <type_nsub> reader (in[i]) ;
				for(size_t j=0;j<reader();j++){
					hists.Fill<0>(reader(j)[0]);
					hists.Fill<1>(reader(j)[1]);
					hists.Fill<2>(reader(j)[2]);
					hists.Fill<3>(reader(j)[3]);
					hists.Fill<4>(reader(j)[4]);
				}
			}
		}
	}

	inline void
	plot_nsub () {
		type_names qcd_train ;
		qcd_train.push_back("./S2/QCD/TRAIN/nsub.0.bin");
		qcd_train.push_back("./S2/QCD/TRAIN/nsub.1.bin");
		qcd_train.push_back("./S2/QCD/TRAIN/nsub.2.bin");
		qcd_train.push_back("./S2/QCD/TRAIN/nsub.3.bin");
		qcd_train.push_back("./S2/QCD/TRAIN/nsub.4.bin");
		qcd_train.push_back("./S2/QCD/TRAIN/nsub.5.bin");
		qcd_train.push_back("./S2/QCD/TRAIN/nsub.6.bin");
		PLOT_SLAVE::plot_nsub(qcd_train,"qcd_train");
		type_names top_train ;
		top_train.push_back("./S2/TOP/TRAIN/nsub.0.bin");
		top_train.push_back("./S2/TOP/TRAIN/nsub.1.bin");
		top_train.push_back("./S2/TOP/TRAIN/nsub.2.bin");
		top_train.push_back("./S2/TOP/TRAIN/nsub.3.bin");
		top_train.push_back("./S2/TOP/TRAIN/nsub.4.bin");
		top_train.push_back("./S2/TOP/TRAIN/nsub.5.bin");
		top_train.push_back("./S2/TOP/TRAIN/nsub.6.bin");
		PLOT_SLAVE::plot_nsub(top_train,"top_train");
	}
}

namespace COMPRESS {

	inline void
	COMPRESS (
		std::string const
			filename
	) {
		CPPFileIO::ExternalStarter<true>::GET
			("/usr/local/bin/xz")("-z9evvfk")(filename)
		; //
	}

	inline void
	COMPRESS_ALL () {
		/* images: */ {
			std::vector <std::string> names ;
			names.push_back("./S2/QCD/TEST/out.0.bin");
			names.push_back("./S2/QCD/TEST/out.1.bin");
			names.push_back("./S2/QCD/TEST/out.2.bin");
			names.push_back("./S2/QCD/TRAIN/out.0.bin");
			names.push_back("./S2/QCD/TRAIN/out.1.bin");
			names.push_back("./S2/QCD/TRAIN/out.2.bin");
			names.push_back("./S2/QCD/TRAIN/out.3.bin");
			names.push_back("./S2/QCD/TRAIN/out.4.bin");
			names.push_back("./S2/QCD/TRAIN/out.5.bin");
			names.push_back("./S2/QCD/TRAIN/out.6.bin");
			names.push_back("./S2/QCD/VAL/out.0.bin");
			names.push_back("./S2/QCD/VAL/out.1.bin");
			names.push_back("./S2/QCD/VAL/out.2.bin");
			names.push_back("./S2/TOP/TEST/out.0.bin");
			names.push_back("./S2/TOP/TEST/out.1.bin");
			names.push_back("./S2/TOP/TEST/out.2.bin");
			names.push_back("./S2/TOP/TRAIN/out.0.bin");
			names.push_back("./S2/TOP/TRAIN/out.1.bin");
			names.push_back("./S2/TOP/TRAIN/out.2.bin");
			names.push_back("./S2/TOP/TRAIN/out.3.bin");
			names.push_back("./S2/TOP/TRAIN/out.4.bin");
			names.push_back("./S2/TOP/TRAIN/out.5.bin");
			names.push_back("./S2/TOP/TRAIN/out.6.bin");
			names.push_back("./S2/TOP/VAL/out.0.bin");
			names.push_back("./S2/TOP/VAL/out.1.bin");
			names.push_back("./S2/TOP/VAL/out.2.bin");

			#pragma omp parallel for
			for (size_t i=0;i<names.size();i++)
			{ COMPRESS (names[i]) ; }
		}
		/* mass */ {
			std::vector <std::string> names ;
			names.push_back("./S2/QCD/TEST/mass.0.bin");
			names.push_back("./S2/QCD/TEST/mass.1.bin");
			names.push_back("./S2/QCD/TEST/mass.2.bin");
			names.push_back("./S2/QCD/TRAIN/mass.0.bin");
			names.push_back("./S2/QCD/TRAIN/mass.1.bin");
			names.push_back("./S2/QCD/TRAIN/mass.2.bin");
			names.push_back("./S2/QCD/TRAIN/mass.3.bin");
			names.push_back("./S2/QCD/TRAIN/mass.4.bin");
			names.push_back("./S2/QCD/TRAIN/mass.5.bin");
			names.push_back("./S2/QCD/TRAIN/mass.6.bin");
			names.push_back("./S2/QCD/VAL/mass.0.bin");
			names.push_back("./S2/QCD/VAL/mass.1.bin");
			names.push_back("./S2/QCD/VAL/mass.2.bin");
			names.push_back("./S2/TOP/TEST/mass.0.bin");
			names.push_back("./S2/TOP/TEST/mass.1.bin");
			names.push_back("./S2/TOP/TEST/mass.2.bin");
			names.push_back("./S2/TOP/TRAIN/mass.0.bin");
			names.push_back("./S2/TOP/TRAIN/mass.1.bin");
			names.push_back("./S2/TOP/TRAIN/mass.2.bin");
			names.push_back("./S2/TOP/TRAIN/mass.3.bin");
			names.push_back("./S2/TOP/TRAIN/mass.4.bin");
			names.push_back("./S2/TOP/TRAIN/mass.5.bin");
			names.push_back("./S2/TOP/TRAIN/mass.6.bin");
			names.push_back("./S2/TOP/VAL/mass.0.bin");
			names.push_back("./S2/TOP/VAL/mass.1.bin");
			names.push_back("./S2/TOP/VAL/mass.2.bin");

			#pragma omp parallel for
			for (size_t i=0;i<names.size();i++)
			{ COMPRESS (names[i]) ; }
		}
		/* nsub */ {
			std::vector <std::string> names ;
			names.push_back("./S2/QCD/TEST/nsub.0.bin");
			names.push_back("./S2/QCD/TEST/nsub.1.bin");
			names.push_back("./S2/QCD/TEST/nsub.2.bin");
			names.push_back("./S2/QCD/TRAIN/nsub.0.bin");
			names.push_back("./S2/QCD/TRAIN/nsub.1.bin");
			names.push_back("./S2/QCD/TRAIN/nsub.2.bin");
			names.push_back("./S2/QCD/TRAIN/nsub.3.bin");
			names.push_back("./S2/QCD/TRAIN/nsub.4.bin");
			names.push_back("./S2/QCD/TRAIN/nsub.5.bin");
			names.push_back("./S2/QCD/TRAIN/nsub.6.bin");
			names.push_back("./S2/QCD/VAL/nsub.0.bin");
			names.push_back("./S2/QCD/VAL/nsub.1.bin");
			names.push_back("./S2/QCD/VAL/nsub.2.bin");
			names.push_back("./S2/TOP/TEST/nsub.0.bin");
			names.push_back("./S2/TOP/TEST/nsub.1.bin");
			names.push_back("./S2/TOP/TEST/nsub.2.bin");
			names.push_back("./S2/TOP/TRAIN/nsub.0.bin");
			names.push_back("./S2/TOP/TRAIN/nsub.1.bin");
			names.push_back("./S2/TOP/TRAIN/nsub.2.bin");
			names.push_back("./S2/TOP/TRAIN/nsub.3.bin");
			names.push_back("./S2/TOP/TRAIN/nsub.4.bin");
			names.push_back("./S2/TOP/TRAIN/nsub.5.bin");
			names.push_back("./S2/TOP/TRAIN/nsub.6.bin");
			names.push_back("./S2/TOP/VAL/nsub.0.bin");
			names.push_back("./S2/TOP/VAL/nsub.1.bin");
			names.push_back("./S2/TOP/VAL/nsub.2.bin");

			#pragma omp parallel for
			for (size_t i=0;i<names.size();i++)
			{ COMPRESS (names[i]) ; }
		}
	}

}

namespace SLAVE {

	inline void
	FormJetImages () {
		IMAGE_FORMER::FormAllImages();
	}

	inline void
	unify_all () {
		PLOTTING::UNIFY::unify_all();
	}

	inline void
	PlotAll () {
		PLOTTING::PlotAll () ;
	}

	inline void
	plot_nsub () {
		PLOTTING::plot_nsub();
	}

	inline void
	compress_all () {
		COMPRESS::COMPRESS_ALL () ;
	}

}

