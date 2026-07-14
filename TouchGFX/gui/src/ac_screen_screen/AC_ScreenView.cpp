#include <gui/ac_screen_screen/AC_ScreenView.hpp>
#include <touchgfx/Color.hpp>
#include <cstring>

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
    upload_signal.setAction(buttonClickedCallback);
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
    else if (&src == &upload_signal)
    {
        presenter->onUploadSignalPressed();
    }
}

void AC_ScreenView::setUploadSignalState(bool isUploading, const char* learningButton)
{
    // Define normal and highlight colors
    uint32_t normalColor = touchgfx::Color::getColorFromRGB(135, 135, 135);
    uint32_t normalPressed = touchgfx::Color::getColorFromRGB(0, 153, 204);
    uint32_t normalPressedBorder = touchgfx::Color::getColorFromRGB(51, 102, 153);

    uint32_t waitingColor = touchgfx::Color::getColorFromRGB(230, 200, 0); // Yellow
    uint32_t learningColor = touchgfx::Color::getColorFromRGB(220, 50, 50); // Red

    // By default, set all buttons to normal or waiting
    uint32_t currentBtnColor = isUploading ? waitingColor : normalColor;

    button_up.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);
    button_down.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);
    button_up_1.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);
    button_down_1.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);
    button_dry.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);
    button_alarm.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);
    button_canhquat.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);
    button_up_1_1.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);

    // Set power button upload state
    bool isPowerLearning = (isUploading && learningButton && strcmp(learningButton, "POWER") == 0);
    button_power.setUploadState(isUploading, isPowerLearning);
    button_power.invalidate();

    // Highlight the active learning button
    if (isUploading && learningButton && learningButton[0] != '\0')
    {
        if (strcmp(learningButton, "TEMP+") == 0)
            button_up.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "TEMP-") == 0)
            button_down.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "FAN+") == 0)
            button_up_1.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "FAN-") == 0)
            button_down_1.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "DRY") == 0)
            button_dry.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "ALARM") == 0)
            button_alarm.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "SWING") == 0)
            button_canhquat.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "MODE") == 0)
            button_up_1_1.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
    }

    // Change style/color of upload_signal button to show status
    if (isUploading)
    {
        // Highlight upload button as active/green
        upload_signal.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(50, 180, 50), normalPressed, touchgfx::Color::getColorFromRGB(30, 120, 30), normalPressedBorder);
    }
    else
    {
        upload_signal.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(0, 102, 153), normalPressed, touchgfx::Color::getColorFromRGB(0, 51, 102), normalPressedBorder);
    }

    // Invalidate everything to force redraw
    button_up.invalidate();
    button_down.invalidate();
    button_up_1.invalidate();
    button_down_1.invalidate();
    button_dry.invalidate();
    button_alarm.invalidate();
    button_canhquat.invalidate();
    button_up_1_1.invalidate();
    upload_signal.invalidate();
}
