#include "returns_analyser.h"
#include "returns_helper.h"
#include "returns_processor.h"
#include <sstream>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <memory>

// Constructor
returns_analyser::returns_analyser(std::vector<double> returns, std::unique_ptr<returns_helper> helper)
    : m_returns(std::move(returns)), m_helper(std::move(helper))
{}

// Return all returns as a comma-separated string
// This function is improved as it no longer has an unwanted extra " ," 
std::string returns_analyser::get_returns() const
{
    std::ostringstream oss;
    
    for (auto it = m_returns.cbegin(); it != m_returns.cend(); ++it) {
        if (it != m_returns.cbegin()) {
            oss << ", ";
        }
        oss << *it;
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
// This function now accounts for if m_returns only contains negative numbers
// We are now using std::max_element for a more idiomatic may to find the largest number in hte collection 
double returns_analyser::get_max() const noexcept
{
    if (m_returns.empty()) {
        return 0.0;
    }

    auto it = std::max_element(m_returns.cbegin(), m_returns.cend());
    return (it != m_returns.cend()) ? *it : 0.0;
}

// Get mean of returns
// Solved the issue of divide-by-0 
// We now return early if we do not have any returns
// At the cost of casting the size to a double, we now divide the sum by the size of m_returns as a double for more precision 
double returns_analyser::get_mean() const noexcept
{
    if (m_returns.empty()) {
        return 0.0;
    }

    double sum = std::accumulate(m_returns.cbegin(), m_returns.cend(), 0.0);
    return sum / static_cast<double>(m_returns.size());
}

// Check if all returns are above threshold
// We now exit early here if we find a number that is not optimal 
bool returns_analyser::returns_optimal() const noexcept
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
