#include <string>
#include <vector>

class Lla {

  public:
    
    static Lla build_from_string(std::string line);
    void disp(void);
    double time, x,y,z;

  
  
  
};

typedef std::vector<Lla> LlaVector_T;