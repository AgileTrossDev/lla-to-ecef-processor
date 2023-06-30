


#include<sstream>
#include<iostream>
#include<iomanip>
#include <cmath>

#include "ecef.h"
#include "lla.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::setprecision;
using std::sqrt;
using std::sin;
using std::cos;
using std::pow;

// Reference: 
// TODO: Move to Constants class
// For global applications, the geodetic reference (datum) used for GPS is the World Geodetic
// System 1984 (WGS84). This ellipsoid has its origin coincident with the ECEF origin. The X-axis
// pierces the Greenwich meridian (where longitude = 0 degrees) and the XY plane make up the
// equatorial plane (latitude = 0 degrees). Altitude is described as the perpendicular distance
// above the ellipsoid surface (which not to be confused with the mean sea level datum).
const double Ecef::WGS84_A = 6378137.0;             // Semi-major axis 
const double Ecef::WGS84_B = 6356752.31424518;      // Semi-minor axis 
const double Ecef::WGS84_E = 0.08181919084262149;   // first eccentricity --> Math.sqrt( (WGS84_A**2 - WGS84_B**2)/WGS84_A**2 )
const double Ecef::PI = 3.141592653589793238;

// Displays an ECEF Record to stdout
void Ecef::disp() const {
   cout << "Ecef Record" << setprecision(17) << " Time: " << time <<  setprecision(100)
    << " X: " << x 
    << " Y:" <<  y 
    << " Z:" <<  z 
    << " VelX: " << velocity_x 
    << " VelY: " << velocity_y 
    << " VelZ: " << velocity_z 
    << endl;
}

// Compares the Time between two ECEF Points
bool Ecef::compare_by_time(const Ecef& a, const Ecef& b) { return a.time < b.time; }

// Parses a string containing a CSV record of a timp stamped LLA coordinates and builds an Ecef instance
// 
// NOTE: Dependent on the Lla class for parsing and building an Lla intance so we can get the required parameters.
// NOTE: Design decision to separate logic into LLA was based on having single purpose functions
Ecef Ecef::build_from_string(string line) {     
  Lla lla_instance = Lla::build_from_string(line);   
  return build_from_lla(lla_instance.time, lla_instance.latitude, lla_instance.longitude, lla_instance.altitude);
}

// Builds an ECEF instance from a time stamped LLA coordinates, by converting the latitude,
// longitude, and altitude to the ECEF X,Y,Z coordinates using the WSG 84 Parameters
//
// Input
//  - time: Unit Time Stamp
//  - latitude - Represented in degrees
//  - longitude - Represented in degrees
//  - altitude - Represented in Kilometers
Ecef Ecef::build_from_lla(double time, double latitude, double longitude, double altitude) {  

  // Convert Input Paramets
  //  - Alitutde KM to Meters
  //  - Degrees to Radians
  altitude = altitude * 1000;  
  latitude = convert_degrees_to_radians(latitude);
  longitude = convert_degrees_to_radians(longitude);

  // Calculate the sin and cos of the Latitude and Longitude 
  double cos_lat = cos(latitude);
  double sin_lat = sin(latitude);
  double cos_long = cos(longitude);
  double sin_long = sin(longitude);

  double N = WGS84_A / sqrt(1.0 - WGS84_E * WGS84_E * sin_lat * sin_lat);

  // Use formula to calculate x,y,z ECEF cordinates
  double x = (N + altitude) * cos_lat * cos_long;
  double y = (N + altitude) * cos_lat * sin_long;
  double z = (N * (1.0 - WGS84_E * WGS84_E) + altitude) * sin_lat;

  // Build ECEF Object
  //cout << setprecision(100) << x << " " << y << " " << z << endl;
  Ecef ret_val(time,x,y,z);

  return ret_val;

}

// Converts the input (Degrees) to Radians
double Ecef::convert_degrees_to_radians(double input) {
  double DEGREES_TO_RADIANS = PI / 180.0;
  return input * DEGREES_TO_RADIANS;
}

void Ecef::calculate_the_velocity(const Ecef& point1, Ecef& point2 ) {

  double time_difference = point2.get_time() - point1.get_time();
  
  point2.set_vx( (point2.get_x() - point1.get_x()) / time_difference);
  point2.set_vy((point2.get_y() - point1.get_y()) / time_difference);
  point2.set_vz((point2.get_z() - point1.get_z()) / time_difference);

  
}

// Perform linear interpolation leading up to the requested time from the previous point within positions
Ecef Ecef::linear_interpolation(const std::vector<Ecef>& positions, double requested_time) {
    // Find the position indices just before and after the requested time
    size_t end_index = 0;
    for (size_t i = 0; i < positions.size(); ++i) {
        if (positions[i].get_time() <= requested_time) {
            end_index = i;
        } else {
            break;
        }
    }

    // Handle edge cases
    if (end_index == positions.size() - 1) {
        return positions[end_index]; // The requested time is after the last position
    } else if (end_index == 0) {
        return positions[0]; // The requested time is before the first position
    }

    const Ecef& startPos = positions[end_index];
    const Ecef& endPos = positions[end_index + 1];

    int start_time = startPos.get_time();
    int end_time = endPos.get_time();
    double timeDiff = end_time - start_time;

    double weightStart = (end_time - requested_time) / timeDiff;
    double weightEnd = (requested_time - start_time) / timeDiff;

    double interpolatedX = weightStart * startPos.get_x() + weightEnd * endPos.get_x();
    double interpolatedY = weightStart * startPos.get_y() + weightEnd * endPos.get_y();
    double interpolatedZ = weightStart * startPos.get_z() + weightEnd * endPos.get_z();

    return Ecef(requested_time, interpolatedX, interpolatedY, interpolatedZ);
}

/*
std::vector<Ecef> Ecef::propagate(const std::vector<Ecef>& positions, double start_time, double end_time) {
    std::vector<Ecef> propagatedPositions;

    // Find the starting position index
    size_t startIndex = 0;
    for (size_t i = 0; i < positions.size(); ++i) {
        if (positions[i].get_time() <= start_time) {
            startIndex = i;
        } else {
            break;
        }
    }

    // Propagate positions starting from the startIndex
    double prevX = positions[startIndex].get_x();
    double prevY = positions[startIndex].get_y();
    double prevZ = positions[startIndex].get_z();

    for (size_t i = startIndex + 1; i < positions.size(); ++i) {
        const Ecef& currentPos = positions[i];
        const Ecef& prevPos = positions[i - 1];

        int deltaTime = currentPos.get_time() - prevPos.get_time();

        // Calculate the velocity components
        double velocityX = (currentPos.get_x() - prevPos.get_x()) / deltaTime;
        double velocityY = (currentPos.get_y() - prevPos.get_y()) / deltaTime;
        double velocityZ = (currentPos.get_z() - prevPos.get_z()) / deltaTime;

        // Propagate the positions within the time interval
        for (int t = prevPos.get_time() + 1; t <= currentPos.get_time() && t <= end_time; ++t) {
            double dt = t - prevPos.get_time();

            double propagatedX = prevX + dt * velocityX;
            double propagatedY = prevY + dt * velocityY;
            double propagatedZ = prevZ + dt * velocityZ;

            propagatedPositions.emplace_back(t, propagatedX, propagatedY, propagatedZ);

            prevX = propagatedX;
            prevY = propagatedY;
            prevZ = propagatedZ;
        }

        // Stop propagating if the end time is reached
        if (currentPos.get_time() >= end_time) {
            break;
        }
    }

    return propagatedPositions;
}
*/

EcefVector_T Ecef::interpolateECEFPoints(const Ecef& startPoint, const Ecef& endPoint, int numIntermediatePoints) {
    EcefVector_T intermediatePoints;
    
    double stepSize = 1.0 / (numIntermediatePoints + 1); // +1 to account for start and end points
    
    for (int i = 1; i <= numIntermediatePoints; i++) {
        double factor = i * stepSize;
        
        Ecef intermediatePoint(
          startPoint.get_time(),
          startPoint.get_x() + (endPoint.get_x() - startPoint.get_x()) * factor,
          startPoint.get_y() + (endPoint.get_y() - startPoint.get_y()) * factor,
          startPoint.get_z() + (endPoint.get_z() - startPoint.get_z()) * factor
        );
        intermediatePoints.push_back(intermediatePoint);
    }
    
    return intermediatePoints;
}

double Ecef::calculate_curvature_from_lat(double latitude) {

  double slatitude = sin(latitude);
  double ret_val = WGS84_A / sqrt(1.0 - (pow(WGS84_E,2)) * pow(slatitude,2));

  return ret_val;

}