#include <gui/tv_screen_screen/TV_ScreenView.hpp>

TV_ScreenView::TV_ScreenView()
{

}

void TV_ScreenView::setupScreen()
{
    TV_ScreenViewBase::setupScreen();
}

void TV_ScreenView::tearDownScreen()
{
    TV_ScreenViewBase::tearDownScreen();
}

void TV_ScreenView::updateDeviceList(const DeviceEntry* devs, int count)
{
    device_menu.setDevices(devs, count);
}
