#ifndef AC_SCREENPRESENTER_HPP
#define AC_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <gui/model/Model.hpp>

using namespace touchgfx;

class AC_ScreenView;

class AC_ScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    AC_ScreenPresenter(AC_ScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    void onDeviceSelected(const DeviceEntry& device);
    void onButtonPressed(const char* buttonName);
    void onUploadSignalPressed();

    virtual void uploadSignalStateChanged(bool isUploading, const char* learningButton);

    virtual ~AC_ScreenPresenter() {}

private:
    AC_ScreenPresenter();

    AC_ScreenView& view;
};

#endif // AC_SCREENPRESENTER_HPP
