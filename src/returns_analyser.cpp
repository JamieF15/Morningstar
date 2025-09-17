#include "returns_analyser.h"
#include "returns_helper.h"
#include "returns_processor.h"
#include <sstream>
#include <iostream>  // for logging

// Constructor
returns_analyser::returns_analyser(std::vector<double> returns, returns_helper* helper)
    : m_returns(std::move(returns)), m_helper(helper)
{}

// Destructor
returns_analyser::~returns_analyser()
{
    bool ok = true;
    try { 
        ok = m_helper ? m_helper->do_notify() : true; 
    }

    catch (...) { 
        ok = false;
    }

    if (!ok) {
        std::cerr << "returns_analyser: helper notify failed\n";
    }
    delete m_helper;
}

// Return all returns as a comma-separated string
// this functiion is improved as it no longer has an unwanted extra " ,"
std::string returns_analyser::get_returns() const
{
    std::ostringstream oss;
    for (size_t i = 0; i < m_returns.size(); ++i) {
        if (i) {
            oss << ", ";
        }
        oss << m_returns[i];
    }
    return oss.str();
}

// Remove all occurrences of a value
// This function has improvements related to removed the iterator invalidation by making it more idiomatic 
void returns_analyser::erase_values(double value)
{
    m_returns.erase(std::remove(m_returns.begin(), m_returns.end(), value),
        m_returns.end());
}

// Get maximum return
// This function now accounts for if m_returns only conatins negative numbers
// Also using size_t instead of int in order to container sizes and avoid signed and unsighed int issues
double returns_analyser::get_max() const
{
    if (m_returns.empty()) {
        return 0.0;
    }

    double result = m_returns[0];

    for (size_t i = 1; i < m_returns.size(); ++i) {

        if (m_returns[i] > result) {
            result = m_returns[i];
        }
    }
    return result;
}

// Get mean of returns
// Solved the issue of devide-by-0 
double returns_analyser::get_mean() const
{
    if (m_returns.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (double v : m_returns) {
        sum += v;
    }
    return sum / static_cast<double>(m_returns.size());
}

// Check if all returns are above threshold
// We now exit early here if we find a number that is not optimal 
bool returns_analyser::returns_optimal() const
{
    for (double v : m_returns) {
        if (v < MIN_OPTIMAL_RETURN) {
            return false;
        }
    }
    return true;
}

// Process returns with external processor
bool returns_analyser::process_returns(const std::shared_ptr<returns_processor>& processor) const
{
    if (!processor) {
        return false;
    }
    processor->process(m_returns);
    return true;
}
