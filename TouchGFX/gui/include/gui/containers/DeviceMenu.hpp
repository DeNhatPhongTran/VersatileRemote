#ifndef DEVICEMENU_HPP
#define DEVICEMENU_HPP

#include <gui_generated/containers/DeviceMenuBase.hpp>
#include <gui/model/Model.hpp>

class DeviceMenu : public DeviceMenuBase
{
public:
    DeviceMenu();
    virtual ~DeviceMenu() {}

    virtual void initialize();

    virtual void scrollList1UpdateItem(ButtonDevice& item, int16_t itemIndex);

    void setDevices(const DeviceEntry* devs, int count);

protected:
    DeviceEntry cachedDevices[Model::MAX_DEVICES];
    int cachedDeviceCount;
};

#endif // DEVICEMENU_HPP
