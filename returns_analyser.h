#ifndef RETURNS_analyser_H
#define RETURNS_analyser_H

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <algorithm>

class returns_helper;
class returns_processor;

class returns_analyser
{
public:
    // Constructor
    returns_analyser(std::vector<double> returns, returns_helper* helper);

    // Destructor
    ~returns_analyser();

    // Methods
    std::string get_returns() const;
    void erase_values(double value);
    double get_max() const;
    double get_mean() const;
    bool returns_optimal() const;
    bool process_returns(const std::shared_ptr<returns_processor>& processor) const;

private:
    std::vector<double> m_returns;
    returns_helper* m_helper;

    // This is now a static constexpr so it is compile-time constant
    // it is also defined here to make the code more readable and maintainable
    static constexpr double MIN_OPTIMAL_RETURN = 0.15;
};

#endif // RETURNS_analyser_H
