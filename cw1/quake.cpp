// COMP2811 Coursework 1: Quake class

#include <stdexcept>
#include "quake.hpp"

using namespace std;


Quake::Quake(const string& tm, double lat, double lon, double dep, double mag):
  time(tm), latitude(lat), longitude(lon), depth(dep), magnitude(mag)
{
  // Add validation code here
  if(getLatitude() > MAX_LATITUDE || getLatitude() < MIN_LATITUDE){
    throw invalid_argument("invalid lattitude");
  }else if (getLongitude() > MAX_LONGITUDE || getLongitude() < MIN_LONGITUDE){
    throw invalid_argument("invalid longitude");
  }else if (getMagnitude() < 0 || getDepth() < 0){
    throw invalid_argument("depth or magnitude cannot be negative");
  }
}


ostream& operator<<(ostream& out, const Quake& quake)
{
  return out << "Time: " << quake.getTime()
             << "\nLatitude: " << quake.getLatitude() << " deg"
             << "\nLongitude: " << quake.getLongitude() << " deg"
             << "\nDepth: " << quake.getDepth() << " km"
             << "\nMagnitude: " << quake.getMagnitude() << endl;
}
