#include <gui/add_device_screen/Add_DeviceView.hpp>
#include <gui/add_device_screen/Add_DevicePresenter.hpp>
#include <gui/model/Model.hpp>
#include <string.h>
#include <cstdio>

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
    snprintf(name, sizeof(name), "device_%s_number_.%d", typePrefix, count + 1);

    model->addDevice(name, "SIG_DEFAULT", type);

    view.navigateBack(type);
}

DeviceType Add_DevicePresenter::getSelectedDeviceType()
{
    return model->getSelectedDeviceType();
}
