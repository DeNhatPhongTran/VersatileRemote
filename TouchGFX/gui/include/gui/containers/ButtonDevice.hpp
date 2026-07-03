#ifndef BUTTONDEVICE_HPP
#define BUTTONDEVICE_HPP

#include <gui_generated/containers/ButtonDeviceBase.hpp>

class ButtonDevice : public ButtonDeviceBase
{
public:
    ButtonDevice();
    virtual ~ButtonDevice() {}

    virtual void initialize();

    void setText(const char* text);

    void setItemIndex(int16_t index);
    int16_t getItemIndex() const;

    void setClickHandler(void (*handler)(void* context, int16_t itemIndex), void* context);

protected:
    int16_t itemIndex;

    void (*clickHandler)(void* context, int16_t itemIndex);
    void* clickContext;

    touchgfx::Callback<ButtonDevice, const touchgfx::AbstractButtonContainer&> deviceClickCallback;
    void deviceClickCallbackHandler(const touchgfx::AbstractButtonContainer& src);
};

#endif // BUTTONDEVICE_HPP
