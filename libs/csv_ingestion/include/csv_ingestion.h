
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>

template <class RecordType>
std::vector<RecordType> ingest(std::string path_to_file);


using std::cout;
using std::endl;
using std::fstream;
using std::string;
using std::ios;
using std::vector;

// Attempts to ingest a CSV File and build a vector the tempalted class.
template <class RecordType>
vector<RecordType> ingest(string path_to_file) {

  vector<RecordType> results;
  string line;

  // Open File Stream to file to ingest
  fstream ingestion_stream;
  ingestion_stream.open(path_to_file, ios::in);
       
  // Read the Data from the stream into a String Vector      
  while (getline(ingestion_stream, line)) {  
    results.push_back(RecordType::build_from_string(line));    
  }
  return results;
};
