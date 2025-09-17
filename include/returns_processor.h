#ifndef RETURNS_PROCESSOR_H
#define RETURNS_PROCESSOR_H

#include <vector>

// Dummy class to compile the main code for a test
class returns_processor
{
public:
    virtual ~returns_processor() = default;

    // Stub: process the returns (does nothing by default).
    virtual void process(const std::vector<double>& values) {}
};

#endif // RETURNS_PROCESSOR_H
