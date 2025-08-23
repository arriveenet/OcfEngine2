// SPDX - License - Identifier : MIT
#pragma once

namespace ocf {

class Object {
public:
    Object();
    virtual ~Object();

    /**
     * @brief Gets the unique identifier for the object.
     * @return The unique identifier for the object.
     */
    unsigned int getID() const;

    /**
     * @brief  Retains the object, increasing its reference count. This is used to manage the
     * lifetime of the object.
     */
    void retain();

    /**
     * @brief  Releases the object, decreasing its reference count. If the reference count reaches
     * zero, the object is destroyed.
     */
    void release();

    /**
     * @brief  Gets the current reference count of the object.
     * @return The current reference count.
     */
    void autorelease();

private:
    unsigned int m_id;              //!< Object ID
    unsigned int m_referenceCount;  //!< Reference count
};

} // namespace ocf
