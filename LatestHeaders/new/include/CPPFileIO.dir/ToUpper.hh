#ifndef _HEADER_GUARD_CPPFileIO_ToUpper_
#define _HEADER_GUARD_CPPFileIO_ToUpper_

#include "../Headers.hh"

inline char ToUpper_Lookup (char const in) {
    switch(in) {
      case 'a' : return 'A' ;
      case 'b' : return 'B' ;
      case 'c' : return 'C' ;
      case 'd' : return 'D' ;
      case 'e' : return 'E' ;
      case 'f' : return 'F' ;
      case 'g' : return 'G' ;
      case 'h' : return 'H' ;
      case 'i' : return 'I' ;
      case 'j' : return 'J' ;
      case 'k' : return 'K' ;
      case 'l' : return 'L' ;
      case 'm' : return 'M' ;
      case 'n' : return 'N' ;
      case 'o' : return 'O' ;
      case 'p' : return 'P' ;
      case 'q' : return 'Q' ;
      case 'r' : return 'R' ;
      case 's' : return 'S' ;
      case 't' : return 'T' ;
      case 'u' : return 'U' ;
      case 'v' : return 'V' ;
      case 'w' : return 'W' ;
      case 'x' : return 'X' ;
      case 'y' : return 'Y' ;
      case 'z' : return 'Z' ;
      default  : return in  ;
    }
}

inline char ToUpper_Subtract (char const in) {
  return ( ('a'<=in) && (in<='z') ? in-32 : in) ;
}

inline void ToUpper_Lookup (std::string & in) {
    for(char & c : in){
        c = ToUpper_Lookup(c);
    }
}

inline void ToUpper_Subtract (std::string & in) {
    for(char & c : in){
        c = ToUpper_Subtract(c);
    }
}

#endif
