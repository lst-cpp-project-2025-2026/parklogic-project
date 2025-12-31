#include "systems/TrackingSystem.hpp"
#include "core/Logger.hpp"
#include "events/GameEvents.hpp"
#include "events/TrackingEvents.hpp"

TrackingSystem::TrackingSystem(std::shared_ptr<EventBus> bus) : eventBus(bus) {
  // Subscribe to tracking events
  eventTokens.push_back(
      eventBus->subscribe<StartTrackingEvent>([this](const StartTrackingEvent &) { this->startTracking(); }));

  eventTokens.push_back(
      eventBus->subscribe<StopTrackingEvent>([this](const StopTrackingEvent &) { this->stopTracking(); }));

  // Monitor for target car spawn
  eventTokens.push_back(eventBus->subscribe<CarSpawnedEvent>([this](const CarSpawnedEvent &e) {
    if (this->waitingForSpawn) {
      this->targetCar = e.car;
      this->waitingForSpawn = false;
      Logger::Info("TrackingSystem: Target car found, beginning tracking.");
    }
  }));

  // Handle target car deletion (safety)
  eventTokens.push_back(eventBus->subscribe<CarDeletedEvent>([this](const CarDeletedEvent &e) {
    if (this->targetCar == e.car) {
      Logger::Info("TrackingSystem: Target car deleted. Stopping tracking.");
      this->stopTracking();
    }
  }));

  // Stop tracking on world reset
  eventTokens.push_back(eventBus->subscribe<GenerateWorldEvent>([this](const GenerateWorldEvent &) {
    if (this->isTrackingActive) {
      Logger::Info("TrackingSystem: World reset detected via GenerateWorldEvent. Stopping tracking.");
      this->stopTracking();
    }
  }));

  // Update tracking position
  eventTokens.push_back(eventBus->subscribe<GameUpdateEvent>([this](const GameUpdateEvent &e) { this->update(e.dt); }));
}

TrackingSystem::~TrackingSystem() = default;

void TrackingSystem::startTracking() {
  isTrackingActive = true;
  waitingForSpawn = true;
  targetCar = nullptr;

  // Request a new car spawn to track
  eventBus->publish(SpawnCarRequestEvent{});
  eventBus->publish(TrackingStatusEvent{true});
  Logger::Info("TrackingSystem: Started.");
}

void TrackingSystem::stopTracking() {
  isTrackingActive = false;
  targetCar = nullptr;
  waitingForSpawn = false;
  eventBus->publish(TrackingStatusEvent{false});
  Logger::Info("TrackingSystem: Stopped.");
}

void TrackingSystem::update(double) {
  if (!isTrackingActive || !targetCar) {
    if (isTrackingActive && !targetCar && !waitingForSpawn) {
        // This shouldn't happen normally, maybe the car was deleted but we missed the event
        Logger::Warn("TrackingSystem: Active but no target car. Stopping.");
        stopTracking();
    }
    return;
  }

  // Forward car position to camera
  Vector2 carPos = targetCar->getPosition();
  eventBus->publish(CameraMoveEvent{carPos});

  // End tracking if target car makes it to the exit
  if (targetCar->getState() == Car::CarState::EXITING && targetCar->hasArrived()) {
    Logger::Info("TrackingSystem: Target car exited. Stopping tracking.");
    stopTracking();
  }
}