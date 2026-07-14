#include <gui/tv_screen_screen/TV_ScreenView.hpp>
#include <gui/tv_screen_screen/TV_ScreenPresenter.hpp>
#include <gui/model/Model.hpp>

TV_ScreenPresenter::TV_ScreenPresenter(TV_ScreenView& v)
    : view(v)
{

}

void TV_ScreenPresenter::activate()
{
    model->setSelectedDeviceType(DEVICE_TV);

    DeviceEntry filtered[20];
    int count = model->getDevicesByType(DEVICE_TV, filtered, 20);
    view.updateDeviceList(filtered, count);

    if (count > 0)
    {
        model->setActiveDevice(filtered[0]);
    }
}

void TV_ScreenPresenter::deactivate()
{
    model->stopUploadSignal();
}

void TV_ScreenPresenter::onDeviceSelected(const DeviceEntry& device)
{
    model->stopUploadSignal();
    model->setActiveDevice(device);
}

void TV_ScreenPresenter::onButtonPressed(const char* buttonName)
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

void TV_ScreenPresenter::onUploadSignalPressed()
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

void TV_ScreenPresenter::uploadSignalStateChanged(bool isUploading, const char* learningButton)
{
    view.setUploadSignalState(isUploading, learningButton);
}
