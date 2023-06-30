# LLA-to-ECEF Conversion
Coding challenge to ingest LLA Coorindates, then converting them to ECEF, which then can be used to evalute the velocity vector at a requested time given in seconds from Unix Epoch.  Output should go to stdout. 

## Usage
Execute: ./bin/app <request time> <path to input>
ex: ./bin/app 1532334000 test/big_test_case.csv
ex: ./bin/app 1532335268 test/big_test_case.csv

## Compilation
make

## Features
- Ingest CSV of LLA Coordinates
- Convert LLA to ECEF
- Calculate ECEF velocity at time points within the Data.
- Interpolate ECEF velocity for any request time

## Execution Flow
- Ingests the CSV file of LLA Data
- Convert LLA to ECEF using WGS84_A and converting units as necessary (Radian/Degrees Meter/KM)
- Sort ECEF Data
- Calculate ECEF velocity for the ECEF Data collection.
- Check to see if requested time is already in collection and if so, return the veclociy and exit.
- Else we need to find the two ECEF records on either side of the requested time stamp for Interlopation  
- Perform interpolation between the two points to calculate the ECEF velocity at the requested time

## Ingest CSV of LLA Coordinates
Application will ingest a comma-separated value file containing LLA Coordinates containing the following collumns **Time**, **Latitude**, **Longitude**, **Atitude**.   Time is from the Unix Epoch in seconds, where the Latitude and Longitude coordinates are in degrees.  **The attitude in the CSV is represented in KM**


## Convert LLA to ECEF

X = ( N + h ) cos lat cos long
Y = ( N + h ) cos lat sin long
Z = ((b^2/a^2)N + h ) sin lat

Where...
h = height above ellipsoid(meters)
N = Radius of curvature (meters)

**NOTE** Conversion must also account for the Altitude input being in Kilometers.

## Calculate ECEF velocitgy at time points within the Data.
The expected baseline for calculating ECEF velocities is the discrete ∆Position/∆Time between consecutive points. The velocity at the first input point is defined as 0. 

**NOTE** Velocity at times that don’t coincide with points in the input file should be calculated by linearly interpolating the velocities calculated for input points. If you are using a more complex method for calculating or interpolating velocity, please note how and why you

### Interpolate ECEF velocity for any request time

## Actions
### TODO
 - Propogate past end of input 
 - More error checking of states
 
### Wish List
 - Dameonize
 - TCP Port ingestion
 - Dockerize
 - External Data Store
 
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
