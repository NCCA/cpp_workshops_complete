#ifndef ADMINSTAFF_H
#define ADMINSTAFF_H

#include "staff.h"

/**
 * @brief The AdminStaff class
 */
class AdminStaff : public Staff
{
public:
    /// Constructor for admin staff
    AdminStaff(const std::string _firstname,
               const std::string _lastname,
               const boost::gregorian::date _dob);

    /// Destructor
    ~AdminStaff();

};

#endif // ADMINSTAFF_H
