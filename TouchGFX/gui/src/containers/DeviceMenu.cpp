#include <gui/containers/DeviceMenu.hpp>
#include <string.h>

DeviceMenu::DeviceMenu() :
    cachedDeviceCount(0)
{
    memset(cachedDevices, 0, sizeof(cachedDevices));
}

void DeviceMenu::initialize()
{
    DeviceMenuBase::initialize();
}

void DeviceMenu::scrollList1UpdateItem(ButtonDevice& item, int16_t itemIndex)
{
    if (itemIndex >= 0 && itemIndex < cachedDeviceCount)
    {
        item.setText(cachedDevices[itemIndex].name);
    }
}

void DeviceMenu::setDevices(const DeviceEntry* devs, int count)
{
    cachedDeviceCount = (count < Model::MAX_DEVICES) ? count : Model::MAX_DEVICES;
    for (int i = 0; i < cachedDeviceCount; i++)
    {
        strncpy(cachedDevices[i].name, devs[i].name, sizeof(cachedDevices[i].name) - 1);
        cachedDevices[i].name[sizeof(cachedDevices[i].name) - 1] = '\0';
        strncpy(cachedDevices[i].signalId, devs[i].signalId, sizeof(cachedDevices[i].signalId) - 1);
        cachedDevices[i].signalId[sizeof(cachedDevices[i].signalId) - 1] = '\0';
        cachedDevices[i].type = devs[i].type;
    }

    scrollList1.setNumberOfItems(cachedDeviceCount);
    scrollList1.invalidate();
}
