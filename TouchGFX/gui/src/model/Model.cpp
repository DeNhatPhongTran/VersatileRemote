#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <string.h>

Model::Model() : modelListener(0), deviceCount(0), selectedDeviceType(DEVICE_TV)
{

}

void Model::tick()
{

}

int Model::getDevicesByType(DeviceType type, DeviceEntry* out, int maxOut)
{
    int count = 0;
    for (int i = 0; i < deviceCount && count < maxOut; i++)
    {
        if (devices[i].type == type)
        {
            out[count] = devices[i];
            count++;
        }
    }
    return count;
}

bool Model::addDevice(const char* name, const char* signalId, DeviceType type)
{
    if (deviceCount >= MAX_DEVICES)
    {
        return false;
    }

    DeviceEntry& entry = devices[deviceCount];
    strncpy(entry.name, name, sizeof(entry.name) - 1);
    entry.name[sizeof(entry.name) - 1] = '\0';
    strncpy(entry.signalId, signalId, sizeof(entry.signalId) - 1);
    entry.signalId[sizeof(entry.signalId) - 1] = '\0';
    entry.type = type;
    deviceCount++;
    return true;
}

void Model::setSelectedDeviceType(DeviceType t)
{
    selectedDeviceType = t;
}

DeviceType Model::getSelectedDeviceType()
{
    return selectedDeviceType;
}
