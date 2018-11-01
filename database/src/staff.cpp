#include "staff.h"
#include <iostream>

/**
 * @brief Staff::Staff
 * @param _firstname
 * @param _lastname
 * @param _dob
 * @param _role
 */
Staff::Staff(const std::string _firstname,
             const std::string _lastname,
             const boost::gregorian::date _dob,
             RoleType _role) :
    Entity(ENTITY_STAFF),
    m_firstname(_firstname),
    m_lastname(_lastname),
    m_dob(_dob),
    m_role(_role) {

}

/**
 * @brief Staff::~Staff
 */
Staff::~Staff() {
}

/**
 * @brief Staff::print prints the standard staff details.
 */
void Staff::print() {
    std::cout << "<type="<<m_role<<" firstname="<<m_firstname.c_str()<<" lastname="<<m_lastname.c_str()<<
                 " dob="<<m_dob.year()<<"-"<<m_dob.month()<<"-"<<m_dob.day()<<">\n";
}
