

#include <iostream>
#include "csv_ingestion.h"
#include "lla.h"


using std::cout;
using std::endl;

// Main Execution of the Assignment
int main (void) {
  cout << "START" << endl;

  auto res = ingest<Lla>("/home/sjackson/Development/project_x/test/big_test_case.csv");

  cout <<"R: " << res.size() << endl;
  res[0].disp();
  int cnt = 0;
  for (auto instance : res) {
    cnt++;
    
    instance.disp();
    
  }
cout << cnt;






  cout << "EXITING" << endl;

  return 0;
}