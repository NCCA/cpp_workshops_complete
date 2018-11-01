#ifndef ENTITY_H
#define ENTITY_H

// Needed to make date/time parsing a doddle
#include <boost/date_time/gregorian/gregorian.hpp>

/**
 * @brief The Entity class - the base class for all database types.
 */
class Entity
{
public:
    /// In case we want to allow for
    typedef enum {
        ENTITY_UNDEFINED,
        ENTITY_STAFF,
        ENTITY_RESOURCE
    } EntityType;

    /// Constructor
    Entity(EntityType _type = ENTITY_UNDEFINED) : m_type(_type) {;}

    /// Destructor
    virtual ~Entity() {;}

    /// Print the contents of this entity to the screen (pure virtual)
    virtual void print() = 0;

protected:
    /// Store the entity type (defined in the list above)
    EntityType m_type;
};

#endif // ENTITY_H
