#ifndef COLLIDE_H
#define COLLIDER_H

#include "assets.h"
#include "graphic_assets.h"
#include <vector>
#include <cmath>

// Forward declaration


struct BonusCollision {
    int char_index;
    float v_x;
};

class Character;


class Collider {
public:
    Collider(){};
    ~Collider() = default;
    
    // Single entry point for collision checking with any Asset type
    
    void resolve_collisions(std::vector<MovableCircle*>& particles, std::vector<Character*>& characters, const std::vector<std::unique_ptr<Asset>>& assets, std::vector<std::unique_ptr<BonusBox>>& bonuses);


    
    // Getters for constraints
    const std::vector<StaticConstraint>& get_static_constraints() const { return static_constraints; }
    const std::vector<StaticConstraint>& get_character_constraints() const { return c_static_constraint; }

private:
    std::vector<StaticConstraint> static_constraints;
    std::vector<DynamicConstraint> dynamic_constraints;
    std::vector<StaticConstraint> c_static_constraint;
    std::vector<BonusCollision> c_static_bonus;
    float penetration_threshold_static {5.0};
    float penetration_threshold_dynamic {0.02};
    float bomb_damages;

    // Type-specific collision handlers
    
    void add_static_contact_constraints(std::vector<MovableCircle*>& particles, std::vector<Character*>& characters, const std::vector<std::unique_ptr<Asset>>& assets, std::vector<std::unique_ptr<BonusBox>>& bonuses);    
    bool check_contact_particle(const MovableCircle& particle, int index, const Asset& asset);
    bool check_contact_characters(const MovableRectangle& character, int index, const Asset& asset);
    bool check_contact_particle_plane(const MovableCircle& particle, int index, const Plane& plane);
    bool check_contact_particle_rectangle(const MovableCircle& particle, int index, const Rectangle& rect);
    void resolve_static_constraints_characters(const std::vector<Character*>& characters);
    void resolve_static_constraints_particles(const std::vector<MovableCircle*>& particles);
    bool check_contact_character_plane(const MovableRectangle& character, int index, const Plane& plane);
    bool check_contact_character_rectangle(const MovableRectangle& c, int index, const Rectangle& rect);
    void handle_character_collision_response(const StaticConstraint& constraint, Character* character);
    void enforce_static_ground_constraints(const StaticConstraint& constraint, MovableAsset& movable_asset);

    void add_dynamic_contact_constraints(const std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters);
    void check_contact_character_projectile(const Character* character1, Character* character2);
    void resolve_dynamic_constraints_particles(const std::vector<MovableCircle*>& particles);
    void enforceDynamicConstraint(const DynamicConstraint& constraint, MovableCircle& p1, MovableCircle& p2);


    void check_contact_character_bonus(Character& character, int i, BonusBox& bonus, std::vector<MovableCircle*>& particles);
    void check_contact_character_bomb(Character& character, int i,Bomb& bomb);


    void resolve_constraints(const std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters);



    
};



#endif