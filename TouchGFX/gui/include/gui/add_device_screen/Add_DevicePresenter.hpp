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

    virtual void activate();
    virtual void deactivate();

    void onConfirmAddDevice();

    virtual ~Add_DevicePresenter() {}

private:
    Add_DevicePresenter();

    Add_DeviceView& view;
};

#endif // ADD_DEVICEPRESENTER_HPP
