namespace NewHEPHeaders /* Pythia to HEPMc part: */ {

    class WriteHepmc2Fifo {
    private:

		std::string name;
        HepMC::Pythia8ToHepMC ToHepMC;
        HepMC::IO_GenEvent ascii_io;

    public:

		WriteHepmc2Fifo (
			std::string _name
		):ascii_io (&(_name[0]), std::ios::out) { name = _name; }

        ~WriteHepmc2Fifo () {}
        inline void operator  () (Pythia8::Pythia & pythia) {
            HepMC::GenEvent * hepmcevt = new HepMC::GenEvent ();
            ToHepMC.fill_next_event (pythia, hepmcevt);
            ascii_io << hepmcevt;
            delete hepmcevt;
        }
    };

    void LHA2HEPMC (std::string lhafile, std::string hepmcfile) {
        WriteHepmc2Fifo outfile (hepmcfile);
        Pythia8::Pythia pythia;
        pythia.readString ("Beams:frameType = 4");
        std::string tmp ("Beams:LHEF = ");
        tmp = tmp + lhafile;
        pythia.readString (&(tmp[0]));
        pythia.init ();
        for (size_t iEvent = 0; (!pythia.info.atEndOfFile ()); iEvent++) if (pythia.next ())
        {outfile (pythia);}
        pythia.stat ();
    }

    namespace PythiaConfig {
        inline void PrepareDefault (Pythia8::Pythia & pythia) {
            pythia.readString ( "Beams:eCM = 13000"         ) ;
            pythia.readString ( "Random:setSeed = on"       ) ;
        }
    }

    template <typename T> inline void Generate (size_t const index, size_t const count) {
        CPPFileIO::SetCPUAffinity((int)(index%8));
        char tmp[128] ; sprintf(tmp,"hepmc.%ld.fifo",index);
        mkfifo (tmp,0755) ; CPPFileIO::ForkMe forker ;
        if(forker.InKid()){
            std::vector <std::string> progs ;
            progs.push_back("./RunDelphes");
            progs.push_back(tmp);
            CPPFileIO::starter_self(progs);
        } else {
            NewHEPHeaders::WriteHepmc2Fifo outwriter(tmp);
            Pythia8::Pythia pythia ;
            {PythiaConfig::PrepareDefault(pythia);T tmpslave(pythia);}
            /* Initialize the random number generator: */ {
                sprintf ( tmp , "Random:seed = %ld" , index + 1 ) ;
                pythia.readString ( tmp ) ;
            }
            pythia.init();
            for (size_t i=0;i<count;i++) if (pythia.next()) {outwriter(pythia);}
        }
    }

}
