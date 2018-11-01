#include "researchstaff.h"

/**
 * @brief ResearchStaff::ResearchStaff
 * @param _firstname
 * @param _lastname
 * @param _dob
 */
ResearchStaff::ResearchStaff(const std::string _firstname,
           const std::string _lastname,
           const boost::gregorian::date _dob) :
    Staff(_firstname, _lastname, _dob, ROLE_RESEARCH) {
}

/**
 * @brief ResearchStaff::~ResearchStaff
 */
ResearchStaff::~ResearchStaff()
{
}
