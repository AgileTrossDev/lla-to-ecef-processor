# LLA-to-ECEF Conversion
Coding challenge to ingest LLA Coorindates, while converting them to ECEF, which then can be used to evalute the velocity vector at two given times given in seconds from Unix Epoch.  Output should go to stdout. 


## Usage

## Compilation

## Features
- Ingest CSV of LLA Coordinates
- Convert LLA to ECEF
- Calculate ECEF velocitgy at time points within the Data.
- Interpolate ECEF velocity for any request time

### Ingest CSV of LLA Coordinates
Application will ingest a comma-separated value file containing LLA Coordinates containing the following collumns **Time**, **Latitude**, **Longitude**, **Atitude**.   Time is from the Unix Epoch in seconds, where the Latitude and Longitude coordinates are in degrees.  **The attitude in the CSV is represented in KM**



### Convert LLA to ECEF

X = ( N + h ) cos lat cos long
Y = ( N + h ) cos lat sin long
Z = ((b^2/a^2)N + h ) sin lat

Where...
h = height above ellipsoid(meters)
N = Radius of curvature (meters)

**NOTE** Conversion must also account for the Altitude input being in Kilometers.

### Calculate ECEF velocitgy at time points within the Data.
The expected baseline for calculating ECEF velocities is the discrete ∆Position/∆Time between consecutive points. The velocity at the first input point is defined as 0. 

**NOTE** Velocity at times that don’t coincide with points in the input file should be calculated by linearly interpolating the velocities calculated for input points. If you are using a more complex method for calculating or interpolating velocity, please note how and why you

### Interpolate ECEF velocity for any request time





## Actions
### TODO
 - CMD Line Parameter with Path
 - Push to Github

### Wish List
 - Dameonize
 - TCP Port ingestion
 - Dockerize
 - External Data Store
 - Conversion to Macator Projections


## References
 - Online Conversion Tool: https://www.convertecef.com/
 - Oh...there is wiki for that: https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#From_geodetic_to_ECEF_coordinates



## Background
### Earth-Centered, Earth Fixed (ECEF)
 - Geocentric coordinates
 - Cartesian coordinate frame of reference used in GPS
 - 3-Dimensional (X,Y,Z) in meters.  Fixed  with respect to Earth
 - Origin of Axis is (0,0,0) is EArth Mass Center of gravity

### Latitude, Longitude, Altitude (LLA)
  - Geodetic coordinates



















/*
int main () {

  Results_Vector_T res = ingest("/home/sjackson/Development/project_x/test/big_test_case.csv");

  cout <<"R: " << res.size() << endl;
  res[0].disp();
  int cnt = 0;
  for (auto instance : res) {
    cnt++;
    //*itr.disp();
    instance.disp();
    
  }
cout << cnt;



  return 0;

};
*/