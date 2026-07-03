#ifndef TV_SCREENPRESENTER_HPP
#define TV_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <gui/model/Model.hpp>

using namespace touchgfx;

class TV_ScreenView;

class TV_ScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    TV_ScreenPresenter(TV_ScreenView& v);

    virtual void activate();
    virtual void deactivate();

    void onDeviceSelected(const DeviceEntry& device);

    virtual ~TV_ScreenPresenter() {}

private:
    TV_ScreenPresenter();

    TV_ScreenView& view;
};

#endif // TV_SCREENPRESENTER_HPP
