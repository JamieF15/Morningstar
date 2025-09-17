#ifndef RETURNS_ANALYSER_H
#define RETURNS_ANALYSER_H

#include <vector>
#include <string>
#include <memory>
#include <algorithm>

class returns_helper;
class returns_processor;

class returns_analyser
{
public:
    // Constructor takes unique_ptr now to prevent the need for manually managing the memory of the helper
    returns_analyser(std::vector<double> returns, std::unique_ptr<returns_helper> helper);

    ~returns_analyser() = default;

    // Methods
    std::string get_returns() const;
    void erase_values(double value);
    double get_max() const noexcept;
    double get_mean() const noexcept;
    bool returns_optimal() const noexcept;
    bool process_returns(const std::shared_ptr<returns_processor>& processor) const;

private:
    std::vector<double> m_returns;
    std::unique_ptr<returns_helper> m_helper;  // Unique ownership

    static constexpr double MIN_OPTIMAL_RETURN = 0.15;
};

#endif // RETURNS_ANALYSER_H
