#include "core/AssetManager.hpp"
#include "core/Logger.hpp"

/**
 * @file AssetManager.cpp
 * @brief Implementation of AssetManager.
 */

AssetManager::~AssetManager() { UnloadAll(); }

void AssetManager::LoadTexture(const std::string &name, const std::string &path) {
  if (textures.find(name) != textures.end()) {
    Logger::Warn("Texture already loaded: {}", name);
    return;
  }

  Texture2D tex = ::LoadTexture(path.c_str());
  if (tex.id == 0) {
    Logger::Error("Failed to load texture: {}", path);
    return;
  }

  textures[name] = tex;
  Logger::Info("Loaded texture: {}", name);
}

Texture2D AssetManager::GetTexture(const std::string &name) {
  if (textures.find(name) == textures.end()) {
    Logger::Warn("Texture not found: {}", name);
    return {0, 0, 0, 0, 0};
  }
  return textures[name];
}

void AssetManager::UnloadTexture(const std::string &name) {
  if (textures.find(name) != textures.end()) {
    ::UnloadTexture(textures[name]);
    textures.erase(name);
    Logger::Info("Unloaded texture: {}", name);
  }
}

void AssetManager::LoadSound(const std::string &name, const std::string &path) {
  if (sounds.find(name) != sounds.end()) {
    Logger::Warn("Sound already loaded: {}", name);
    return;
  }

  Sound snd = ::LoadSound(path.c_str());
  if (snd.frameCount == 0) {
    Logger::Error("Failed to load sound: {}", path);
    return;
  }

  sounds[name] = snd;
  Logger::Info("Loaded sound: {}", name);
}

Sound AssetManager::GetSound(const std::string &name) {
  if (sounds.find(name) == sounds.end()) {
    Logger::Warn("Sound not found: {}", name);
    return {0, 0, 0, 0};
  }
  return sounds[name];
}

void AssetManager::UnloadSound(const std::string &name) {
  if (sounds.find(name) != sounds.end()) {
    ::UnloadSound(sounds[name]);
    sounds.erase(name);
    Logger::Info("Unloaded sound: {}", name);
  }
}

void AssetManager::LoadMusic(const std::string &name, const std::string &path) {
  if (musicStreams.find(name) != musicStreams.end()) {
    Logger::Warn("Music already loaded: {}", name);
    return;
  }

  Music mus = ::LoadMusicStream(path.c_str());
  if (mus.stream.buffer == nullptr) {
    Logger::Error("Failed to load music: {}", path);
    return;
  }

  musicStreams[name] = mus;
  Logger::Info("Loaded music: {}", name);
}

Music AssetManager::GetMusic(const std::string &name) {
  if (musicStreams.find(name) == musicStreams.end()) {
    Logger::Warn("Music not found: {}", name);
    return {0, 0, 0, 0, 0};
  }
  return musicStreams[name];
}

void AssetManager::UnloadMusic(const std::string &name) {
  if (musicStreams.find(name) != musicStreams.end()) {
    ::UnloadMusicStream(musicStreams[name]);
    musicStreams.erase(name);
    Logger::Info("Unloaded music: {}", name);
  }
}

void AssetManager::LoadAllAssets() {
  // --- Textures ---
  LoadTexture("menu_bg", "assets/menu_background.png");
  LoadTexture("config_bg", "assets/config_background.png");
  LoadTexture("sound_on", "assets/sound_on.png");
  LoadTexture("sound_off", "assets/volume-mute.png");

  // Grass Background
  LoadTexture("grass1", "assets/grass1.png");
  LoadTexture("grass2", "assets/grass2.png");
  LoadTexture("grass3", "assets/grass3.png");
  LoadTexture("grass4", "assets/grass4.png");

  // Roads and Entrances
  LoadTexture("road", "assets/road.png");
  LoadTexture("entrance_up", "assets/entrance_up.png");
  LoadTexture("entrance_down", "assets/entrance_down.png");
  LoadTexture("entrance_double", "assets/entrance_double.png");

  // Parking Facilities
  LoadTexture("parking_small_up", "assets/parking_small_up.png");
  LoadTexture("parking_small_down", "assets/parking_small_down.png");
  LoadTexture("parking_large_up", "assets/parking_large_up.png");
  LoadTexture("parking_large_down", "assets/parking_large_down.png");

  // Charging Stations
  LoadTexture("charging_small_up", "assets/charging_small_up.png");
  LoadTexture("charging_small_down", "assets/charging_small_down.png");
  LoadTexture("charging_large_up", "assets/charging_large_up.png");
  LoadTexture("charging_large_down", "assets/charging_large_down.png");

  // Cars
  LoadTexture("car11", "assets/car11.png");
  LoadTexture("car12", "assets/car12.png");
  LoadTexture("car13", "assets/car13.png");
  LoadTexture("car21", "assets/car21.png");
  LoadTexture("car22", "assets/car22.png");
  LoadTexture("car23", "assets/car23.png");

  // --- Sounds ---
  LoadSound("click", "assets/click_sound.mp3");

  // --- Music ---
  LoadMusic("bg_music", "assets/background_music.mp3");

  Logger::Info("All assets loaded and cached.");
}

void AssetManager::UnloadAll() {
  for (auto &pair : textures) {
    ::UnloadTexture(pair.second);
  }
  textures.clear();

  for (auto &pair : sounds) {
    ::UnloadSound(pair.second);
  }
  sounds.clear();

  for (auto &pair : musicStreams) {
    ::UnloadMusicStream(pair.second);
  }
  musicStreams.clear();

  Logger::Info("Unloaded all managed assets.");
}
