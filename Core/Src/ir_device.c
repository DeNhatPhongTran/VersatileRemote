/**
 ******************************************************************************
 * @file    ir_device.c
 * @brief   Device entity and button management implementation.
 ******************************************************************************
 */

#include "ir_device.h"
#include <string.h>
#include <stdio.h>

static ir_device_t *s_registry[IR_MAX_DEVICES];
static uint8_t      s_registry_count = 0;

void ir_device_init(ir_device_t *dev, const char *name, ir_device_type_t type)
{
    if (dev == NULL) return;
    memset(dev, 0, sizeof(ir_device_t));

    if (name != NULL) {
        strncpy(dev->name, name, IR_MAX_NAME_LEN - 1u);
        dev->name[IR_MAX_NAME_LEN - 1u] = '\0';
    }
    dev->type         = type;
    dev->button_count = 0u;
}

int ir_device_add_button(ir_device_t *dev, const char *name, const ir_signal_t *signal)
{
    if (dev == NULL || name == NULL || signal == NULL) return 0;
    if (dev->button_count >= IR_MAX_BUTTONS) return 0;

    ir_button_t *btn = &dev->buttons[dev->button_count];

    strncpy(btn->name, name, IR_MAX_NAME_LEN - 1u);
    btn->name[IR_MAX_NAME_LEN - 1u] = '\0';

    memcpy(&btn->signal, signal, sizeof(ir_signal_t));

    dev->button_count++;
    return 1;
}

const ir_signal_t *ir_device_get_signal(const ir_device_t *dev, const char *name)
{
    if (dev == NULL || name == NULL) return NULL;

    for (uint8_t i = 0; i < dev->button_count; i++) {
        if (strncmp(dev->buttons[i].name, name, IR_MAX_NAME_LEN) == 0) {
            return &dev->buttons[i].signal;
        }
    }
    return NULL;
}

const ir_button_t *ir_device_get_button_by_index(const ir_device_t *dev, uint8_t index)
{
    if (dev == NULL || index >= dev->button_count) return NULL;
    return &dev->buttons[index];
}

const char *ir_device_type_name(ir_device_type_t type)
{
    switch (type) {
        case IR_DEV_TV:              return "TV";
        case IR_DEV_AIR_CONDITIONER: return "Air Conditioner";
        case IR_DEV_FAN:             return "Fan";
        case IR_DEV_SET_TOP_BOX:     return "Set-Top Box";
        case IR_DEV_PROJECTOR:       return "Projector";
        case IR_DEV_AUDIO:           return "Audio";
        default:                     return "Generic";
    }
}

void ir_device_print(const ir_device_t *dev)
{
    if (dev == NULL) return;
    printf("Device   : %s (%s)\r\n", dev->name, ir_device_type_name(dev->type));
    printf("Buttons  : %u\r\n", dev->button_count);
    for (uint8_t i = 0; i < dev->button_count; i++) {
        printf("  [%2u] %s\r\n", i, dev->buttons[i].name);
    }
}

int ir_registry_add(ir_device_t *dev)
{
    if (dev == NULL || s_registry_count >= IR_MAX_DEVICES) return 0;
    s_registry[s_registry_count++] = dev;
    return 1;
}

ir_device_t *ir_registry_find(const char *name)
{
    if (name == NULL) return NULL;
    for (uint8_t i = 0; i < s_registry_count; i++) {
        if (strncmp(s_registry[i]->name, name, IR_MAX_NAME_LEN) == 0) {
            return s_registry[i];
        }
    }
    return NULL;
}

uint8_t ir_registry_count(void)
{
    return s_registry_count;
}

ir_device_t *ir_registry_get(uint8_t index)
{
    if (index >= s_registry_count) return NULL;
    return s_registry[index];
}
