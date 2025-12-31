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
    
    void resolve_collisions(const std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters, const std::vector<std::unique_ptr<Asset>>& assets);

    void resolve_constraints(const std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters);

    void add_static_contact_constraints(const std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters, const std::vector<std::unique_ptr<Asset>>& assets);
    void enforce_static_ground_constraints(const StaticConstraint& constraint, MovableAsset& movable_asset);
    
    // Getters for constraints
    const std::vector<StaticConstraint>& get_static_constraints() const { return static_constraints; }
    const std::vector<StaticConstraint>& get_character_constraints() const { return c_static_constraint; }

private:
    std::vector<StaticConstraint> static_constraints;
    std::vector<StaticConstraint> c_static_constraint;
    // Type-specific collision handlers
    bool check_contact_particle(const MovableCircle& particle, int index, const Asset& asset);
    bool check_contact_characters(const MovableRectangle& character, int index, const Asset& asset);
    bool check_contact_particle_plane(const MovableCircle& particle, int index, const Plane& plane);
    bool check_contact_particle_rectangle(const MovableCircle& particle, int index, const Rectangle& rect);
    void resolve_constraints_characters(const std::vector<Character*>& characters);
    void resolve_constraints_particles(const std::vector<MovableCircle*>& particles);
    bool check_contact_character_plane(const MovableRectangle& character, int index, const Plane& plane);
    bool check_contact_character_rectangle(const MovableRectangle& c, int index, const Rectangle& rect);
    void handle_character_collision_response(const StaticConstraint& constraint, Character* character);


    
};



#endif