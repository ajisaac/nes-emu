#include "Filter.h"

#include <numbers>

float FirstOrderFilter::Step(const float x) {
    const float y = B0 * x + B1 * prevX - A1 * prevY;
    prevY = y;
    prevX = x;
    return y;
}

FirstOrderFilter LowPassFilter(const float sampleRate, const float cutoffFreq) {
    const float c = sampleRate / std::numbers::pi_v<float> / cutoffFreq;
    const float a0i = 1 / (1 + c);
    return FirstOrderFilter{
        .B0 = a0i,
        .B1 = a0i,
        .A1 = (1 - c) * a0i,
    };
}

FirstOrderFilter HighPassFilter(const float sampleRate, const float cutoffFreq) {
    const float c = sampleRate / std::numbers::pi_v<float> / cutoffFreq;
    const float a0i = 1 / (1 + c);
    return FirstOrderFilter{
        .B0 = c * a0i,
        .B1 = -c * a0i,
        .A1 = (1 - c) * a0i,
    };
}

FilterChain::FilterChain(const std::initializer_list<FirstOrderFilter> filters) : filters{filters} {
}

float FilterChain::Step(float x) {
    for (auto &filter : filters) {
        x = filter.Step(x);
    }
    return x;
}
