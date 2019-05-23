#include "CPPFileIO.hh"
int main ( int argc , char ** argv ) {
    CPPFileIO::GetArgs args ( argc , argv ) ;
    std::vector <std::string> progargs ; /* Arguments: */ {
        progargs.push_back ( "/usr/bin/xfce4-terminal" ) ;
        progargs.push_back ( "--disable-server"        ) ;
        progargs.push_back ( "--maximize"              ) ;
        progargs.push_back ( "--hide-menubar"          ) ;
        progargs.push_back ( "--hide-borders"          ) ;
        progargs.push_back ( "--hide-toolbar"          ) ;
        progargs.push_back ( "--hide-scrollbar"        ) ;
        progargs.push_back ( "--zoom=2"                ) ;
        progargs.push_back ( "--execute"               ) ;
        progargs.push_back ( "/opt/google/chrome/google-chrome" ) ;
    } for ( size_t i = 1 ; i < args () ; i++ ) { progargs.push_back ( args (i) ) ; }
    CPPFileIO::starter_self(progargs);
    return 0 ;
}
