#include <gui/add_device_screen/Add_DeviceView.hpp>
#include <gui/add_device_screen/Add_DevicePresenter.hpp>
#include <gui/model/Model.hpp>
#include <string.h>

Add_DevicePresenter::Add_DevicePresenter(Add_DeviceView& v)
    : view(v)
{

}

void Add_DevicePresenter::activate()
{
    DeviceType type = model->getSelectedDeviceType();
    view.showDeviceType(type);
}

void Add_DevicePresenter::deactivate()
{

}

void Add_DevicePresenter::onConfirmAddDevice()
{
    DeviceType type = model->getSelectedDeviceType();

    const char* typePrefix = "Dev";
    switch (type)
    {
        case DEVICE_TV:
            typePrefix = "TV";
            break;
        case DEVICE_AC:
            typePrefix = "AC";
            break;
        case DEVICE_PROJECTOR:
            typePrefix = "PJ";
            break;
        default:
            break;
    }

    DeviceEntry allDevices[20];
    int count = model->getDevicesByType(type, allDevices, 20);

    char name[32];
    int index = count + 1;

    strncpy(name, typePrefix, sizeof(name) - 1);
    int len = strlen(name);
    if (len < (int)(sizeof(name) - 1))
    {
        name[len++] = '_';
        if (index >= 100) { name[len++] = '0' + (index / 100); index %= 100; }
        if (index >= 10 || len > (int)strlen(typePrefix) + 1) { name[len++] = '0' + (index / 10); index %= 10; }
        name[len++] = '0' + index;
        name[len] = '\0';
    }

    model->addDevice(name, "SIG_DEFAULT", type);
}
