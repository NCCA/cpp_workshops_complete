#ifndef DATABASE_H
#define DATABASE_H

// Needed for std::string 's
#include <string>

// Needed for STL lists
#include <list>

// Needed for parsing
#include <boost/regex.hpp>

// Needed for shared pointers
#include <boost/shared_ptr.hpp>

// A forward declaration for the type Entity
// class Entity {};

// Headers for the different types of staff entities which can be added to our database
#include "researchstaff.h"
#include "adminstaff.h"
#include "productionstaff.h"

// The EntityList is essentially the storage type for our database
typedef std::list< std::unique_ptr<Entity> > EntityList;

/**
 * @brief The Database class
 * Stores a bunch of entities.
 */
class Database
{
public:
    /// A constructor which takes an input filename
    Database(const std::string &_filename);

    /// A destructor, which wipes clean the database
    ~Database();

    /// Dump the contents of the database to the screen
    void print();

private:
    /// Parse an input file given the filename
    void parse(const std::string &_filename);

    /// Delete the database
    void clear();

    /// The actual data stored with this database
    EntityList m_data;

    /// Regular expressions used to match staff and resources
    static const boost::regex staff_expr, resource_expr;
};

#endif // DATABASE_H
