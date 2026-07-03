#include <gui/tv_screen_screen/TV_ScreenView.hpp>

TV_ScreenView::TV_ScreenView()
{

}

void TV_ScreenView::setupScreen()
{
    TV_ScreenViewBase::setupScreen();
    device_menu.setItemClickCallback(TV_ScreenView::onDeviceMenuItemClick, this);
}

void TV_ScreenView::tearDownScreen()
{
    TV_ScreenViewBase::tearDownScreen();
}

void TV_ScreenView::updateDeviceList(const DeviceEntry* devs, int count)
{
    device_menu.setDevices(devs, count);
}

void TV_ScreenView::onDeviceMenuItemClick(void* context, int16_t itemIndex)
{
    static_cast<TV_ScreenView*>(context)->handleDeviceSelected(itemIndex);
}

void TV_ScreenView::handleDeviceSelected(int16_t itemIndex)
{
    const DeviceEntry* device = device_menu.getDeviceAt(itemIndex);
    if (device)
    {
        presenter->onDeviceSelected(*device);
    }
}
