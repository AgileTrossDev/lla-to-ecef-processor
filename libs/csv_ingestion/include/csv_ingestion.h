#ifndef CSV_INGESTION_H
#define CSV_INGESTION_H


#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>

//-------------------------------------------------------------
// Function Definitions
//-------------------------------------------------------------
template <class RecordType>
std::vector<RecordType> ingest(std::string path_to_file);

//-------------------------------------------------------------
// Template Functions that are needed are compilation
//-------------------------------------------------------------

// Attempts to ingest a CSV File and builds an instance of each
// templated class and inserts into a vector of templated class.
template <class RecordType>
std::vector<RecordType> ingest(std::string path_to_file) {

  std::vector<RecordType> results;
  std::string line;

  // Open File Stream to file to ingest
  std::fstream ingestion_stream;
  ingestion_stream.open(path_to_file, std::ios::in);
       
  // Read the Data from the stream, build the templated Record, and
  // insert into return Vector      
  while (getline(ingestion_stream, line)) {  
    results.push_back(RecordType::build_from_string(line));    
  }
  return results;
};
#endif