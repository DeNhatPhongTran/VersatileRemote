#ifndef BUTTONPOWER_HPP
#define BUTTONPOWER_HPP

#include <gui_generated/containers/ButtonPowerBase.hpp>

class ButtonPower : public ButtonPowerBase
{
public:
    ButtonPower();
    virtual ~ButtonPower() {}

    virtual void initialize();
protected:
};

#endif // BUTTONPOWER_HPP
