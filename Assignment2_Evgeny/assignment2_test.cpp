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

int main()
{
    OpenAddMap map(OpenAddMap::ProbingScheme::LINEAR);
    bool success = true;

    //test insert
    map.debug();
    success = map.insert(123);
    map.debug();
    success = success && map.insert(1234);
    map.debug();
    success = success && map.insert(12);
    map.debug();
    success = success && !map.insert(123);

    if (success)
        std::cout << "insert ok" << std::endl;
    else
        std::cout << "insert broken" << std::endl;

    //test remove
    success = !map.remove(125);
    success = success && map.remove(12);
    map.debug();
    success = success && !map.remove(12);

    if (success)
        std::cout << "remove ok" << std::endl;
    else
        std::cout << "remove broken" << std::endl;

    //test find
    success = map.find(123);
    success = success && !map.find(125);
    success = success && !map.find(12);

    if(success)
        std::cout << "find ok" << std::endl;
    else
        std::cout << "find broken" << std::endl;

    // test numElem
    size_t elem = map.numElem();

    if (elem == 2)
        std::cout << "numElem ok" << std::endl;
    else
        std::cout << "numElem broken" << std::endl;

    // test size and load
    size_t s = map.size();
    float load = static_cast<float>(elem)/s;

    if (load > 0.24 && load < 0.76)
        std::cout << "load ok" << std::endl;
    else
        std::cout << "load out of range" << std::endl;

    return 0;
}

