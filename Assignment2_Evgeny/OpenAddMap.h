#ifndef OPENADDMAP_H
#define OPENADDMAP_H

#include <vector>

class OpenAddMap
{
private:
    unsigned int scheme;
    std::vector<unsigned int> map;
    unsigned int  table_size;
    unsigned int n_elements;
    unsigned int hash_2;
    
    float loadFactor();
    
    bool isPrime(unsigned int);

    void hash(unsigned &, unsigned &, unsigned const &);
    
    void resize_table(bool);

public:
    // defines which probing scheme are available
    enum class ProbingScheme : unsigned
    {
        LINEAR,
        QUADRATIC,
        DOUBLE
    };

    // Constructor, takes a ProbingScheme as argument
    OpenAddMap(ProbingScheme const);

    // returns the size (reserved slots) of the table.
    unsigned int size() const;
    // returns the number of elements currently in the table (not including NIL markers)
    unsigned int numElem() const;

    // insert a new element into the table. On success return true.
    // If element was already in the table return false.
    bool insert(unsigned const int);

    //search for entry in the table. If found return true otherwise false.
    bool find(unsigned const int);

    // delete entry from table. If deleted return true.
    // If element was not in the table return false.
    bool remove(unsigned const int);
    
    void debug();
};

#endif	// OPENADDMAP_H

