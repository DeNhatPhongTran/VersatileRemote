#include <gui/ac_screen_screen/AC_ScreenView.hpp>

AC_ScreenView::AC_ScreenView() :
    buttonClickedCallback(this, &AC_ScreenView::buttonClickedCallbackHandler)
{

}

void AC_ScreenView::setupScreen()
{
    AC_ScreenViewBase::setupScreen();
    device_menu.setItemClickCallback(AC_ScreenView::onDeviceMenuItemClick, this);

    // Bind action callbacks to AC remote buttons
    button_power.setAction(buttonClickedCallback);
    button_up.setAction(buttonClickedCallback);       // Temp+
    button_down.setAction(buttonClickedCallback);     // Temp-
    button_up_1.setAction(buttonClickedCallback);     // Fan+
    button_down_1.setAction(buttonClickedCallback);   // Fan-
    button_dry.setAction(buttonClickedCallback);       // Dry
    button_alarm.setAction(buttonClickedCallback);     // Alarm
    button_canhquat.setAction(buttonClickedCallback);  // Swing
    button_up_1_1.setAction(buttonClickedCallback);   // Mode
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

void AC_ScreenView::buttonClickedCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (button_power.isPressed(src))
    {
        presenter->onButtonPressed("POWER");
    }
    else if (&src == &button_up)
    {
        presenter->onButtonPressed("TEMP+");
    }
    else if (&src == &button_down)
    {
        presenter->onButtonPressed("TEMP-");
    }
    else if (&src == &button_up_1)
    {
        presenter->onButtonPressed("FAN+");
    }
    else if (&src == &button_down_1)
    {
        presenter->onButtonPressed("FAN-");
    }
    else if (&src == &button_dry)
    {
        presenter->onButtonPressed("DRY");
    }
    else if (&src == &button_alarm)
    {
        presenter->onButtonPressed("ALARM");
    }
    else if (&src == &button_canhquat)
    {
        presenter->onButtonPressed("SWING");
    }
    else if (&src == &button_up_1_1)
    {
        presenter->onButtonPressed("MODE");
    }
}
