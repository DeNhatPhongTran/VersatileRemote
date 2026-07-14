#ifndef AC_SCREENVIEW_HPP
#define AC_SCREENVIEW_HPP

#include <gui_generated/ac_screen_screen/AC_ScreenViewBase.hpp>
#include <gui/ac_screen_screen/AC_ScreenPresenter.hpp>
#include <gui/model/Model.hpp>

class AC_ScreenView : public AC_ScreenViewBase
{
public:
    AC_ScreenView();
    virtual ~AC_ScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateDeviceList(const DeviceEntry* devs, int count);
    void setUploadSignalState(bool isUploading, const char* learningButton);

protected:
    static void onDeviceMenuItemClick(void* context, int16_t itemIndex);
    void handleDeviceSelected(int16_t itemIndex);

    touchgfx::Callback<AC_ScreenView, const touchgfx::AbstractButtonContainer&> buttonClickedCallback;
    void buttonClickedCallbackHandler(const touchgfx::AbstractButtonContainer& src);
};

#endif // AC_SCREENVIEW_HPP
