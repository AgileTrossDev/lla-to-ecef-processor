#ifndef LLA_H
#define LLA_H

#include <string>
#include <vector>

// Class representing Latitude, Longitude, and Attitude Geodetic Coordinates
class Lla {

  public:
    
    static Lla build_from_string(std::string line);
    void disp(void);
    double time, latitude, longitude, altitude;

};

typedef std::vector<Lla> LlaVector_T;

#endif