// Code Review

// As part of our technical assessment, please perform a code review on the following code snippet.

// As a first pass, suppose a junior developer has submitted this as an initial pull request for some feedback. 
// What comments or suggestions you would make to aid them in refining the code and getting it closer to a merge approval?

// Next, rewrite the below to be more readable and maintainable.  As part of your re-write, note any other improvements
// you would consider in addition to the changes you've made.


class returns_analyzer
{
    public:
        returns_analyzer(std::vector<double> returns, returns_helper* helper)
        {
            m_helper = helper;
            m_returns = returns;
        }

        ~returns_analyzer()
        {
            if(!m_helper->do_notify())
            {
                throw std::runtime_error("error");
            }
            delete m_helper;
        }

        void get_returns(std::string out)
        {
            for(int i = 0; i < m_returns.size(); i++)
            {
                out += std::to_string(m_returns[i])
                out += ", ";
            }
        }

        void erase_values(double value)
        {
            for (auto it = m_returns.begin(); it != m_returns.end(); it++)
            {
                if (*it == value)
                {
                    m_returns.erase(it);
                }
            }
        }

        double get_max()
        {
            double result = 0.0;
            for(int i = 0; i < m_returns.size(); i++)
            {
                if(m_returns[i] > result)
                {
                    result = m_returns[i];
                }
            }
            return result;
        }

        double get_mean()
        {
            double mean = 0.0;
            for(int i = 0; i < m_returns.size(); i++)
            {
                mean += m_returns[i];
            }
            mean /= m_returns.size();
            return mean;
        }

        bool returns_optimal()
        {
            bool optimal = true;
            for(int i = 0; i < m_returns.size(); i++)
            {
                if(m_returns[i] < 0.15)
                {
                    optimal = false;
                }
            }

            return optimal;
        }

        bool process_returns(const std::shared_ptr<returns_processor>& processor)
        {
            if(!processor)
            {
                return false;
            }
            else
            {
                processor->process(m_returns);
                return true;
            }
        }

        std::vector<double> m_returns;
        returns_helper* m_helper;
}

