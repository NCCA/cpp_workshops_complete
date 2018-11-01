#include "database.h"

// Needed for cout and cerr
#include <iostream>

// Needed for reading from the file with ifstream (why not include these in the header?)
#include <fstream>

/// Staff regular expression has code, firstname, lastname, birthdate
static const boost::regex staff_regex("([apr]) ([A-Za-z]+) ([A-Za-z]+) ([0-9/]+)");

/// Resource has code, resourcename, purchasedate
static const boost::regex resource_regex("[0-9]+ ([A-Za-z]+) ([0-9/]+)");

/**
 * @brief Database::Database
 * @param _filename
 */
Database::Database(const std::string &_filename) {
    parse(_filename);
}

/**
 * @brief Database::~Database
 */
Database::~Database() {
    clear();
}

/**
 * @brief Database::clear
 * Erase the contents of the database
 */
void Database::clear() {
    m_data.clear();
}

/**
 * @brief Database::print dumps the database contents to screen
 */
void Database::print() {
    // Iterate over the data in the list. Notice the use of the keyword "auto" here - it is
    // the lazy way of inferring types (slightly easier that EntityList::iterator)
    // It's really powerful (and dangerous IMO): http://en.cppreference.com/w/cpp/language/auto
    for (auto it = m_data.begin(); it != m_data.end(); ++it) {
        (*it)->print();
    }
}

/**
 * @brief Database::parse Parses the input file and inserts the entries into the DB.
 * I've gone with an approach which attempts to parse each line with a set of prebaked regular expressions.
 * This isn't the most efficient method, but is pretty flexible, and demonstrates several nifty concepts from
 * boost which might prove useful.
 * @param _filename A filename to parse
 */
void Database::parse(const std::string &_filename) {
    // This will store our regular expression matching results. Plenty of reading matter here:
    // http://www.boost.org/doc/libs/1_57_0/libs/regex/doc/html/index.html
    boost::match_results<std::string::const_iterator> results;

    // This will store each line from the file
    std::string line;

    // The input file
    std::ifstream infile (_filename.c_str());

    // Sanity check - if not open something went wrong
    if (infile.is_open()) {
        while ( getline (infile,line) ) {
            if (boost::regex_match(line, results, staff_regex)) {
                // Format should be:
                // results[1] code of role (a, p or r)
                std::string code_str = results[1];
                char code = code_str.c_str()[0];

                // results[2] first name
                std::string firstname = results[2];

                // results[3] last name
                std::string lastname = results[3];

                // results[4] date of birth (as a string)
                std::string dob_str = results[4];
                boost::gregorian::date dob(boost::gregorian::from_simple_string(dob_str));

                // Essentially create database entries using this "factory" paradigm
                Entity *ent_ptr;
                switch(code) {
                case 'a': // Handle the admin staff case
                    ent_ptr = new AdminStaff(firstname, lastname, dob); break;
                case 'p': // Handle the production staff case
                    ent_ptr = new ProductionStaff(firstname, lastname, dob); break;
                case 'r': // Handle the research staff case
                    ent_ptr = new ResearchStaff(firstname, lastname, dob); break;
                default:
                    std::cerr << "Shouldn't get here!\n";
                    exit(0);
                }
                // Create a unique ptr from the given pointer. See the documentation for details:
                // http://www.boost.org/doc/libs/1_55_0/doc/html/boost/interprocess/unique_ptr.html
                std::unique_ptr<Entity> ent(ent_ptr);

                // Now push the unique pointer to the back of the list using the move operator:
                // http://en.cppreference.com/w/cpp/utility/move
                m_data.push_back(std::move(ent));
            } else if (boost::regex_match(line, results, resource_regex)) {
                // Implement resource parsing here
            } else {
                std::cout << "Line "<<line.c_str()<<" could not be parsed!\n";
            }
        }
        // Be good and close up the opened file
        infile.close();
    }
}
