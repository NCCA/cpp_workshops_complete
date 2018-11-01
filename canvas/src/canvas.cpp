#include "canvas.h"
#include "lineoperation.h"
#include "rectangleoperation.h"
#include <iostream>

/**
 * @brief Canvas::Canvas Construct a clean canvas.
 */
Canvas::Canvas() : m_currentOperation(Operation::LINE), m_currentColour(Operation::RED)
{
    // Set our end iterator to the end of the list
    m_endIt = m_operations.end();

    // Start by assuming the mouse is not clicked
    m_mouseDown = false;
}

/**
 * @brief Canvas::~Canvas Clear away allocated memory
 */
Canvas::~Canvas() {
    OperationList::iterator it;
    for (it = m_operations.begin(); it != m_operations.end(); ++it) {
        delete (*it);
    }
}

/**
 * @brief Canvas::draw This is where all the magic happens.
 * Due to class polymorphism, we don't care about the type of the operation before
 * we call draw.
 * @param r The SDL_Renderer which we'll use to draw this primitive.
 */
void Canvas::draw(SDL_Renderer* r) {
    OperationList::iterator it;

    // Note that we've modified the draw to iterate only to a specified end
    for (it = m_operations.begin(); it != m_endIt; ++it) {
    //for (it = m_operations.begin(); it != m_operations.end(); ++it) {
        if ((*it)->apply(r) != EXIT_SUCCESS) exit(0);
    }
}

/**
 * @brief Canvas::mouseDown - record the current mouse position so we can create our primitive
 * @param x
 * @param y
 */
void Canvas::mouseDown(int x, int y) {
    m_mouseDown = true;
    Operation *op = NULL;
    switch(m_currentOperation) {
    // Note we'll need to know the operation type when we construct it
    case (Operation::LINE):
        op = new LineOperation(x, y, x, y);
        break;
    case (Operation::RECTANGLE):
        op = new RectangleOperation(x,y, x, y);
        break;
    }
    if (op != NULL) {
        // Set the colour of our newly creater operation
        op->setColour(m_currentColour);
        // Add our operation to the list
        //m_operations.push_back(op);
        addOperation(op);
    }
}

/**
 * @brief Canvas::mouseMove handle rubber banding behaviour if the mouse has moved
 * while the mouse is held down.
 * @param x
 * @param y
 */
void Canvas::mouseMove(int x, int y) {
    if (m_mouseDown) {
        // To implement rubber banding, we'll move the position of the endpoints
        // of the operation by setting them directly
        if (!m_operations.empty()) { // the list should have something in it
            // Set the position of the end point for rubber banding
            OperationList::iterator it = m_endIt;
            --it;
            (*it)->setEndPoint(x,y);
        }
    }
}

/**
 * @brief Canvas::mouseUp: create the operation from the mouse input. You'll need to add new primitives here
 * @param x
 * @param y
 */
void Canvas::mouseUp(int /*x*/, int /*y*/) {
    m_mouseDown = false;
}

/**
 * @brief Canvas::addOperation Handles undo / redo behaviour
 * @param op
 */
void Canvas::addOperation(Operation *op) {
    if (m_endIt == m_operations.end()) {
        // In the regular case we just expect the operation to be added to the end of the list
        m_operations.push_back(op);

        // Update the end iterator
        m_endIt = m_operations.end();
    } else { // In this case, the user has performed some sort of undo - undone operations are lost
        // First delete the data associated with the elements
        OperationList::iterator it;
        for (it = m_endIt; it != m_operations.end(); ++it) {
            delete *it;
        }
        // Now erase the entries from the list
        m_operations.erase(m_endIt, m_operations.end());

        // Add the new operation
        m_operations.push_back(op);

        // Reset the undo / redo iterator to the end of the list
        m_endIt = m_operations.end();
    }
}

/**
 * @brief Canvas::handleKey Allows for rudimentary keyboard input
 * @param c The key that was hit
 */
void Canvas::handleKey(char c, int, int) {
    if (c == 'o') {
        cycleOperation();
    } else if (c == 'c') {
        cycleColour();
    } else if (c == 'u') {
        undo();
    } else if (c == 'r') {
        redo();
    }
}

/**
 * @brief Canvas::cycleOperation: change the currently selected operation
 */
void Canvas::cycleOperation() {
    // Note that this is complicated because I'm using an enumerated type with a typedef
    m_currentOperation = static_cast<Operation::Types>((static_cast<int>(m_currentOperation) + 1) % Operation::numTypes);
}

/**
 * @brief Canvas::cycleColours: cycle through the available colours
 */
void Canvas::cycleColour() {
    // Note that this is complicated because I'm using an enumerated type with a typedef
    m_currentColour = static_cast<Operation::Colour>((static_cast<int>(m_currentColour) + 1) % Operation::numColours);
}

/**
 * @brief Canvas::undo
 */
void Canvas::undo() {
    if (m_endIt == m_operations.begin()) {
        // trying to iterate beyond the start of the list
    } else {
        // Decrement the current end pointer
        --m_endIt;
    }
}

/**
 * @brief Canvas::redo
 */
void Canvas::redo() {
    if (m_endIt == m_operations.end()) {
        // trying to redo past the end of the list
    } else {
        // Increment the pointer
        ++m_endIt;
    }
}
