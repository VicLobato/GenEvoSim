#pragma once

// Used for basic functions that can be used anywhere

float min(float a, float b) {
    return (a < b) ? a : b;
}

float max(float a, float b) {
    return (a > b) ? a : b;
}

float clamp(float value, float low, float high) {
    return min(max(value, low), high);
}