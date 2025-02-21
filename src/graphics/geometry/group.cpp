#include "group.hpp"

#include <algorithm>

#include "intersection.hpp"

namespace gfx {
    // Object Inserter (from object ref)
    void Group::addChild(const Object& object)
    {
        auto cloned_object_ptr{ object.clone() };
        cloned_object_ptr->setParent(this);
        m_children.push_back(cloned_object_ptr);
        m_bounds.mergeWithBox(cloned_object_ptr->getLocalSpaceBounds());
    }

    // Copy Assignment Operator
    Group& Group::operator=(const Group& rhs)
    {
        this->setTransform(rhs.getTransform());
        m_children = rhs.m_children;
        this->setParentForAllChildren(this);
        m_bounds = rhs.m_bounds;

        return *this;
    }

    // Move Assignment Operator
    Group& Group::operator=(Group&& rhs) noexcept
    {
        this->setTransform(rhs.getTransform());
        m_children = std::move(rhs.m_children);
        this->setParentForAllChildren(this);
        m_bounds = rhs.m_bounds;

        return *this;
    }

    // Object Inserter (from pointer)
    void Group::addChild(const std::shared_ptr<Object>& object_ptr)
    {
        object_ptr->setParent(this);
        m_children.push_back(object_ptr);
        m_bounds.mergeWithBox(object_ptr->getLocalSpaceBounds());
    }

    // Intersections with Child Object(s) in a Group
    std::vector<Intersection> Group::calculateIntersections(const Ray& transformed_ray) const
    {
        // Check if ray intersects bounding box
        std::vector<Intersection> intersections{ };
        if (!m_bounds.isIntersectedBy(transformed_ray))
            return intersections;

        // Aggregate intersections across all children
        for (const auto& object_ptr : m_children) {
            intersections.append_range(object_ptr->getObjectIntersections(transformed_ray));
        }

        std::sort(intersections.begin(), intersections.end());
        return intersections;
    }

    // Group Object Equivalency Check
    bool Group::areEquivalent(const Object& other_object) const
    {
        const Group& other_group{ dynamic_cast<const Group&>(other_object) };

        // Ensure the number of children in the next layer match for each object
        const size_t num_children{ this->m_children.size() };
        if (num_children != other_group.m_children.size())
            return false;

        // Compare each child object in tree
        for (int i = 0; i < num_children; ++i) {
            if (*this->m_children[i] != *other_group.m_children[i])
                return false;
        }
        return true;
    }

    // Parent Setter Helper Method
    void Group::setParentForAllChildren(Group* const parent_ptr) const
    {
        for (const auto& child_ptr : m_children) {
            child_ptr->setParent(parent_ptr);
        }
    }

    // Group Bounding Volume Calculator
    BoundingBox Group::calculateBounds() const
    {
        BoundingBox new_enclosing_volume{ };

        for (const auto& child_ptr : m_children) {
            new_enclosing_volume.mergeWithBox(child_ptr->getLocalSpaceBounds());
        }
        return new_enclosing_volume;
    }
}