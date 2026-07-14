#include <gui/ac_screen_screen/AC_ScreenView.hpp>
#include <gui/ac_screen_screen/AC_ScreenPresenter.hpp>
#include <gui/model/Model.hpp>

AC_ScreenPresenter::AC_ScreenPresenter(AC_ScreenView& v)
    : view(v)
{

}

void AC_ScreenPresenter::activate()
{
    model->setSelectedDeviceType(DEVICE_AC);

    DeviceEntry filtered[20];
    int count = model->getDevicesByType(DEVICE_AC, filtered, 20);
    view.updateDeviceList(filtered, count);

    if (count > 0)
    {
        model->setActiveDevice(filtered[0]);
    }
}

void AC_ScreenPresenter::deactivate()
{
    model->stopUploadSignal();
}

void AC_ScreenPresenter::onDeviceSelected(const DeviceEntry& device)
{
    model->stopUploadSignal();
    model->setActiveDevice(device);
}

void AC_ScreenPresenter::onButtonPressed(const char* buttonName)
{
    if (model->isUploadMode())
    {
        model->handleButtonPressInUpload(buttonName);
    }
    else
    {
        model->transmitActiveDeviceSignal(buttonName);
    }
}

void AC_ScreenPresenter::onUploadSignalPressed()
{
    if (model->isUploadMode())
    {
        model->stopUploadSignal();
    }
    else
    {
        model->startUploadSignal();
    }
}

void AC_ScreenPresenter::uploadSignalStateChanged(bool isUploading, const char* learningButton)
{
    view.setUploadSignalState(isUploading, learningButton);
}
