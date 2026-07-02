#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>

typedef enum {
    DEVICE_TV = 0,
    DEVICE_PROJECTOR = 1,
    DEVICE_AC = 2,
    DEVICE_COUNT = 3
} DeviceType;

struct DeviceEntry {
    char name[32];
    char signalId[32];
    DeviceType type;
};

class ModelListener;

class Model
{
public:
    static const int MAX_DEVICES = 20;

    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    int getDevicesByType(DeviceType type, DeviceEntry* out, int maxOut);
    bool addDevice(const char* name, const char* signalId, DeviceType type);

    void setSelectedDeviceType(DeviceType t);
    DeviceType getSelectedDeviceType();

protected:
    ModelListener* modelListener;
    DeviceEntry devices[MAX_DEVICES];
    int deviceCount;
    DeviceType selectedDeviceType;
};

#endif // MODEL_HPP
