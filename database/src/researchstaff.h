#ifndef RESEARCHSTAFF_H
#define RESEARCHSTAFF_H

#include "staff.h"

/**
 * @brief The ResearchStaff class
 */
class ResearchStaff : public Staff
{
public:
    /// Construct a research staff member
    ResearchStaff(const std::string _firstname,
                  const std::string _lastname,
                  const boost::gregorian::date _dob);
    ~ResearchStaff();

};

#endif // RESEARCHSTAFF_H
