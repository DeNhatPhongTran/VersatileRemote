#ifndef ADD_DEVICEVIEW_HPP
#define ADD_DEVICEVIEW_HPP

#include <gui_generated/add_device_screen/Add_DeviceViewBase.hpp>
#include <gui/add_device_screen/Add_DevicePresenter.hpp>
#include <gui/model/Model.hpp>

class Add_DeviceView : public Add_DeviceViewBase
{
public:
    Add_DeviceView();
    virtual ~Add_DeviceView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void showDeviceType(DeviceType type);

protected:
    touchgfx::Callback<Add_DeviceView, const touchgfx::AbstractButtonContainer&> acceptButtonCallback;
    void acceptButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

    touchgfx::Unicode::UnicodeChar typeNameBuffer[32];
};

#endif // ADD_DEVICEVIEW_HPP
