#pragma once

#include <chrono>

void setEnabled(bool state);

float BaseOptions_getSensitivity(float currentSensitivity);

float LevelRendererPlayer_getFov(float currentFov, bool applyEffects);