#include <gui/add_device_screen/Add_DeviceView.hpp>
#include <touchgfx/Unicode.hpp>

Add_DeviceView::Add_DeviceView() :
    acceptButtonCallback(this, &Add_DeviceView::acceptButtonCallbackHandler)
{
    typeNameBuffer[0] = '\0';
}

void Add_DeviceView::setupScreen()
{
    Add_DeviceViewBase::setupScreen();
    button_accept.setAction(acceptButtonCallback);
}

void Add_DeviceView::tearDownScreen()
{
    Add_DeviceViewBase::tearDownScreen();
}

void Add_DeviceView::showDeviceType(DeviceType type)
{
    const char* typeName = "Device";
    switch (type)
    {
        case DEVICE_TV:
            typeName = "TV";
            break;
        case DEVICE_AC:
            typeName = "AC";
            break;
        case DEVICE_PROJECTOR:
            typeName = "Projector";
            break;
        default:
            break;
    }

    touchgfx::Unicode::strncpy(typeNameBuffer, typeName, 32);
    deviceName.setWildcard(typeNameBuffer);
    deviceName.invalidate();
}

void Add_DeviceView::navigateBack(DeviceType type)
{
    switch (type)
    {
        case DEVICE_TV:
            application().gotoTV_ScreenScreenSlideTransitionWest();
            break;
        case DEVICE_AC:
            application().gotoAC_ScreenScreenNoTransition();
            break;
        default:
            application().gotoTV_ScreenScreenSlideTransitionWest();
            break;
    }
}

void Add_DeviceView::acceptButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &button_accept)
    {
        presenter->onConfirmAddDevice();
    }
}
