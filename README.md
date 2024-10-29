# Grav Project Documentation

## Project Overview
The Grav project is built within Unreal Engine 5, primarily in C++ with a Blueprint-based Quest System. The Gameplay Ability System (GAS) manages key abilities like **Smoke Grenade** and **Dash**, while a **Gravity Gun** mechanic replaces the default projectile firing. UI widgets provide visual feedback for ability cooldowns and quest progression.

---

## Tasks

### 1. Gravity Gun
- **Description**: Replaces the default projectile mechanic with a gravity gun inspired by Half-Life 2.
- **Functionality**:
  - **Pick Up Objects**: Allows the player to pick up nearby physics-enabled objects.
  - **Launch Objects**: Fires the picked-up object with adjustable force.
  - **Variables Exposed**: Configured in `BP_FirstPersonCharacter`.

### 2. Ability System
#### Smoke Grenade
- **Description**: Launches a smoke grenade to create a visual obscuration effect. 
- **Cooldown**: 3 seconds.
- **Duration**: Lasts for 3 seconds.
- **Damage**: None (visual effect only).

#### Dash
- **Description**: Provides a quick burst of movement in the player's input direction.
- **Cooldown**: 2 seconds.
- **Duration**: Instantaneous.

### 3. Quest System (Blueprint-based)
- **Functionality**: Manages the lifecycle of quests, including objectives that must be completed.
- **Objectives**: Each quest contains multiple objectives, including tasks like arriving at locations, collecting items, and interacting with actors.
- **Modularity**: Designed for easy modification, allowing new quests to be added and updated without major changes to the system.

### 4. UI Widgets
- **Ability UI**: Displays current abilities and their cooldown timers, with visual icons indicating when abilities are ready to use.
- **Quest UI**: 
  - An on-screen HUD displays progress for the active quest objectives.
  - The Quest Log shows all active and completed quests with their statuses.

### 5. Destructible Actor
- A destructible Actor was created to test the Damage system. However, abilities to damage the actor haven’t been set up yet due to time constraints.

---

## Highlights and Improvements

### Highlights
- **Performance**: Ability System and Gravity Gun functionalities were implemented in C++ for optimized performance and extendibility.
- **Helper Functions**: Created `AsyncTaskAttributeChanged` and `AsyncTaskCooldownChanged` to facilitate communication with UI elements.
- **Quest System**: The Blueprint-based Quest System enables easy adjustments and rapid iteration of quest flows.

### Improvements
- Further testing could enhance the interaction between abilities and objectives in quests.
- Optimizing quest data storage could support larger quest chains and save data portability.
- The Quest System can be migrated to C++ for enhanced performance.
- UI widgets could also be implemented in C++ for further optimization.

---
