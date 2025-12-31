#include <gtest/gtest.h>
#include "raylib.h"
#include "entities/Car.hpp"
#include "entities/map/World.hpp"
#include "entities/map/Waypoint.hpp"
#include "entities/map/Modules.hpp"   // Necessary to work with Modules
#include "systems/PathPlanner.hpp"    // Necessary to work with PathPlanner
#include "systems/TrafficSystem.hpp"
#include "events/GameEvents.hpp"
#include "core/EventBus.hpp"
#include "core/EntityManager.hpp"

// --- Test Suite 1: Car Logic ---

// 1. Battery & Charging Test
TEST(CarTest, BatteryLogic) {
    Car myCar({0, 0}, nullptr, {0, 0}, Car::CarType::ELECTRIC);

    float initialBattery = myCar.getBatteryLevel();
    EXPECT_GE(initialBattery, 10.0f);
    EXPECT_LE(initialBattery, 90.0f);

    // Decrease charge
    myCar.charge(-20.0f);
    EXPECT_FLOAT_EQ(myCar.getBatteryLevel(), initialBattery - 20.0f);
}

// 2. Priority Logic
TEST(CarTest, PrioritySystem) {
    Car myCar({0, 0}, nullptr, {0, 0}, Car::CarType::COMBUSTION);

    // Default priority
    EXPECT_EQ(myCar.getPriority(), Car::Priority::PRIORITY_DISTANCE);

    // Change priority
    myCar.setPriority(Car::Priority::PRIORITY_PRICE);
    EXPECT_EQ(myCar.getPriority(), Car::Priority::PRIORITY_PRICE);
}

// 3. Waypoint Queue Logic
TEST(CarTest, WaypointQueue) {
    Car myCar({0, 0}, nullptr, {0, 0}, Car::CarType::ELECTRIC);

    // Initially arrived (since it has no path)
    EXPECT_TRUE(myCar.hasArrived());

    // Add Waypoint
    Waypoint wp({100.0f, 100.0f}); 
    myCar.addWaypoint(wp);

    // Now it shouldn't have arrived
    EXPECT_FALSE(myCar.hasArrived());

    // Clear the path
    myCar.clearWaypoints();
    EXPECT_TRUE(myCar.hasArrived());
}

// 4. Movement Logic (Velocity Update)
TEST(CarTest, MovementLogic) {
    // Create a stationary car
    Car myCar({0, 0}, nullptr, {0, 0}, Car::CarType::COMBUSTION);
    
    // Initial velocity 0
    EXPECT_FLOAT_EQ(myCar.getVelocity().x, 0.0f);
    EXPECT_FLOAT_EQ(myCar.getVelocity().y, 0.0f);

    // Give it velocity (Simulate Acceleration)
    Vector2 newVel = {50.0f, 0.0f};
    myCar.setVelocity(newVel);

    // Ensure velocity has changed
    EXPECT_FLOAT_EQ(myCar.getVelocity().x, 50.0f);

    // Note: we didn't call update() here because update needs World* (nullptr might crash it)
    // But we tested that setVelocity works.
}

// --- Test Suite 2: PathPlanner Logic ---

// 5. Generate Path Sanity (Does it generate a path?)
TEST(PathPlannerTest, GeneratePathSanity) {
    // 1. Setup Car
    Car myCar({0, 0}, nullptr, {0, 0}, Car::CarType::COMBUSTION);

    // 2. Setup Module (Large Parking Example)
    LargeParking parkingFac(true); // Top parking
    parkingFac.worldPosition = {200, 200}; // Put it somewhere in the world

    // 3. Setup Spot
    // According to the Spot struct in your code: localPosition, orientation, id, state, price
    Spot targetSpot = { {10.0f, 10.0f}, 0.0f, 1, SpotState::FREE, 5.0f };

    // 4. Generate Path
    std::vector<Waypoint> path = PathPlanner::GeneratePath(&myCar, &parkingFac, targetSpot);

    // 5. Assertions
    EXPECT_FALSE(path.empty()) << "PathPlanner failed to generate a path!";
    EXPECT_GE(path.size(), 2) << "Path should have at least Start and End points";
}

// 6. Path Accuracy (Does the path reach the target?)
TEST(PathPlannerTest, PathEndsAtTarget) {
    Car myCar({0, 0}, nullptr, {0, 0}, Car::CarType::ELECTRIC);
    
    // Use SmallChargingStation as an example
    SmallChargingStation charger(false); // Bottom
    charger.worldPosition = {500, 500};

    Spot targetSpot = { {20.0f, 20.0f}, 1.57f, 5, SpotState::FREE, 10.0f };

    std::vector<Waypoint> path = PathPlanner::GeneratePath(&myCar, &charger, targetSpot);
    
    ASSERT_FALSE(path.empty());

    // The last point in the path
    Waypoint finalPoint = path.back();

    // Expected position (Global Position = Module Pos + Spot Local Pos)
    float expectedX = charger.worldPosition.x + targetSpot.localPosition.x;
    float expectedY = charger.worldPosition.y + targetSpot.localPosition.y;

    // Ensure we arrived (within 5 units error margin)
    EXPECT_NEAR(finalPoint.position.x, expectedX, 5.0f);
    EXPECT_NEAR(finalPoint.position.y, expectedY, 5.0f);
}

// --- Test Suite 3: World Logic ---

TEST(WorldTest, GridToggle) {
    World myWorld(1000.0f, 1000.0f);

    bool initialState = myWorld.isGridEnabled();
    myWorld.toggleGrid();
    EXPECT_NE(myWorld.isGridEnabled(), initialState);
}

// --- Test Suite 4: TrafficSystem Logic ---

TEST(TrafficSystemTest, PassThroughFallback) {
    auto bus = std::make_shared<EventBus>();
    EntityManager em(bus);
    
    // Add only a road, no facilities
    auto road = std::make_unique<NormalRoad>();
    road->worldPosition = {0, 0};
    em.addModule(std::move(road));
    
    TrafficSystem ts(bus, em);
    
    // Create a car
    auto car = std::make_unique<Car>(Vector2{0, 0}, nullptr, Vector2{15, 0}, Car::CarType::COMBUSTION);
    Car* carPtr = car.get();
    em.addCar(std::move(car));
    
    // Manually trigger the spawn event (which TrafficSystem listens to)
    bus->publish(CarSpawnedEvent{carPtr});
    
    // Wait a bit or explicitly check state
    // TrafficSystem should have assigned a path immediately
    EXPECT_EQ(carPtr->getState(), Car::CarState::EXITING);
    EXPECT_FALSE(carPtr->hasArrived());
    
    // Verify it has waypoints
    // carPtr->draw(true) or similar? No, just check waypoints
    // Waypoints are private in Car, but we can check hasArrived()
}

#include "core/AssetManager.hpp"

// --- Main ---
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    // Initialize Raylib for tests (needed for LoadTexture)
    SetTraceLogLevel(LOG_NONE);
    if (!IsWindowReady()) {
        InitWindow(1, 1, "test");
    }

    // Load all assets required for entities like World and Modules
    AssetManager::Get().LoadAllAssets();

    int result = RUN_ALL_TESTS();

    // Clean up assets before window is closed to ensure GL context is valid
    AssetManager::Get().UnloadAll();

    if (IsWindowReady()) {
        CloseWindow();
    }
    return result;
}
