#ifndef COLLIDE_H
#define COLLIDER_H

#include "assets.h"
#include <vector>
#include <cmath>

// Forward declaration
class Character;


class Collider {
public:
    Collider(){};
    ~Collider() = default;
    
    // Single entry point for collision checking with any Asset type
    bool checkContact(const MovableAsset& particle, int index, const Asset& asset);
    void resolve_collisions(const std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters, const std::vector<std::unique_ptr<Asset>>& assets);

    void resolve_constraints(const std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters);

    void add_static_contact_constraints(const std::vector<MovableCircle*>& particles, const std::vector<std::unique_ptr<Asset>>& assets);
    void enforce_static_ground_constraints(const StaticConstraint& constraint, MovableAsset& movable_asset);

private:
    std::vector<StaticConstraint> static_constraints;
    std::vector<StaticConstraint> c_static_constraint;
    // Type-specific collision handlers
    bool checkContactCirclePlane(const MovableCircle& particle, int index, const Plane& plane);
    bool checkContactCircleRectangle(const MovableCircle& particle, int index, const Rectangle& rect);
    void resolve_constraints_characters(const std::vector<Character*>& characters);
    void resolve_constraints_particles(const std::vector<MovableCircle*>& particles);
    bool checkContactCharacterPlane(const Character& character, int index, const Plane& plane);
    bool checkContactCharacterRectangle(const MovableRectangle& c, const Rectangle& rect);


    
};



#endif