#ifndef NES_EMU_FILTER_H
#define NES_EMU_FILTER_H
#include <initializer_list>
#include <vector>

// First order filters are defined by the following parameters.
// y[n] = B0*x[n] + B1*x[n-1] - A1*y[n-1]
struct FirstOrderFilter {
    float B0{};
    float B1{};
    float A1{};
    float prevX{};
    float prevY{};

    float Step(float x);
};

// sampleRate: samples per second
// cutoffFreq: oscillations per second
FirstOrderFilter LowPassFilter(float sampleRate, float cutoffFreq);

FirstOrderFilter HighPassFilter(float sampleRate, float cutoffFreq);

// FilterChain runs a sample through each filter in order. An empty chain passes
// samples through unchanged, like Go's nil FilterChain.
class FilterChain {
public:
    std::vector<FirstOrderFilter> filters;

    FilterChain() = default;

    FilterChain(std::initializer_list<FirstOrderFilter> filters);

    float Step(float x);
};

#endif //NES_EMU_FILTER_H
