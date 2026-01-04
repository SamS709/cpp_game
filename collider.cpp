#include "collider.h"
#include "character.h"

void Collider::resolve_collisions(std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters, const std::vector<std::unique_ptr<Asset>>& assets, std::vector<std::unique_ptr<BonusBox>>& bonuses){
    add_static_contact_constraints(particles, characters, assets, bonuses);
    add_dynamic_contact_constraints(particles, characters);
    resolve_constraints(particles, characters);

}

void Collider::add_static_contact_constraints(std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters, const std::vector<std::unique_ptr<Asset>>& assets, std::vector<std::unique_ptr<BonusBox>>& bonuses){
        static_constraints.clear();
        c_static_constraint.clear();
        for (int i = 0; i < particles.size(); ++i) {
            particles[i]->set_has_contact(false);
            for (const auto& asset : assets) {
                auto contact = check_contact_particle(*particles[i], i, *asset);
                if (contact) {
                    particles[i]->set_has_contact(true);
                }
            }
        }
        for (int i = 0; i < characters.size(); ++i) {
            characters[i]->set_has_contact(false);
            for (const auto& asset : assets) {
                auto contact = check_contact_characters(*characters[i], i, *asset);
                if (contact) {
                    characters[i]->set_has_contact(true);
                }
            }
            for (auto &bonus : bonuses) {
                check_contact_character_bonus(*characters[i], *bonus, particles);
                
            }
        }
        
}


bool Collider::check_contact_particle(const MovableCircle& particle, int index, const Asset& asset) {
    if (const Plane* plane = dynamic_cast<const Plane*>(&asset)) {
        return check_contact_particle_plane(particle, index, *plane);
        
    }
    else if (const Rectangle* rect = dynamic_cast<const Rectangle*>(&asset)) {
        return check_contact_particle_rectangle(particle, index, *rect);
    }
    return false;
}

bool Collider::check_contact_particle_plane(const MovableCircle& particle, int index, const Plane& plane) {
    const Vec2 center = particle.get_pos_expected();
    const float radius = particle.get_radius();
    const Vec2 toCenter = center - plane.get_pos();
    const Vec2 center_real = particle.get_pos();
    const Vec2 toCenter_real = center_real - plane.get_pos(); 
    // Distance along normal
    float distance = toCenter.dot(plane.get_norm());
    float distance_real = toCenter_real.dot(plane.get_norm());
    
    // Only collide if particle is on the correct side
    if (distance_real < 0) return false; 
    
    float penetration = radius - distance;
    if (penetration <= 0) return false; 

    // Finite segment test along plane tangent
    Vec2 tangent = Vec2{-plane.get_norm().y, plane.get_norm().x}; 
    float s = toCenter.dot(tangent);          
    
    StaticConstraint constraint;
    constraint.index = index;

    // Check collision type based on position along plane
    if (s < 0.0) {
        // Left edge: circular cap around init_point
        float dist = (center - plane.get_pos()).length();
        if (dist > radius) return false;
        
        // Recalculate for point collision
        constraint.penetration = radius - dist;
        constraint.normal = (center - plane.get_pos()).normalized();
        constraint.contactPoint = center - constraint.normal * radius;
        
    } else if (s > plane.get_w()) {
        // Right edge: circular cap around end_point
        Vec2 end_point = plane.get_pos() + tangent * plane.get_w();
        float dist = (center - end_point).length();
        if (dist > radius) return false;
        
        // Recalculate for point collision
        constraint.penetration = radius - dist;
        constraint.normal = (center - end_point).normalized();
        constraint.contactPoint = center - constraint.normal * radius;
        
    } else {
        // Flat face collision: use original plane normal
        constraint.normal = plane.get_norm();
        constraint.penetration = penetration;
        constraint.contactPoint = center - plane.get_norm() * radius;
    }
    // constraint.penetration *=0.99;
    static_constraints.push_back(constraint);
    return true;
}

bool Collider::check_contact_particle_rectangle(const MovableCircle& particle, int index, const Rectangle& rect) {
    bool c0 = check_contact_particle_plane(particle, index, *rect.plane_x_0);
    bool c1 = check_contact_particle_plane(particle, index, *rect.plane_x_1);
    bool c2 = check_contact_particle_plane(particle, index, *rect.plane_y_0);
    bool c3 = check_contact_particle_plane(particle, index, *rect.plane_y_1);
    return (c0 || c1 || c2 || c3);
    
}


bool Collider::check_contact_characters(const MovableRectangle& character, int index, const Asset& asset){
    if (const Plane* plane = dynamic_cast<const Plane*>(&asset)) {
        return check_contact_character_plane(character, index, *plane);
        
    }
    else if (const Rectangle* rect = dynamic_cast<const Rectangle*>(&asset)) {
        return check_contact_character_rectangle(character, index, *rect);
    }
    return false;
}


bool Collider::check_contact_character_plane(const MovableRectangle& character, int index, const Plane& plane) {
    // Get character bounding box using expected position (same as particles)
    const Vec2 char_pos(character.get_x_expected(), character.get_y_expected());
    const float w = character.get_w();
    const float h = character.get_h();
    
    // Four corners of the character's bounding box
    Vec2 corners[4] = {
        Vec2(char_pos.x, char_pos.y-h),                             // Top-left
        Vec2(char_pos.x + w/2.0, char_pos.y-h),     // Top-right
        Vec2(char_pos.x, char_pos.y),     // Bottom-left
        Vec2(char_pos.x + w/2.0, char_pos.y)  // Bottom-right
    };
    
    // Find the corner with maximum penetration
    float max_penetration = 0.0f;
    int deepest_corner = -1;
    const float max_allowed_penetration = 25;  // Ignore corners that are too far behind
    
    for (int i = 0; i < 4; ++i) {
        Vec2 toCorner = corners[i] - plane.get_pos();
        float distance = toCorner.dot(plane.get_norm());
        
        // Penetration is positive when the corner is past the plane surface
        // For a plane pointing up (norm = (0,-1)), a corner below it has negative distance
        float penetration = -distance;
        
        // Only consider corners that penetrate the plane within reasonable bounds
        // Ignore corners that are too far behind (prevents pushing when jumping over walls)
        if (penetration <= 0 || penetration > max_allowed_penetration) continue;
        
        // Check if corner is within the plane segment bounds
        Vec2 tangent(-plane.get_norm().y, plane.get_norm().x);
        float s = toCorner.dot(tangent);
        
        // Plane length is always stored in w (see Plane constructor)
        float plane_length = plane.get_w();
        
        if (s >= 0 && s <= plane_length) {
            // Corner is within plane segment - keep track of maximum
            if (penetration > max_penetration) {
                max_penetration = penetration;
                deepest_corner = i;
            }
        }
    }
    
    // No collision if no corner penetrates
    if (deepest_corner == -1) return false;

    StaticConstraint constraint;
    constraint.index = index;
    constraint.normal = plane.get_norm();
    constraint.penetration = max_penetration * 0.99;  
    constraint.contactPoint = corners[deepest_corner];
    c_static_constraint.push_back(constraint);
    return true;
}

void Collider::check_contact_character_bonus(const MovableRectangle& character, BonusBox& bonus, std::vector<MovableCircle*>& particles) {
    const Vec2 char_pos(character.get_x_expected(), character.get_y_expected());
    const float char_w = character.get_w() ; 
    const float char_h = character.get_h();
    
    float char_left = char_pos.x - char_w / 2.0;
    float char_right = char_pos.x + char_w / 2.0;
    float char_top = char_pos.y - char_h;
    float char_bottom = char_pos.y;
    
    float rect_left = bonus.get_x();
    float rect_right = bonus.get_x() + bonus.get_w();
    float rect_top = bonus.get_y();
    float rect_bottom = bonus.get_y() + bonus.get_h();
    
    // Check for AABB overlap
    bool overlap_x = char_right > rect_left && char_left < rect_right;
    bool overlap_y = char_bottom > rect_top && char_top < rect_bottom;
    
    if (!overlap_x || !overlap_y) return;
    bonus.activate(character.get_v_x(), particles);
}

bool Collider::check_contact_character_rectangle(const MovableRectangle& character, int index, const Rectangle& rect) {
    const Vec2 char_pos(character.get_x_expected(), character.get_y_expected());
    const float char_w = character.get_w() ; 
    const float char_h = character.get_h();
    
    float char_left = char_pos.x - char_w / 2.0;
    float char_right = char_pos.x + char_w / 2.0;
    float char_top = char_pos.y - char_h;
    float char_bottom = char_pos.y;
    
    float rect_left = rect.get_x();
    float rect_right = rect.get_x() + rect.get_w();
    float rect_top = rect.get_y();
    float rect_bottom = rect.get_y() + rect.get_h();
    
    // Check for AABB overlap
    bool overlap_x = char_right > rect_left && char_left < rect_right;
    bool overlap_y = char_bottom > rect_top && char_top < rect_bottom;
    
    if (!overlap_x || !overlap_y) return false;
    
    // Calculate penetration depths on each axis
    float penetration_left = char_right - rect_left;    
    float penetration_right = rect_right - char_left;   
    float penetration_top = char_bottom - rect_top;     
    float penetration_bottom = rect_bottom - char_top;  
    
    float penetration_x = std::min(penetration_left, penetration_right);
    float penetration_y = std::min(penetration_top, penetration_bottom);
    
    
    StaticConstraint constraint;
    constraint.index = index;
    
    // Prioritize penetrationalong y to prevent from big bouncing along x
    if (penetration_x < 0.1*penetration_y) {
        constraint.penetration = penetration_x * 0.99;
        if (penetration_left < penetration_right) {
            // Character is penetrating from the left
            constraint.normal = Vec2(-1.0f, 0.0f);
            constraint.contactPoint = Vec2(char_right, (char_top + char_bottom) / 2.0f);
        } else {
            // Character is penetrating from the right
            constraint.normal = Vec2(1.0f, 0.0f);
            constraint.contactPoint = Vec2(char_left, (char_top + char_bottom) / 2.0f);
        }
    } else {
        constraint.penetration = penetration_y * 0.99;
        if (penetration_top < penetration_bottom) {
            // Character is penetrating from the top
            constraint.normal = Vec2(0.0f, -1.0f);
            constraint.contactPoint = Vec2((char_left + char_right) / 2.0f, char_bottom);
        } else {
            // Character is penetrating from the bottom
            constraint.normal = Vec2(0.0f, 1.0f);
            constraint.contactPoint = Vec2((char_left + char_right) / 2.0f, char_top);
        }
    }
    
    c_static_constraint.push_back(constraint);
    return true;
}


void Collider::resolve_static_constraints_characters(const std::vector<Character*>& characters){
    int solver_iterations = 1;
    for (int iter = 0; iter < solver_iterations; ++iter) {
        for (const auto& constraint : c_static_constraint) {
            Character* character = characters[constraint.index];
            enforce_static_ground_constraints(constraint, *character);
            
            handle_character_collision_response(constraint, character);
        }
    }
}


void Collider::resolve_static_constraints_particles(const std::vector<MovableCircle*>& particles) {
    int solver_iterations = 1;
    int its;
  
    for (const auto& constraint : static_constraints) {
        its = constraint.penetration < penetration_threshold_static ? 1 : solver_iterations;
        for (int it = 0; it < its; it++) {
            enforce_static_ground_constraints(constraint, *particles[constraint.index]);
        }
    }
    
}

void Collider::enforce_static_ground_constraints(const StaticConstraint& constraint, MovableAsset& movable_asset) {
        movable_asset.update_expected_pos_collision(constraint.normal * constraint.penetration);
}

void Collider::handle_character_collision_response(const StaticConstraint& constraint, Character* character) {
    // Get velocity components along collision normal
    Vec2 velocity = character->get_v();
    float velocity_along_normal = velocity.dot(constraint.normal);
    
    // Check if moving into the surface (negative velocity along normal)
    if (velocity_along_normal < 0) {
        
        // Check if this is a ground collision (normal pointing mostly upward)
        Vec2 up_direction(0, -1);
        float alignment_with_up = constraint.normal.dot(up_direction);
        
        // If collision is with ground (normal aligned with up) and character is jumping
        if (alignment_with_up > 0.7f && character->get_jumping()) {
            character->set_jumping(false);
            
            // Resume moving if direction key is still pressed
            if (character->get_right_pressed() || character->get_left_pressed()) {
                character->set_moving(true);
            }
        }
    }
}

void Collider::add_dynamic_contact_constraints(const std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters) {
    dynamic_constraints.clear();
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            Vec2 diff = particles[j]->get_pos_expected() - particles[i]->get_pos_expected();
            float distance = diff.length();
            float minDist = particles[i]->get_radius()  + particles[j]->get_radius() ;
            float penetration = minDist - distance;
            
            if (penetration > 0 && distance > 0.0001f) {
                DynamicConstraint constraint;
                constraint.particleIndex1 = i;
                constraint.particleIndex2 = j;
                constraint.normal = diff.normalized();
                constraint.penetration = penetration;
                dynamic_constraints.push_back(constraint);
                
                particles[i]->set_has_contact(true);
                particles[j]->set_has_contact(true);
            }
        }
    }
}

void Collider::resolve_dynamic_constraints_particles(const std::vector<MovableCircle*>& particles){
    int solver_iterations = 1;
    int its;
    for (const auto& constraint : dynamic_constraints) {
        its = constraint.penetration < penetration_threshold_dynamic ? 1 : solver_iterations;
        for (int it = 0; it < its; it++){
            enforceDynamicConstraint(constraint, *particles[constraint.particleIndex1], *particles[constraint.particleIndex2]);
        }
    } 
}


void Collider::enforceDynamicConstraint(const DynamicConstraint& constraint, MovableCircle& p1, MovableCircle& p2) {
        float totalInvMass = p1.get_inv_mass() + p2.get_inv_mass();
        if (totalInvMass < 0.0001f) return;
        
        float correction1 = p1.get_inv_mass() / totalInvMass;
        float correction2 = p2.get_inv_mass() / totalInvMass;
        
        p1.update_expected_pos_collision(constraint.normal * (-constraint.penetration * correction1));
        p2.update_expected_pos_collision(constraint.normal * (constraint.penetration * correction2));
}

void Collider::resolve_constraints(const std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters) {
    resolve_static_constraints_particles(particles);
    resolve_static_constraints_characters(characters);
    resolve_dynamic_constraints_particles(particles);
}
