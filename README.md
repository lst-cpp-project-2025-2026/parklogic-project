# ParkLogic: Procedural Traffic Simulation

![C++](https://img.shields.io/badge/Language-C%2B%2B20-00599C?style=flat-square&logo=c%2B%2B)
![Raylib](https://img.shields.io/badge/Graphics-Raylib_5.5-white?style=flat-square)
![CMake](https://img.shields.io/badge/Build-CMake-064F8C?style=flat-square&logo=cmake)
![GoogleTest](https://img.shields.io/badge/Testing-GoogleTest-brightgreen?style=flat-square&logo=google)

> ParkLogic is a deterministic simulation environment developed in C++20 for modeling vehicle-facility interactions. It features a custom engine built on Raylib, procedural world generation, event-driven architecture, and autonomous agent AI with heuristic-based destination arbitration.

---

## Gallery

![menu_background](https://github.com/user-attachments/assets/38612a9f-2d99-4fd1-ba2b-eec6af12075a)

### Demo Video

https://github.com/user-attachments/assets/a588c9f4-afe3-497d-9f15-b5d347adf936

### Screenshots

![alt text](https://github.com/user-attachments/assets/74631bc8-e68b-48b5-9797-99a4d74ff578)

![alt text](https://github.com/user-attachments/assets/8c7cbd96-d5e7-498f-9f6c-e9961bb5247b)

![alt text](https://github.com/user-attachments/assets/83deccfa-8e29-4c25-9b76-51fee183ebbd)

![alt text](https://github.com/user-attachments/assets/72532cfa-b618-48c1-abf6-dd56a86e06f4)

![alt text](https://github.com/user-attachments/assets/cf42f830-0117-4a8c-956a-e2e14f16b26d)

---

## Core Features

- **Procedural World Generation**: Algorithmic assembly of road networks and facilities based on user-defined configurations.
- **Autonomous Agent AI**: Vehicles utilize steering behaviors (Seek, Arrival) and spatial corridor detection for collision avoidance and path following.
- **Economic Heuristics**: Agents independently select destinations based on priority strategies such as proximity (`PRIORITY_DISTANCE`) or price (`PRIORITY_PRICE`).
- **Dynamic Simulation Controls**: Real-time adjustment of simulation speed (1x-5x), camera manipulation (WASD Pan / Mouse Zoom), and manual car spawning.
- **Comprehensive Monitoring**: Context-aware dashboard for real-time analysis of agent states, facility occupancy, and global performance metrics.

---

## Technical Architecture

The project is built on a decoupled, observer-based architecture using a centralized event bus:

- **Core Engine**: Manages the "Fix Your Timestep" algorithm (60Hz physics), high-DPI windowing, and the central execution loop.
- **EventBus**: A type-safe Pub/Sub system (RTTI-based) that facilitates communication between simulation systems and the UI.
- **Systems Architecture**:
  - **TrafficSystem**: Manages macroscopic agent lifecycles, flow rates, and spawning logic.
  - **PathPlanner**: Generates multi-phase geometric trajectories including merging, approach, and parking maneuvers.
  - **TrackingSystem**: Automated viewport management for monitoring specific agents.
  - **AudioManager**: Dedicated service for managing background music and positional sound effects.

---

## Contributors

The ParkLogic project was developed by:

- Bilal Houari
- Safouane Bousakhra
- Ayman Amtot
- Saad Marzouq
- Adil Elbahlouli

---

## Technology Stack

- **Language**: C++20
- **Graphics**: Raylib 5.5
- **Build System**: CMake 3.20+
- **Testing**: GoogleTest (GTest)

---

## Getting Started

### Prerequisites

- A C++20 compatible compiler (GCC 11+, Clang 13+, or MSVC 2019+)
- CMake 3.20 or higher
- Standard OpenGL 3.3+ development headers

### Build Instructions (Linux)

The project uses CMake's `FetchContent` to manage all dependencies automatically.

```bash
# 1. Configuration
cmake -B build

# 2. Build
cmake --build build -j$(nproc)

# 3. Run
./build/parklogic
```

### Running Tests

Unit tests for core engine components and simulation logic can be executed via:

```bash
# Build tests
cmake --build build --target unit_tests

# Run tests
./build/tests/unit_tests
```

---

## Documentation

For a comprehensive technical analysis of the architecture, design patterns, and implementation details, refer to the [Technical Report](report/report.pdf).

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
