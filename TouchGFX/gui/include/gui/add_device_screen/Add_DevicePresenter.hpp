#ifndef ADD_DEVICEPRESENTER_HPP
#define ADD_DEVICEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Add_DeviceView;

class Add_DevicePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Add_DevicePresenter(Add_DeviceView& v);

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

    virtual ~Add_DevicePresenter() {}

private:
    Add_DevicePresenter();

    Add_DeviceView& view;
};

#endif // ADD_DEVICEPRESENTER_HPP
