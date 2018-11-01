#ifndef STAFF_H
#define STAFF_H

// The base class of Staff
#include "entity.h"

// Needed for date storage
#include <boost/date_time/gregorian/gregorian.hpp>

class Staff : public Entity
{
public:
    /// An enumerated type for the different types of roles available
    typedef enum {
        ROLE_UNDEFINED,
        ROLE_ADMIN,
        ROLE_PRODUCTION,
        ROLE_RESEARCH
    } RoleType;

    /// Constructor
    Staff(const std::string _firstname,
          const std::string _lastname,
          const boost::gregorian::date _dob,
          RoleType _role = ROLE_UNDEFINED);

    /// Destructor
    virtual ~Staff();

    /// Pure virtual print function
    virtual void print();

protected:
    /// Some standard goodies to store with this class
    std::string m_firstname;
    std::string m_lastname;
    boost::gregorian::date m_dob;

    /// A local typedef to identify the type of role of this staff member
    RoleType m_role;


};

#endif // STAFF_H
