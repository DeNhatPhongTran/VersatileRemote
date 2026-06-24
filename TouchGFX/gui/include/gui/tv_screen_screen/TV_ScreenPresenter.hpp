#ifndef TV_SCREENPRESENTER_HPP
#define TV_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class TV_ScreenView;

class TV_ScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    TV_ScreenPresenter(TV_ScreenView& v);

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

    virtual ~TV_ScreenPresenter() {}

private:
    TV_ScreenPresenter();

    TV_ScreenView& view;
};

#endif // TV_SCREENPRESENTER_HPP
