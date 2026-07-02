#include <gui/add_device_screen/Add_DeviceView.hpp>
#include <touchgfx/Unicode.hpp>

Add_DeviceView::Add_DeviceView() :
    acceptButtonCallback(this, &Add_DeviceView::acceptButtonCallbackHandler)
{

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

    touchgfx::Unicode::UnicodeChar buf[32];
    uint16_t len = 0;
    len += touchgfx::Unicode::strncpy(buf + len, "Add ", 32 - len);
    len += touchgfx::Unicode::strncpy(buf + len, typeName, 32 - len);
    len += touchgfx::Unicode::strncpy(buf + len, " Device", 32 - len);
    buf[len] = '\0';
    deviceName.setWildcard(buf);
    deviceName.invalidate();
}

void Add_DeviceView::acceptButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &button_accept)
    {
        presenter->onConfirmAddDevice();
    }
}
