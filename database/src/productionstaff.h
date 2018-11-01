#ifndef PRODUCTIONSTAFF_H
#define PRODUCTIONSTAFF_H

#include "staff.h"

/**
 * @brief The ProductionStaff class
 */
class ProductionStaff : public Staff
{
public:
    /// Construct a production staff member
    ProductionStaff(const std::string _firstname,
                    const std::string _lastname,
                    const boost::gregorian::date _dob);
    ~ProductionStaff();

};

#endif // PRODUCTIONSTAFF_H
