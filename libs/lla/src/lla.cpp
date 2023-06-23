#include<sstream>
#include<iostream>
#include<iomanip>

#include "lla.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::stof; 
using std::atof; 
using std::setprecision;

// Displays a Record
void Lla::disp() {
   cout <<  setprecision(17) << "Lla: " << time << " "<<  setprecision(100) << x << " " << y << " " <<  z << " " << endl;
}

// Parses the line into a time stamp and 3 distinct Floats and builds a new Lla.    
Lla Lla::build_from_string(string line) {
  stringstream s(line);
  string string_val;
  Lla new_record;
  int index = 0;
  while (getline(s, string_val, ',')) {    
    
   // cout << "|" << string_val << "|" << endl;
    switch (index) {
      case 0:
        new_record.time = atof(string_val.c_str());
      case 1:
        new_record.x = atof(string_val.c_str());
      case 2:
        new_record.y = atof(string_val.c_str());
      case 3:
        new_record.z = atof(string_val.c_str());
    }
    index +=1;
  }

  //new_record.disp();
  return new_record;
}
