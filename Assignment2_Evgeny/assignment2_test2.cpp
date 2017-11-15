/*
* This file is a minimal sanity check example to make sure that you correctly
* implement the desired interface.
* It must compile with:
* "g++ -std=c++14 -Wall -Wextra -Werror -O3 -DNDEBUG -pedantic assignment2_test.cpp OpenAddMap.cpp -o assignment2_test"
*
*/

#define _GLIBCXX_USE_C99 1
#include "OpenAddMap.h"
#include <iostream>
#include <cstdlib>
#include <iostream>

int main()
{
    OpenAddMap map(OpenAddMap::ProbingScheme::LINEAR);
    bool success = true;
    OpenAddMap map2(OpenAddMap::ProbingScheme::QUADRATIC);
    bool success2 = true;
    OpenAddMap map3(OpenAddMap::ProbingScheme::DOUBLE);
    bool success3 = true;
    map.debug();
    int k;
    for(int i=0;i<20;i++)
    { k= std::rand();
    //  k= 2*i; // for a list of even numbers
    //  k=i; //for a list from 0 to 19
      success = success && map.insert(k);
      success2 = success2 && map2.insert(k);
      success3 = success3 && map3.insert(k);

  }
  map.debug();
  map2.debug();
  map3.debug();
    if (success)
        std::cout << "insert ok" << std::endl;
    else
        std::cout << "insert broken" << std::endl;

    if (success2)
        std::cout << "insert ok" << std::endl;
    else
        std::cout << "insert broken" << std::endl;


            if (success3)
                std::cout << "insert ok" << std::endl;
            else
                std::cout << "insert broken" << std::endl;



    return 0;

}
