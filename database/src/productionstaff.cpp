#include "productionstaff.h"

ProductionStaff::ProductionStaff(const std::string _firstname,
           const std::string _lastname,
           const boost::gregorian::date _dob) :
    Staff(_firstname, _lastname, _dob, ROLE_PRODUCTION) {
}

ProductionStaff::~ProductionStaff()
{
}
