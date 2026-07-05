#include <gui/add_device_screen/Add_DeviceView.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdio.h>

Add_DeviceView::Add_DeviceView() :
    acceptButtonCallback(this, &Add_DeviceView::acceptButtonCallbackHandler),
    cancelButtonCallback(this, &Add_DeviceView::cancelButtonCallbackHandler)
{
    typeNameBuffer[0] = '\0';
}

void Add_DeviceView::setupScreen()
{
    Add_DeviceViewBase::setupScreen();
    
    // Bind action callbacks to accept and cancel buttons
    button_accept.setAction(acceptButtonCallback);
    button_cancel.setAction(cancelButtonCallback);
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
    printf("Add_DeviceView: navigating back for type %d\r\n", type);
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
    printf("Add_DeviceView: Accept button clicked! &src=%p, &button_accept=%p\r\n", 
           (void*)&src, (void*)static_cast<const touchgfx::AbstractButtonContainer*>(&button_accept));
    if (&src == static_cast<const touchgfx::AbstractButtonContainer*>(&button_accept))
    {
        presenter->onConfirmAddDevice();
    }
}

void Add_DeviceView::cancelButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    printf("Add_DeviceView: Cancel button clicked! &src=%p, &button_cancel=%p\r\n", 
           (void*)&src, (void*)static_cast<const touchgfx::AbstractButtonContainer*>(&button_cancel));
    if (&src == static_cast<const touchgfx::AbstractButtonContainer*>(&button_cancel))
    {
        // Go back to the correct screen depending on selected type
        navigateBack(presenter->getSelectedDeviceType());
    }
}
