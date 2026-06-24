#ifndef DEVICEMENU_HPP
#define DEVICEMENU_HPP

#include <gui_generated/containers/DeviceMenuBase.hpp>

class DeviceMenu : public DeviceMenuBase
{
public:
    DeviceMenu();
    virtual ~DeviceMenu() {}

    virtual void initialize();
protected:
};

#endif // DEVICEMENU_HPP
