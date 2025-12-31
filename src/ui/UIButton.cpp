#include "ui/UIButton.hpp"
#include "core/AudioManager.hpp"
#include "events/InputEvents.hpp"
#include "raylib.h"

UIButton::UIButton(Vector2 pos, Vector2 sz, const std::string &buttonText, std::shared_ptr<EventBus> bus)
    : UIElement(pos, sz, bus), text(buttonText) {

  // Subscribe to Mouse Move
  tokens.push_back(eventBus->subscribe<MouseMovedEvent>([this](const MouseMovedEvent &e) {
    if (!visible)
      return;
    Rectangle rect = {position.x, position.y, size.x, size.y};
    isHovered = CheckCollisionPointRec(e.position, rect);
  }));

  // Subscribe to Mouse Click
  tokens.push_back(eventBus->subscribe<MouseClickEvent>([this](const MouseClickEvent &e) {
    if (!visible)
      return;

    if (isHovered && e.button == MOUSE_BUTTON_LEFT) {
      if (e.down) {
        isPressed = true;
      } else if (isPressed) {
        // Trigger click sound and callback
        AudioManager::Get().PlaySound("click", 1.0f);

        if (onClick)
          onClick();
        isPressed = false;
      }
    } else if (!e.down) {
      isPressed = false;
    }
  }));
}

void UIButton::draw() {
  if (!visible)
    return;

  // Neon Color Palette
  const Color NEON_BASE = {30, 30, 70, 180};
  const Color NEON_HOVER = {140, 0, 255, 230};
  const Color NEON_PRESS = {0, 250, 255, 255};
  const Color NEON_BORDER = {0, 255, 255, 255};
  const Color NORMAL_BORDER = {100, 100, 200, 255};

  // Determine current color state
  Color bodyColor = isPressed ? NEON_PRESS : (isHovered ? NEON_HOVER : NEON_BASE);
  Color borderColor = isHovered ? NEON_BORDER : NORMAL_BORDER;
  float borderThickness = isHovered ? 3.0f : 1.5f;

  // 1. Draw Body
  DrawRectangleV(position, size, bodyColor);

  // 2. Draw Border
  Rectangle btnRect = {position.x, position.y, size.x, size.y};
  DrawRectangleLinesEx(btnRect, borderThickness, borderColor);

  // 3. Draw Text
  const int FONT_SIZE = 22;
  int textWidth = MeasureText(this->text.c_str(), FONT_SIZE);
  DrawText(this->text.c_str(), (int)(position.x + (size.x - textWidth) / 2),
           (int)(position.y + (size.y - FONT_SIZE) / 2), FONT_SIZE, WHITE);
}

void UIButton::setText(const std::string &t) { this->text = t; }
void UIButton::setOnClick(std::function<void()> cb) { onClick = cb; }
void UIButton::update(double) {}
