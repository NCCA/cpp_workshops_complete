#include "adminstaff.h"

/**
 * @brief AdminStaff
 * @param _firstname
 * @param _lastname
 * @param _dob
 */
AdminStaff::AdminStaff(const std::string _firstname,
           const std::string _lastname,
           const boost::gregorian::date _dob) :
    Staff(_firstname, _lastname, _dob, ROLE_ADMIN) {
}

/**
 * @brief AdminStaff::~AdminStaff
 */
AdminStaff::~AdminStaff()
{
}
