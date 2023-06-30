// Description: LLA-To-Ecef Velocity Vector
// Author: Scott Jackson
//
// Build: make
//
// Execute: ./bin/app <request time> <path to input>
//  ex: ./bin/app 1532334000 test/big_test_case.csv
//  ex: ./bin/app 1532335268 test/big_test_case.csv
//
// Execution Logic
//  - Ingests the CSV file of LLA Data
//  - Convert LLA to ECEF using WGS84_A and converting units as necessary (Radian/Degrees Meter/KM)
//  - Sort ECEF Data
//  - Calculate ECEF velocity for the ECEF Data collection.
//  - Check to see if requested time is already in collection and if so, return the veclociy and exit.
//  - Else we need to find the two ECEF records on either side of the requested time stamp for Interlopation  
//  - Perform interpolation between the two points to calculate the ECEF velocity at the requested time

// Standard Library
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <vector>

// Application Specific
#include "csv_ingestion.h"
#include "ecef.h"

// Using these specific stdlib functions
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::sort;
using std::stod;
using std::lower_bound;   // Essentially binary search
using std::upper_bound;   // Essentially binary search
using std::setprecision;
using std::vector;
using std::distance;

// Structure containing the parameters for the Job Request
struct JobRequest {
    std::string ingest_path = "";
    double request_time;    
    double interpolate_start;
    double interpolate_end;

    void disp();
};

// Prints out the Job Request
void JobRequest::disp() {
  cout << "\n\nJob Request"
    << setprecision(100)
    << "\nRequest Time: " << request_time    
    << "\nInput: " << ingest_path 
    << "\nInterpolate Start: " << interpolate_start
    << "\nInterpolate End: " << interpolate_end 
    << endl;
}

// Verifies file exists
bool file_exists_check(const string& file_path) {
  ifstream file(file_path);
  return file.good();
}

// Builds the Job Request from the CLI Parameters
JobRequest build_job_request(int argc, char* argv[]) {
  JobRequest request;

  // We expect two input parameters, along with the applicatoin name
  if (argc != 3) {
    cout << "Usage: ./bin/app <timestamp_start> <path to CSV input file>" << endl;
    cout << "ex: ./bin/app 1532335268 test/big_test_case.csv" << endl;
    // TODO: Consider Default values
    return request;
  }

  request.request_time = stod(argv[1]);  
  request.ingest_path = argv[2]; 

  return request;

}

// Validates the Job Request Params are valid
bool validate_job_request(const JobRequest& job_request) {
  if (job_request.ingest_path == "") {
    // Job Request not created correctly
    return  false;
  } else if (!file_exists_check(job_request.ingest_path)) {
    cout << "Invalid CSV Path: " << job_request.ingest_path << endl;
    return false;  
  } else if (job_request.request_time < 0 ){
    cout << "Request Time must be a positive value." << endl;
    return false;
  }

  return true;
}

// Function that finds the nearest time stamps for a templated object within a vector. The before_time
// boolean flag indicates if we are finding the time before or after the requested_time
//
// Big O: log(n)
// NOTE: Template must support get_time
template<typename T>
double find_nearest_timestamp(const vector<T>& time_stamped_data, double requested_time, bool before_time) {
    
  if (before_time) {
    auto it = lower_bound(time_stamped_data.begin(), time_stamped_data.end(), requested_time,
                              [](const T& a, int requested_time) {                                
                                  return a.get_time() < requested_time;
                              });  

    if (it == time_stamped_data.begin()) {
        // Requested time is smaller than all timestamps in the vector
        cout << "Request Time occurs before first record in data set" << endl;
        return -1; 
    } else if (it == time_stamped_data.end()) {
        // Requested time is greater than or equal to all timestamps in the vector
        cout << "Request Time occurs after last record in data set" << endl;
        return time_stamped_data.back().get_time();
    } else {
        // Found a timestamp before or just before the requested time
        cout << "Found Time Stamp" << endl;
        return (it - 1)->get_time();
    }
  } else  {
    auto it = std::upper_bound(time_stamped_data.begin(), time_stamped_data.end(), requested_time,
                              [](int requested_time, const T& a) {
                                  return requested_time < a.get_time();
                              });

    // Requested time is greater than all timestamps in the vector, in which
    // case the iterator will be at vector end, or the iterator will be 
    // the selected time stamp.

    if (it == time_stamped_data.end()) {
        
        return -1; 
    } else {
        // Found a timestamp equal to or just after the requested time
        return it->get_time();
    }
  }
  return -1;
}

// Returns the Tempalted Object in the vector for the requested time.  If the time was not found,
// then a new empty object is returned
//
// Big O: log(n)
template<typename T>
T search_for_instance_by_request_time(const vector<T>& time_stamped_data, double request_time) {

    auto it = lower_bound(time_stamped_data.begin(), time_stamped_data.end(), request_time, [](const T& obj, double time_stamp) {
        return obj.get_time() < time_stamp;
    });

    if (it != time_stamped_data.end() && it->get_time() == request_time) {
        // Object with the target timestamp found     
        return *it;
    } else {
        cout << "Record for request time not found: " << request_time << endl;
        // Object with the target timestamp not found
        // You can return a default-constructed object or throw an exception,
        // depending on your use case.
        // Here, we'll return a default-constructed object.
        return T(); // Assuming MyObject has a default constructor
    }
}


// Caculates the velocity of all records of the templated class in the positions vector and
// updates the class with it's velocity. 
//
// NOTE: Template must support calculate_the_velocity
template<typename T>
void calculate_velocity(vector<T>& positions) {
  for (size_t i = 1; i < positions.size(); ++i) {
      T& current_position = positions[i];
      const T& previous_position = positions[i - 1];
      T::calculate_the_velocity(previous_position, current_position);        
  }
}

// Find velocity for a requested time by finding interpolated position, and then caculate velocity
template<typename T>
T interpolate(const T& position_1, const T& position_2, double request_time) {
  
  cout << "\n\nVelocity interpolation being performed..." << endl;
  cout << "Position 1:" << endl;
  position_1.disp();

  cout << "Position 2:" << endl;
  position_2.disp();

  // Calculate the time difference between the two closest known positions
  double time_diff = position_2.get_time() - position_1.get_time();

  cout << "Time diff between closest known positions: " << position_2.get_time() << " - " << position_1.get_time() << " = " << time_diff << endl;
  
  // Calculate the interpolation weights based on the time difference between the known positions. 
  // The weight for the earlier measurement should be proportional to the remaining time until the
  // requested time, while the weight for the later measurement should be proportional to the elapsed
  // time since the earlier measurement.
  double weight_later = (request_time - position_1.get_time()) / time_diff;
  double weight_earlier = 1.0 - weight_later;

  cout << "Interpolation Weights: " << weight_earlier << " | " << weight_later << endl;

  // Interpolate the new positions
  T result(request_time,
   weight_earlier * position_1.get_x() + weight_later * position_2.get_x(),
   weight_earlier * position_1.get_y() + weight_later * position_2.get_y(),
   weight_earlier * position_1.get_z() + weight_later * position_2.get_z());
 
  // Now calulate the velocity from position 1 to the interpolated
  Ecef::calculate_the_velocity(position_1, result);

  return result;
}

// Main Execution of the Assignment
//  - Validate CLI parameters
//  - Ingests CSV LLA input and generate vector of ECEF records.
//  - Sort ECEF Records by Time Stamp
//  - Find the time stamps we are using to interpolate
//  - Calculate velocity across the vector 
//  - Interpolate
int main(int argc, char* argv[]) {
  
  // Validate execution Parameters
  auto job_request = build_job_request(argc, argv);
  if (!validate_job_request(job_request)) {
    cout << "** Invalid Job Request. Exiting **" << endl;
    return -1;
  }

  // Ingest the CSV File of LLA Data, converting it to ECEF parameters and then sort it
  auto ecef_data = ingest<Ecef>(job_request.ingest_path);
  sort(ecef_data.begin(), ecef_data.end(), Ecef::compare_by_time);

  // Calculate the Velocities across the entire data set
  calculate_velocity(ecef_data);
  
  // Find the Timestamps we can use for Iterpolation
  job_request.interpolate_start = find_nearest_timestamp(ecef_data, job_request.request_time, true);
  job_request.interpolate_end = find_nearest_timestamp(ecef_data, job_request.request_time, false);
  job_request.disp();

  auto position_1 = search_for_instance_by_request_time(ecef_data,job_request.interpolate_start);
  auto position_2 = search_for_instance_by_request_time(ecef_data,job_request.interpolate_end);
  auto result  = interpolate(position_1, position_2, job_request.request_time);

  cout << "\n\nResulting ECEF:" << endl;
  result.disp();

  cout << "EXITING" << endl;

  return 0;
}