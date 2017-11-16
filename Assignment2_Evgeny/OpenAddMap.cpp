#include <vector>
#include <iostream>
#include "OpenAddMap.h"

// Return load factor
float OpenAddMap::loadFactor() { return (float)n_elements/table_size; }

// Check if an integer is a prime number.
bool OpenAddMap::isPrime(unsigned int num)
{
    if (num == 2 || num == 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    else
    {
        for (int i = 5; i * i <= num; i += 6)
        {
            if (num % i == 0 || num % (i + 2) == 0) return false;
        }
        return true;
    }
}

// Returns a hash value of an integer, depending on the probing scheme;
void OpenAddMap::hash(unsigned &iteration, unsigned &key, unsigned const &value)
{
    switch(scheme)
    {
        // Linear probing: h(v) = (v mod m + i) mod m
        // h(v) - hash function, v = value, m = table size, i = iteration
        case 0:
        {
            key = (value % table_size + iteration) % table_size;
            break;
        }
        // Quadratic probing: h(v) = v mod m + i*i
        case 1:
        {
            /* It is difficult to guarantee that insertions will succeed
             * when the load factor is > 0.5. Because of that we need to consider
             * the number of iterations. If the number > m then we resize the table
             * and try to insert the element in new bigger table
             */
            if (iteration > table_size)
            {
                resize_table(true);
                iteration = 0;
                key = value % table_size;
            }
            else
            {
                key = value % table_size + iteration * iteration;
            }
            break;
        }
        /* Double hashing: h(v) = (v mod m + i*(v mod m_2 + 1)) mod m;
         * m_2 is a prime number less than m;
         * We add 1 to ensure that the step is not 0, otherwise there is a chance
         * of an endless loop
         */
        case 2:
        {
            key = ((value % table_size) + iteration * ((value % hash_2) + 1)) % table_size;
            break;
        }
    }
}

// Resize table, used if load factor > 0.75 or < 0.25, also used if the insertion
// with quadratic method is not successful (using force = true)
void OpenAddMap::resize_table(bool force = false)
{
    // Define new size and save the current size to use with 2nd hash function;
    // Increase the size
    if (loadFactor() > 0.75 || force)
    {
        hash_2 = table_size;
        table_size = table_size * 2 + 1;
        while(!isPrime(table_size))
        {
          table_size += 2;
        }
    }
    // Reduce the size
    else
    {
        table_size = n_elements * 2 + 1;
        // Ensure that the new size is prime
        while(!isPrime(table_size))
        {
          table_size += 2;
        }
        hash_2 = table_size - 2;
        while(!isPrime(hash_2) && hash_2 != 1)
        {
          hash_2 -= 2;
        }
    }

    std::vector<unsigned> new_map(table_size);
    std::fill (new_map.begin(), new_map.end(), -2u);
    // Rehash the old table - add all values to the new table using hashing
    unsigned int key;
    for (unsigned int value : map)
    {
        if (value == -2u || value == -1u) continue;
        key = value % table_size;
        unsigned int iteration (0);
        while(true)
        {
            hash(iteration, key, value);
            if (new_map[key] == -2u)
            {
                new_map[key] = value;
                break;
            }
            ++iteration;
        }
    }
    // Resize
    map.resize(table_size);
    map = new_map;
}

enum class ProbingScheme : unsigned
{
    LINEAR,
    QUADRATIC,
    DOUBLE
};

// Constructor, ititialize the member values and define the probing scheme;
// We always start with table size 3 filled with "empty flags" -2u
OpenAddMap::OpenAddMap(ProbingScheme const n)
{
    map.resize(3);
    std::fill (map.begin(), map.end(), -2u);
    table_size = 3;
    n_elements = 0;
    hash_2 = 2;
    switch(n)
    {
        case ProbingScheme::LINEAR:
        {
            scheme = 0;
            break;
        }
        case ProbingScheme::QUADRATIC:
        {
            scheme = 1;
            break;
        }
        case ProbingScheme::DOUBLE:
        {
            scheme = 2;
            break;
        }
    }
}

unsigned int OpenAddMap::size() const { return table_size; }

unsigned int OpenAddMap::numElem() const { return n_elements; }

// Insert a new element into the table. On success return true.
// If element was already in the table return false.
bool OpenAddMap::insert(unsigned const int value)
{
    std::cout << "Inserting " << value << std::endl;
    unsigned int key = value % table_size;
    unsigned int iteration (0);
    while(true)
    {
        hash(iteration, key, value);
        if (map[key] == value)
        {
            std::cout << value << " already in the table! " << value << std::endl;
            return false;
        }
        if (map[key] == -2u || map[key] == -1u)
        {
            map[key] = value;
            ++n_elements;
            if (loadFactor() > 0.75 ) resize_table();
            std::cout << "Inserting successful!" << std::endl;
            std::cout << "# probes: " << iteration << std::endl;
            return true;
        }
        ++iteration;
    }
}

// Search for entry in the table. If found return true otherwise false.
bool OpenAddMap::find(unsigned const int value)
{
    std::cout << "Searching " << value << std::endl;
    unsigned int key = value % table_size;
    unsigned int iteration (0);
    while(true)
    {
        hash(iteration, key, value);
        if (map[key] == value)
        {
            std::cout << "Value found!" << std::endl;
            std::cout << "# probes: " << iteration << std::endl;
            return true;
        }
        if (map[key] == -2u)
        {
            std::cout << "Value not found!" << std::endl;
            return false;
        }
        ++iteration;
    }
}

/* Delete entry from table. If deleted return true.
 * If element was not in the table return false.
 * Use -1u flag to indicate the deleted element position
 */
bool OpenAddMap::remove(unsigned const int value)
{
    std::cout << "Removing " << value << std::endl;
    unsigned int key = value % table_size;
    unsigned int iteration (0);
    while(true)
    {
        hash(iteration, key, value);
        if (map[key] == value)
        {
            map[key] = -1u;
            --n_elements;
            // Resize only of the size of the table > 3
            if (loadFactor() < 0.25 && table_size > 3) resize_table();
            std::cout << "Value removed!" << std::endl;
            std::cout << "# probes: " << iteration << std::endl;
            return true;
        }
        if (map[key] == -2u)
        {
            std::cout << "Value not found!" << std::endl;
            return false;
        }
        ++iteration;
    }
}

// Print the current table, use . as empty cell, NIL as previously deleted element
void OpenAddMap::debug()
{   
    std::cout << std::endl;
    std::cout << "Current table:" << std::endl;
    std::cout << "Probing scheme : " << scheme << std::endl;
    std::cout << "# Elements : " << n_elements << std::endl;
    std::cout << "Table size : " << table_size << std::endl;
    std::cout << "Load factor : " << loadFactor() << std::endl;
    std::cout << "*==========" << std::endl;
    for (unsigned int value : map)
    {

        if (value == -2u) std::cout << "." << std::endl;
        else if (value == -1u) std::cout << "NIL" << std::endl;
        else std::cout << value << std::endl;
    }
    std::cout << "==========*" << std::endl;
    std::cout << std::endl;
}
