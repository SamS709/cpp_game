#include "collider.h"
#include "character.h"

void Collider::resolve_collisions(const std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters, const std::vector<std::unique_ptr<Asset>>& assets){
    add_static_contact_constraints(particles, assets);
    resolve_constraints(particles, characters);

}

void Collider::add_static_contact_constraints(const std::vector<MovableCircle*>& particles, const std::vector<std::unique_ptr<Asset>>& assets){
        static_constraints.clear();
        for (int i = 0; i < particles.size(); ++i) {
            particles[i]->hasContact = false;
            for (const auto& asset : assets) {
                auto contact = checkContact(*particles[i], i, *asset);
                if (contact) {
                    particles[i]->hasContact = true;
                }
            }
        }
}

// void Collider::add_static_contact_constraints(Character& c1, Character& c2, const std::vector<std::unique_ptr<Asset>>& assets){
//         static_constraints.clear();
//         for (int i = 0; i < particles.size(); ++i) {
//             particles[i]->hasContact = false;
//             for (const auto& asset : assets) {
//                 auto contact = checkContact(*particles[i], i, *asset);
//                 if (contact) {
//                     particles[i]->hasContact = true;
//                 }
//             }
//         }
// }

bool Collider::checkContact(const MovableAsset& m_a, int index, const Asset& asset) {
    // Try casting to Plane
    if (const Plane* plane = dynamic_cast<const Plane*>(&asset)) {
        if (const MovableCircle* circle = dynamic_cast<const MovableCircle*>(&m_a)) {
            return checkContactCirclePlane(*circle, index, *plane);
        } 
    }
    // Try casting to Rectangle
    else if (const Rectangle* rect = dynamic_cast<const Rectangle*>(&asset)) {
        if (const MovableCircle* circle = dynamic_cast<const MovableCircle*>(&m_a)) {
            return checkContactCircleRectangle(*circle, index, *rect);
        } else if (const MovableRectangle* c = dynamic_cast<const MovableRectangle*>(&m_a)) {
            return checkContactCharacterRectangle(*c, *rect);
        } 
    }
    // Unknown or unsupported asset type
    return false;
}

bool Collider::checkContactCirclePlane(const MovableCircle& particle, int index, const Plane& plane) {
    const Vec2 center = particle.get_pos_expected();
    const float radius = particle.get_radius();
    const Vec2 toCenter = center - plane.get_pos();

    // Distance along normal - particle must be on the "positive" side of the plane
    float distance = toCenter.dot(plane.get_norm());
    
    // Only collide if particle is on the correct side (distance > 0) and within radius
    if (distance < 0) return false;  // Particle is behind the plane
    
    float penetration = radius - distance;
    if (penetration <= 0) return false;  // Particle is too far away

    // Finite segment test along plane tangent
    Vec2 tangent = Vec2{-plane.get_norm().y, plane.get_norm().x}; // perpendicular to normal
    float s = toCenter.dot(tangent);          // local coord along the plane
    
    StaticConstraint constraint;
    constraint.particleIndex = index;

    // Check collision type based on position along plane
    if (s < 0.0f) {
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
    static_constraints.push_back(constraint);
    return true;
}

bool Collider::checkContactCircleRectangle(const MovableCircle& particle, int index, const Rectangle& rect) {
    bool c0 = checkContactCirclePlane(particle, index, *rect.plane_x_0);
    bool c1 = checkContactCirclePlane(particle, index, *rect.plane_x_1);
    bool c2 = checkContactCirclePlane(particle, index, *rect.plane_y_0);
    bool c3 = checkContactCirclePlane(particle, index, *rect.plane_y_1);
    return (c0 || c1 || c2 || c3);
    
}

bool Collider::checkContactCharacterPlane(const Character& character, int index, const Plane& plane) {
    // Get character bounding box
    const Vec2 char_pos(character.get_x(), character.get_y());
    const float w = character.get_w();
    const float h = character.get_h();
    
    // Four corners of the character's bounding box
    Vec2 corners[4] = {
        char_pos,                    // Top-left
        Vec2(char_pos.x + w, char_pos.y),     // Top-right
        Vec2(char_pos.x, char_pos.y + h),     // Bottom-left
        Vec2(char_pos.x + w, char_pos.y + h)  // Bottom-right
    };
    
    // Find the corner with maximum penetration
    float max_penetration = -1000.0f;
    int deepest_corner = -1;
    
    for (int i = 0; i < 4; ++i) {
        Vec2 toCorner = corners[i] - plane.get_pos();
        float distance = toCorner.dot(plane.get_norm());
        
        // Only consider corners on the correct side of the plane
        if (distance < 0) continue;
        
        float penetration = -distance;  // Negative distance means penetration
        
        // Check if corner is within the plane segment bounds
        Vec2 tangent(-plane.get_norm().y, plane.get_norm().x);
        float s = toCorner.dot(tangent);
        
        if (s >= 0 && s <= plane.get_w()) {
            // Corner is within plane segment
            if (penetration > max_penetration) {
                max_penetration = penetration;
                deepest_corner = i;
            }
        }
    }
    
    // No collision if no corner penetrates
    if (deepest_corner == -1 || max_penetration <= 0) return false;
    
    // Create constraint for the deepest penetrating corner
    StaticConstraint constraint;
    constraint.particleIndex = index;
    constraint.normal = plane.get_norm();
    constraint.penetration = max_penetration;
    constraint.contactPoint = corners[deepest_corner];
    c_static_constraint.push_back(constraint);
    return true;
}
bool Collider::checkContactCharacterRectangle(const MovableRectangle& c, const Rectangle& rect) {
    return false;
}

void Collider::resolve_constraints(const std::vector<MovableCircle*>& particles, const std::vector<Character*>& characters) {
    resolve_constraints_particles(particles);
    resolve_constraints_characters(characters);
}

void Collider::resolve_constraints_characters(const std::vector<Character*>& characters){
    // TODO: Implement character collision constraints
    // if(checkContactCharacterRectangle(c1)){
    //     enforce_static_ground_constraints(c1_constraint, c1);
    // }
}


void Collider::resolve_constraints_particles(const std::vector<MovableCircle*>& particles) {
    int solver_iterations = 3;
    for (int iter = 0; iter < solver_iterations; ++iter) {
        // Resolve static constraints
        for (const auto& constraint : static_constraints) {
            enforce_static_ground_constraints(constraint, *particles[constraint.particleIndex]);
        }
        
        // // Resolve dynamic constraints
        // for (const auto& constraint : dynamicConstraints) {
        //     enforceDynamicConstraint(constraint, 
        //         particles[constraint.particleIndex1],
        //         particles[constraint.particleIndex2]);
        // }
    }
}

void Collider::enforce_static_ground_constraints(const StaticConstraint& constraint, MovableAsset& movable_asset) {
        movable_asset.update_expected_pos_collision(constraint.normal * constraint.penetration);
    }