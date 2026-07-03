#include <gui/ac_screen_screen/AC_ScreenView.hpp>
#include <gui/ac_screen_screen/AC_ScreenPresenter.hpp>
#include <gui/model/Model.hpp>

AC_ScreenPresenter::AC_ScreenPresenter(AC_ScreenView& v)
    : view(v)
{

}

void AC_ScreenPresenter::activate()
{
    model->setSelectedDeviceType(DEVICE_AC);

    DeviceEntry filtered[20];
    int count = model->getDevicesByType(DEVICE_AC, filtered, 20);
    view.updateDeviceList(filtered, count);
}

void AC_ScreenPresenter::deactivate()
{

}

void AC_ScreenPresenter::onDeviceSelected(const DeviceEntry& device)
{
    model->setActiveDevice(device);
}
