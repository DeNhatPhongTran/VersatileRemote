#include <gui/containers/ButtonDevice.hpp>

ButtonDevice::ButtonDevice() :
    itemIndex(-1),
    clickHandler(0),
    clickContext(0),
    deviceClickCallback(this, &ButtonDevice::deviceClickCallbackHandler)
{

}

void ButtonDevice::initialize()
{
    ButtonDeviceBase::initialize();
    device.setAction(deviceClickCallback);
}

void ButtonDevice::setText(const char* text)
{
}

void ButtonDevice::setItemIndex(int16_t index)
{
    itemIndex = index;
}

int16_t ButtonDevice::getItemIndex() const
{
    return itemIndex;
}

void ButtonDevice::setClickHandler(void (*handler)(void* context, int16_t itemIndex), void* context)
{
    clickHandler = handler;
    clickContext = context;
}

void ButtonDevice::deviceClickCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (clickHandler)
    {
        clickHandler(clickContext, itemIndex);
    }
}
