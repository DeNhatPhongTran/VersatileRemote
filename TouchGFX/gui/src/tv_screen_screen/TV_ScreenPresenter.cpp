#include <gui/tv_screen_screen/TV_ScreenView.hpp>
#include <gui/tv_screen_screen/TV_ScreenPresenter.hpp>
#include <gui/model/Model.hpp>

TV_ScreenPresenter::TV_ScreenPresenter(TV_ScreenView& v)
    : view(v)
{

}

void TV_ScreenPresenter::activate()
{
    model->setSelectedDeviceType(DEVICE_TV);

    DeviceEntry filtered[20];
    int count = model->getDevicesByType(DEVICE_TV, filtered, 20);
    view.updateDeviceList(filtered, count);
}

void TV_ScreenPresenter::deactivate()
{

}

void TV_ScreenPresenter::onDeviceSelected(const DeviceEntry& device)
{
    model->setActiveDevice(device);
}
