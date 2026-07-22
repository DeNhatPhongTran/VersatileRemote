#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <string.h>
#include <stdio.h>

// Include C headers from the registry
#include "ir_device.h"
#include "ir_signal.h"

extern "C" {
extern volatile uint8_t g_gui_ir_frame_ready;
extern ir_signal_t g_gui_ir_frame;
extern volatile uint8_t g_buzzer_beep_request;
void ir_receive_flush(void);
}

static ir_device_type_t mapDeviceType(DeviceType type)
{
    switch (type)
    {
        case DEVICE_TV:        return IR_DEV_TV;
        case DEVICE_PROJECTOR: return IR_DEV_PROJECTOR;
        case DEVICE_AC:        return IR_DEV_AIR_CONDITIONER;
        default:               return IR_DEV_GENERIC;
    }
}

Model::Model() : modelListener(0), deviceCount(0), selectedDeviceType(DEVICE_TV), activeDeviceSet(false), isUploading(false)
{
    learningButtonName[0] = '\0';
}

void Model::tick()
{
    if (isUploading && learningButtonName[0] != '\0')
    {
        if (g_gui_ir_frame_ready)
        {
            // Decoded frame is ready.
            if (g_gui_ir_frame.protocol != IR_PROTO_UNKNOWN)
            {
                /* Filter out noise/glitches (valid remote controls send at least 10 pulse transitions) */
                if (g_gui_ir_frame.raw_len < 10)
                {
                    printf("Model: Ignored noise/glitch frame (Len: %d < 10 transitions)\r\n", g_gui_ir_frame.raw_len);
                    g_gui_ir_frame_ready = 0; // Clear flag and continue listening
                    return;
                }

                // Find device in registry and update button signal
                if (activeDeviceSet)
                {
                    ir_device_t* dev = ir_registry_find(activeDevice.name);
                    if (dev)
                    {
                        // Add button (or update if exists)
                        ir_device_add_button(dev, learningButtonName, &g_gui_ir_frame);
                        g_buzzer_beep_request = 1; // Request buzzer beep feedback
                        printf("Model: SUCCESSFULLY LEARNED button '%s' on device '%s'\r\n", learningButtonName, dev->name);
                        printf("  -> Protocol: %s, Address: 0x%04lX, Command: 0x%04lX, Bits: %d, Raw Len: %d\r\n",
                               ir_protocol_name(g_gui_ir_frame.protocol),
                               (unsigned long)g_gui_ir_frame.address,
                               (unsigned long)g_gui_ir_frame.command,
                               g_gui_ir_frame.bits,
                               g_gui_ir_frame.raw_len);
                    }
                    else
                    {
                        printf("Model: ERROR - Active device '%s' not found in registry!\r\n", activeDevice.name);
                    }
                }
                else
                {
                    printf("Model: ERROR - No active device selected! Cannot save learned signal.\r\n");
                }
            }
            // Clear flag
            g_gui_ir_frame_ready = 0;

            // Transition this button back to waiting state
            learningButtonName[0] = '\0';

            // Notify listener
            if (modelListener)
            {
                modelListener->uploadSignalStateChanged(isUploading, learningButtonName);
            }
        }
    }
}

void Model::startUploadSignal()
{
    isUploading = true;
    learningButtonName[0] = '\0';
    printf("Model: Upload signal mode STARTED. Waiting for button selection...\r\n");
    if (modelListener)
    {
        modelListener->uploadSignalStateChanged(isUploading, learningButtonName);
    }
}

void Model::stopUploadSignal()
{
    if (isUploading)
    {
        isUploading = false;
        learningButtonName[0] = '\0';
        printf("Model: Upload signal mode STOPPED/CANCELLED.\r\n");
        if (modelListener)
        {
            modelListener->uploadSignalStateChanged(isUploading, learningButtonName);
        }
    }
}

bool Model::isUploadMode() const
{
    return isUploading;
}

const char* Model::getLearningButton() const
{
    return learningButtonName;
}

void Model::handleButtonPressInUpload(const char* buttonName)
{
    if (!isUploading) return;

    // Flush any pending old IR frames right when we start learning
    ir_receive_flush();

    // Set new learning button
    strncpy(learningButtonName, buttonName, sizeof(learningButtonName) - 1);
    learningButtonName[sizeof(learningButtonName) - 1] = '\0';
    printf("Model: Button '%s' is registered to LEARN. Listening for IR signals...\r\n", buttonName);

    if (modelListener)
    {
        modelListener->uploadSignalStateChanged(isUploading, learningButtonName);
    }
}

int Model::getDevicesByType(DeviceType type, DeviceEntry* out, int maxOut)
{
    ir_device_type_t c_type = mapDeviceType(type);
    int count = 0;
    int registry_size = ir_registry_count();
    
    for (int i = 0; i < registry_size && count < maxOut; i++)
    {
        ir_device_t* dev = ir_registry_get(i);
        if (dev && dev->type == c_type)
        {
            strncpy(out[count].name, dev->name, sizeof(out[count].name) - 1);
            out[count].name[sizeof(out[count].name) - 1] = '\0';
            
            // signalId is just a legacy placeholder for the UI
            strncpy(out[count].signalId, "SIG_C_REGISTRY", sizeof(out[count].signalId) - 1);
            out[count].signalId[sizeof(out[count].signalId) - 1] = '\0';
            
            out[count].type = type;
            count++;
        }
    }
    return count;
}

bool Model::addDevice(const char* name, const char* signalId, DeviceType type)
{
    ir_device_type_t c_type = mapDeviceType(type);
    
    // Allocate statically from the C registry pool
    ir_device_t* new_dev = ir_registry_allocate(name, c_type);
    if (!new_dev) return false;
    
    // Add default mock buttons so they can be triggered from the UI
    ir_signal_t sig;
    ir_signal_reset(&sig);
    sig.protocol = IR_PROTO_NEC;
    sig.address  = 0x01;
    sig.bits     = 32;
    
    if (type == DEVICE_TV)
    {
        sig.command = 0x02; ir_device_add_button(new_dev, "POWER", &sig);
        sig.command = 0x03; ir_device_add_button(new_dev, "VOL+", &sig);
        sig.command = 0x04; ir_device_add_button(new_dev, "VOL-", &sig);
        sig.command = 0x05; ir_device_add_button(new_dev, "CH+", &sig);
        sig.command = 0x06; ir_device_add_button(new_dev, "CH-", &sig);
    }
    else if (type == DEVICE_AC)
    {
        sig.command = 0x11; ir_device_add_button(new_dev, "POWER", &sig);
        sig.command = 0x12; ir_device_add_button(new_dev, "TEMP+", &sig);
        sig.command = 0x13; ir_device_add_button(new_dev, "TEMP-", &sig);
    }
    
    return true;
}

void Model::setSelectedDeviceType(DeviceType t)
{
    selectedDeviceType = t;
}

DeviceType Model::getSelectedDeviceType()
{
    return selectedDeviceType;
}

void Model::setActiveDevice(const DeviceEntry& device)
{
    strncpy(activeDevice.name, device.name, sizeof(activeDevice.name) - 1);
    activeDevice.name[sizeof(activeDevice.name) - 1] = '\0';
    strncpy(activeDevice.signalId, device.signalId, sizeof(activeDevice.signalId) - 1);
    activeDevice.signalId[sizeof(activeDevice.signalId) - 1] = '\0';
    activeDevice.type = device.type;
    activeDeviceSet = true;
}

const DeviceEntry* Model::getActiveDevice() const
{
    if (activeDeviceSet)
    {
        return &activeDevice;
    }
    return 0;
}

bool Model::hasActiveDevice() const
{
    return activeDeviceSet;
}

void Model::transmitActiveDeviceSignal(const char* buttonName)
{
    if (!activeDeviceSet) return;
    
    ir_device_t* dev = ir_registry_find(activeDevice.name);
    if (dev)
    {
        printf("Model: Attempting transmit for button name: %s\r\n", buttonName);
        const ir_signal_t* sig = ir_device_get_signal(dev, buttonName);
        if (sig)
        {
            printf("Model: Transmitting button %s on device %s (Protocol: %s, Command: 0x%02lX)\r\n", buttonName, dev->name, ir_protocol_name(sig->protocol), (unsigned long)sig->command);
            ir_transmit(sig);
        }
        else
        {
            printf("Button %s not found on device %s\r\n", buttonName, dev->name);
        }
    }
    else
    {
        printf("Device %s not found in registry\r\n", activeDevice.name);
    }
}
