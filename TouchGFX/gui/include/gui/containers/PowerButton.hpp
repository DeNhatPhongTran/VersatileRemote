#ifndef POWERBUTTON_HPP
#define POWERBUTTON_HPP

#include <gui_generated/containers/PowerButtonBase.hpp>

class PowerButton : public PowerButtonBase
{
public:
    PowerButton();
    virtual ~PowerButton() {}

    virtual void initialize();
protected:
};

#endif // POWERBUTTON_HPP
