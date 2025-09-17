#ifndef RETURNS_HELPER_H
#define RETURNS_HELPER_H

// Dummy class to compile the main code for a test
class returns_helper
{
public:
    virtual ~returns_helper() = default;

    virtual bool do_notify() { 
        return true;
    }
};

#endif // RETURNS_HELPER_H
