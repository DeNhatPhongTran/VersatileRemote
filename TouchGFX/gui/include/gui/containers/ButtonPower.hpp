#ifndef BUTTONPOWER_HPP
#define BUTTONPOWER_HPP

#include <gui_generated/containers/ButtonPowerBase.hpp>

class ButtonPower : public ButtonPowerBase
{
public:
    ButtonPower();
    virtual ~ButtonPower() {}

    virtual void initialize();

    void setAction(touchgfx::GenericCallback<const touchgfx::AbstractButtonContainer&>& callback)
    {
        power.setAction(callback);
    }

    bool isPressed(const touchgfx::AbstractButtonContainer& src) const
    {
        return &src == &power;
    }
protected:
};

#endif // BUTTONPOWER_HPP
