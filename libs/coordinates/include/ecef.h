#include <string>
#include <vector>

struct ECEFVelocity {
    double vx;
    double vy;
    double vz;
};



// Class representing Earth-Centered, Earth Fixed (ECEF) cordinates
class Ecef {

  public:
    
    // Builder Functions
    static Ecef build_from_string(std::string line);
    static Ecef build_from_lla(double time, double latitude, double longitude, double altitude); 
    static ECEFVelocity calculateECEFVelocity(const Ecef& point1, const Ecef& point2 );
    std::vector<Ecef> interpolateECEFPoints(const Ecef& startPoint, const Ecef& endPoint, int numIntermediatePoints);

    static const double PI;       // PI Value
    static const double WGS84_A;  // Semi-major axis 
    static const double WGS84_B;  // Semi-minor axis 
    static const double WGS84_E;  // first eccentricity --> Math.sqrt( (a**2 - b**2)/a**2 )

    // Constructors
    Ecef(double _time, double _x, double _y, double _z) :time(_time), x(_x), y(_y), z(_z) {};      
    void disp(void); 

    // Accessors
    double  get_time() const {return time;}
    double  get_x() const {return x;}
    double  get_y() const {return y;}
    double  get_z() const {return z;}


  private:

    static double calculate_curvature_from_lat(double latitude);
    static double convert_degrees_to_radians(double input);

    double time, x,y, z;

};

typedef std::vector<Ecef> EcefVector_T;