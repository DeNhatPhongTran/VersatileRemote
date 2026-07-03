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

    void setItemClickCallback(void (*handler)(void* context, int16_t itemIndex), void* context);

    const DeviceEntry* getDeviceAt(int16_t index) const;

protected:
    DeviceEntry cachedDevices[Model::MAX_DEVICES];
    int cachedDeviceCount;

    void (*itemClickHandler)(void* context, int16_t itemIndex);
    void* itemClickContext;

    static void staticItemClickHandler(void* context, int16_t itemIndex);
};

#endif // DEVICEMENU_HPP
