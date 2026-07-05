#include <gui/tv_screen_screen/TV_ScreenView.hpp>

TV_ScreenView::TV_ScreenView() :
    buttonClickedCallback(this, &TV_ScreenView::buttonClickedCallbackHandler)
{

}

void TV_ScreenView::setupScreen()
{
    TV_ScreenViewBase::setupScreen();
    device_menu.setItemClickCallback(TV_ScreenView::onDeviceMenuItemClick, this);

    // Bind action callbacks to TV remote buttons
    button_power.setAction(buttonClickedCallback);
    button_up.setAction(buttonClickedCallback);     // CH+
    button_down.setAction(buttonClickedCallback);   // CH-
    button_ok.setAction(buttonClickedCallback);     // OK
    button_up_1.setAction(buttonClickedCallback);   // VOL+
    button_down_1.setAction(buttonClickedCallback); // VOL-
    button_ok_1.setAction(buttonClickedCallback);   // MUTE
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

void TV_ScreenView::buttonClickedCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (button_power.isPressed(src))
    {
        presenter->onButtonPressed("POWER");
    }
    else if (&src == &button_up)
    {
        presenter->onButtonPressed("CH+");
    }
    else if (&src == &button_down)
    {
        presenter->onButtonPressed("CH-");
    }
    else if (&src == &button_ok)
    {
        presenter->onButtonPressed("OK");
    }
    else if (&src == &button_up_1)
    {
        presenter->onButtonPressed("VOL+");
    }
    else if (&src == &button_down_1)
    {
        presenter->onButtonPressed("VOL-");
    }
    else if (&src == &button_ok_1)
    {
        presenter->onButtonPressed("MUTE");
    }
}
