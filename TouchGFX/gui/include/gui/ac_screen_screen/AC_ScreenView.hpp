#ifndef AC_SCREENVIEW_HPP
#define AC_SCREENVIEW_HPP

#include <gui_generated/ac_screen_screen/AC_ScreenViewBase.hpp>
#include <gui/ac_screen_screen/AC_ScreenPresenter.hpp>

class AC_ScreenView : public AC_ScreenViewBase
{
public:
    AC_ScreenView();
    virtual ~AC_ScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // AC_SCREENVIEW_HPP
