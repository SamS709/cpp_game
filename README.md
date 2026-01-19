# Stickman Fighting Game

A 2D fighting game developed as a school project using C++ and Qt framework. Battle against your opponent with various attacks, special moves, and power-ups!

## Gameplay Overview

![Gameplay](resources/videos/gameplay.gif)

*Two players battle it out in this fast-paced fighting game!*

## Controls

![Controls](resources/images/readme/controls.png)

### Player 1 (Left Side)
- **Z**: Jump / Double Jump
- **Q**: Move Left
- **S**: Move Down / Crouch
- **D**: Move Right
- **A**: Sword Attack
- **E**: Projectile Attack

### Player 2 (Right Side)
- **↑**: Jump / Double Jump
- **←**: Move Left
- **↓**: Move Down / Crouch
- **→**: Move Right
- **0**: Sword Attack
- **1**: Projectile Attack

## Bonuses

Collect power-ups that spawn randomly on the battlefield to gain advantages!

### Bomb Bonus 
![Bomb Bonus](resources/images/readme/bonus_bomb.png)

Spawns multiple bombs that explode after a short delay, dealing damage to your opponent.

### Speed Boost 
![Speed Bonus](resources/images/readme/bonus_speed.png)

Temporarily increases your movement speed for a limited time.

### Health Pack 
![Life Bonus](resources/images/readme/bonus_life.png)

Restores health points instantly.

## Health Bar

![Lifebar](resources/images/readme/lifebar.png)

The health bar dynamically changes color based on remaining health:
- **Green**: High health (near 100%)
- **Yellow**: Medium health (around 50%)
- **Red**: Low health (near 0%)

## Character Actions

<<<<<<< HEAD
The animations are made using sequence of pngs, loaded as QPixmap, stores in QVector (one per action). Each frame (QPixmap) am is chooseen programatically depending on the action.

=======
>>>>>>> dev
### Jump
<img src="resources/images/readme/jump.png" alt="Jump" width="200">

Press the jump key to leap into the air. Can perform a double jump while airborne for enhanced mobility.

### Slide
<img src="resources/images/readme/slide.png" alt="Slide" width="200">

Press down while running to perform a sliding move that allows you to evade attacks and move quickly under opponents.

### Sword Attack
<img src="resources/images/readme/sword_attack.png" alt="Sword Attack" width="200">

Execute a powerful sword strike to damage your opponent. Each character has unique sword attack animations.

### Low Sword Attack (Combo)
<img src="resources/images/readme/sword_attack_low.png" alt="Sword Attack Low" width="200">

**Special Combo**: Press **Down** and **Sword Attack** keys simultaneously while crouching to perform a low sword attack. This ground-level attack can catch opponents off-guard!

## Debugging - Hitbox Visualization

![Hitbox Debug](resources/videos/bounding_boxes.gif)

For debugging purposes, you can enable hitbox visualization by setting `draw_boxes` to `true` in the Character class. This will display:
- **Red boxes**: Asset/sprite boundaries
- **Green boxes**: Character collision hitboxes
- **Blue boxes**: Sword attack ranges

## Technical Details

### Physics Engine
The game uses **Position-Based Dynamics** for realistic physics simulation, providing smooth and responsive character movements, collision detection, and projectile trajectories.

### Technologies Used
- **C++17**: Core game logic
- **Qt 6.10**: GUI framework and rendering
- **CMake**: Build system
- **Ninja**: Build tool

## Building the Project

1. Ensure you have Qt 6.10+ installed
2. Configure CMake:
   ```bash
   cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=<path_to_qt>
   ```
3. Build:
   ```bash
   cmake --build build
   ```
4. Run the executable from the build directory

## Future Improvements

This game is currently in development and far from being finished. As an AI enthusiast, my ultimate goal is to implement **Reinforcement Learning (RL)** to train a "super" agent that can master the game mechanics and defeat any opponent. 

---

**Note**: This project was developed as part of a school assignment to demonstrate C++ programming skills, game development concepts, and GUI application development using Qt framework.
