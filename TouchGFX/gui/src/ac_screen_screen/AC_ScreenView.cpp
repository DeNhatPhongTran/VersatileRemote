#include <gui/ac_screen_screen/AC_ScreenView.hpp>

AC_ScreenView::AC_ScreenView()
{

}

void AC_ScreenView::setupScreen()
{
    AC_ScreenViewBase::setupScreen();
}

void AC_ScreenView::tearDownScreen()
{
    AC_ScreenViewBase::tearDownScreen();
}

void AC_ScreenView::updateDeviceList(const DeviceEntry* devs, int count)
{
    device_menu.setDevices(devs, count);
}
