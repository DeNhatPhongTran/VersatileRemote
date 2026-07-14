#include <gui/tv_screen_screen/TV_ScreenView.hpp>
#include <touchgfx/Color.hpp>
#include <cstring>

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
    upload_signal.setAction(buttonClickedCallback);
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
    else if (&src == &upload_signal)
    {
        presenter->onUploadSignalPressed();
    }
}

void TV_ScreenView::setUploadSignalState(bool isUploading, const char* learningButton)
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
    button_ok.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);
    button_up_1.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);
    button_down_1.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);
    button_ok_1.setBoxWithBorderColors(currentBtnColor, normalPressed, currentBtnColor, normalPressedBorder);

    // Set power button upload state
    bool isPowerLearning = (isUploading && learningButton && strcmp(learningButton, "POWER") == 0);
    button_power.setUploadState(isUploading, isPowerLearning);
    button_power.invalidate();

    // Highlight the active learning button
    if (isUploading && learningButton && learningButton[0] != '\0')
    {
        if (strcmp(learningButton, "CH+") == 0)
            button_up.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "CH-") == 0)
            button_down.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "OK") == 0)
            button_ok.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "VOL+") == 0)
            button_up_1.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "VOL-") == 0)
            button_down_1.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
        else if (strcmp(learningButton, "MUTE") == 0)
            button_ok_1.setBoxWithBorderColors(learningColor, normalPressed, learningColor, normalPressedBorder);
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
    button_ok.invalidate();
    button_up_1.invalidate();
    button_down_1.invalidate();
    button_ok_1.invalidate();
    upload_signal.invalidate();
}
