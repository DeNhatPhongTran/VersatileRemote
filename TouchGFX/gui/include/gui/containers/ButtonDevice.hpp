#ifndef BUTTONDEVICE_HPP
#define BUTTONDEVICE_HPP

#include <gui_generated/containers/ButtonDeviceBase.hpp>

class ButtonDevice : public ButtonDeviceBase
{
public:
    ButtonDevice();
    virtual ~ButtonDevice() {}

    virtual void initialize();
protected:
};

#endif // BUTTONDEVICE_HPP
