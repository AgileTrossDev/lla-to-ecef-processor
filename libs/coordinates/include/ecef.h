#ifndef ECEF_H
#define ECEF_H

#include <string>
#include <vector>

// Class representing Earth-Centered, Earth Fixed (ECEF) cordinates
class Ecef {

  public:
    
    // Builder Functions
    static Ecef build_from_string(std::string line);
    static Ecef build_from_lla(double time, double latitude, double longitude, double altitude); 

    // Class Level Static Functions
    static void calculate_the_velocity(const Ecef& point1, Ecef& point2 );
    static bool compare_by_time(const Ecef& a, const Ecef& b);
       
    // Constants
    // TODO: Move to Constants file
    static const double PI;       // PI Value
    static const double WGS84_A;  // Semi-major axis 
    static const double WGS84_B;  // Semi-minor axis 
    static const double WGS84_E;  // first eccentricity --> Math.sqrt( (a**2 - b**2)/a**2 )

    // Constructors
    Ecef() :time(-1), x(0), y(0), z(0),velocity_x(0),velocity_y(0), velocity_z(0) {};      
    Ecef(double _time, double _x, double _y, double _z) :time(_time), x(_x), y(_y), z(_z),velocity_x(0),velocity_y(0), velocity_z(0) {};      

    // Operations
    void disp(void) const; 

    // Accessors
    double  get_time() const {return time;}
    double  get_x() const {return x;}
    double  get_y() const {return y;}
    double  get_z() const {return z;}

    double  get_vx() const {return velocity_x;}
    double  get_vy() const {return velocity_y;}
    double  get_vz() const {return velocity_z;}
    void  set_vx(double i) {velocity_x = i;}
    void  set_vy(double i) {velocity_y = i;}
    void  set_vz(double i) {velocity_z = i;}

  private:

    static double calculate_curvature_from_lat(double latitude);
    static double convert_degrees_to_radians(double input);

    double time, x, y, z, velocity_x,velocity_y, velocity_z;

};

typedef std::vector<Ecef> EcefVector_T;

#endif