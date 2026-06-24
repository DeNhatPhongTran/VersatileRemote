#ifndef TV_SCREENVIEW_HPP
#define TV_SCREENVIEW_HPP

#include <gui_generated/tv_screen_screen/TV_ScreenViewBase.hpp>
#include <gui/tv_screen_screen/TV_ScreenPresenter.hpp>

class TV_ScreenView : public TV_ScreenViewBase
{
public:
    TV_ScreenView();
    virtual ~TV_ScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // TV_SCREENVIEW_HPP
