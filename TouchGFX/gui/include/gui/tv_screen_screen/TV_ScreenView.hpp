#ifndef TV_SCREENVIEW_HPP
#define TV_SCREENVIEW_HPP

#include <gui_generated/tv_screen_screen/TV_ScreenViewBase.hpp>
#include <gui/tv_screen_screen/TV_ScreenPresenter.hpp>
#include <gui/model/Model.hpp>

class TV_ScreenView : public TV_ScreenViewBase
{
public:
    TV_ScreenView();
    virtual ~TV_ScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateDeviceList(const DeviceEntry* devs, int count);

protected:
    static void onDeviceMenuItemClick(void* context, int16_t itemIndex);
    void handleDeviceSelected(int16_t itemIndex);

    touchgfx::Callback<TV_ScreenView, const touchgfx::AbstractButtonContainer&> buttonClickedCallback;
    void buttonClickedCallbackHandler(const touchgfx::AbstractButtonContainer& src);
};

#endif // TV_SCREENVIEW_HPP
