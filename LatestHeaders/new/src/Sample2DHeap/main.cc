#include "./LatestHeaders/new/include/Read_Show_Functions.hh"
#include "./LatestHeaders/new/include/Spherical_Geometry_Clustering.hh"

using TYPE_INT   = long;
using TYPE_INTS   = std::vector <TYPE_INT>;
using TYPE_FLOAT = double;
using TYPE_ULONG = unsigned long;
using TYPE_COORDINATE =
  Spherical_Geometry_Clustering::D2GPS_Coordinates<TYPE_FLOAT, TYPE_INT>;
using TYPE_RAND = CPPFileIO::myrandgen<pcg64, TYPE_FLOAT, TYPE_INT>;
using TYPE_HEAP_STORE = Spherical_Geometry_Clustering::D2GPS_Store <TYPE_FLOAT, TYPE_INT>;
using TYPE_ELEMENT = TYPE_HEAP_STORE::TYPE_ELEMENT;

int main() {
    size_t constexpr nums = 1000;
    TYPE_RAND                                            lat_gen(1, -90, 90);
    TYPE_RAND                                            lon_gen(1, -180, 180);
    CPPFileIO::Dynamic1DArray<TYPE_COORDINATE, TYPE_INT> indices(nums);
    for (size_t i = 0; i < nums; i++) {
        TYPE_COORDINATE coordinates;
        coordinates.latitude  = lat_gen[0];
        coordinates.longitude = lon_gen[0];
        indices(i)            = coordinates;
    }
    TYPE_HEAP_STORE store("./teststore");
    store.MAKE_STORE(indices);
    TYPE_INTS out_indices;
    store(out_indices, -5.0, -5.0, 5.0, 5.0);
    Spherical_Geometry_Clustering::D2GPS_Box<TYPE_FLOAT, TYPE_INT> bbox;
    bbox.MIN.latitude = -5;
    bbox.MIN.longitude = -5;
    bbox.MAX.latitude = 5;
    bbox.MAX.longitude = 5;
    for(TYPE_INT i=0;i<indices();i++){
      bool const res = bbox(indices(i));
      if(res) {
      printf("truth... %ld\n",i);
      }
    }
    CPPFileIO::FileArray <TYPE_ELEMENT> reader("./teststore/out.store.bin");
    size_t const limit = reader(); 
    for(size_t i=0;i<limit;i++){
      bool const res= bbox(reader(i).POINT);
      if(res) {
        printf("After Sorting... %ld\n", reader(i).INDEX);
      }
      
    }
    return 0;
}
