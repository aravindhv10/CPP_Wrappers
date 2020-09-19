#ifndef NewHEPHeaders2_HH
#define NewHEPHeaders2_HH

	#ifdef USE_ALL
	#define USE_PYTHIA
	#define USE_FASTJET
	#define USE_DELPHES
	#define USE_ROOT
	#endif

	#include "CPPFileIO/main.hh"

	#ifdef USE_PYTHIA
	#include <Pythia8/Pythia.h>
	#include <Pythia8Plugins/HepMC2.h>
	#endif

	#ifdef USE_FASTJET
	#include <fastjet/Selector.hh>
	#include <fastjet/tools/JHTopTagger.hh>
	#include <fastjet/ClusterSequence.hh>
	#endif

	#include "Definitions.hh"

	#include "SpacialVectors.hh"
	#include "FeynmanDiagrams.hh"

	#include "TopReconstruction.hh"

	#ifdef Pythia8_Pythia_H
	#include "PyMGGen.hh"
	#endif

	#ifdef __FASTJET_JETDEFINITION_HH__
	#include "ConvinentJetClustering.hh"
	#include "JetImageFormation.hh"
	#endif


	#ifdef USE_ROOT
	#include "RooWrappers.hh"
	#endif

	#ifdef USE_DELPHES
	#include "DelphesWrappers.hh"
	#endif

#endif
