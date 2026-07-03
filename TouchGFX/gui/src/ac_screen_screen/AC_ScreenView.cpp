#include <gui/ac_screen_screen/AC_ScreenView.hpp>

AC_ScreenView::AC_ScreenView()
{

}

void AC_ScreenView::setupScreen()
{
    AC_ScreenViewBase::setupScreen();
    device_menu.setItemClickCallback(AC_ScreenView::onDeviceMenuItemClick, this);
}

void AC_ScreenView::tearDownScreen()
{
    AC_ScreenViewBase::tearDownScreen();
}

void AC_ScreenView::updateDeviceList(const DeviceEntry* devs, int count)
{
    device_menu.setDevices(devs, count);
}

void AC_ScreenView::onDeviceMenuItemClick(void* context, int16_t itemIndex)
{
    static_cast<AC_ScreenView*>(context)->handleDeviceSelected(itemIndex);
}

void AC_ScreenView::handleDeviceSelected(int16_t itemIndex)
{
    const DeviceEntry* device = device_menu.getDeviceAt(itemIndex);
    if (device)
    {
        presenter->onDeviceSelected(*device);
    }
}
