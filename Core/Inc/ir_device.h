/**
 ******************************************************************************
 * @file    ir_device.h
 * @brief   Device entity and button management for the Smart Remote.
 *
 * Models a controllable device (TV, Air Conditioner, Fan, etc.) as a named
 * entity that owns a fixed set of buttons. Each button maps a human-readable
 * name to an ir_signal_t for transmission.
 *
 * Example usage:
 *   ir_device_t tv;
 *   ir_device_init(&tv, "Samsung TV");
 *   ir_device_add_button(&tv, "POWER", &power_signal);
 *   ir_device_add_button(&tv, "VOL+",  &volup_signal);
 *
 *   const ir_signal_t *sig = ir_device_get_signal(&tv, "POWER");
 *   // transmit *sig ...
 ******************************************************************************
 */

#ifndef INC_IR_DEVICE_H_
#define INC_IR_DEVICE_H_

#include "ir_signal.h"

/* ---------------------------------------------------------------------------
 * Configuration
 * ---------------------------------------------------------------------------*/

/** Maximum characters in a device or button name (including null terminator). */
#define IR_MAX_NAME_LEN     32u

/** Maximum number of buttons a single device can hold. */
#define IR_MAX_BUTTONS      12u

/** Maximum number of devices in the global device registry. */
#define IR_MAX_DEVICES      8u

/* ---------------------------------------------------------------------------
 * Device Type Enum
 * ---------------------------------------------------------------------------*/

/**
 * @brief Describes the class of remote-controlled device.
 * Used for display purposes and optional UI icon selection.
 */
typedef enum {
    IR_DEV_GENERIC = 0,
    IR_DEV_TV,
    IR_DEV_AIR_CONDITIONER,
    IR_DEV_FAN,
    IR_DEV_SET_TOP_BOX,
    IR_DEV_PROJECTOR,
    IR_DEV_AUDIO
} ir_device_type_t;

/* ---------------------------------------------------------------------------
 * Button Structure
 * ---------------------------------------------------------------------------*/

/**
 * @brief A named button that owns an IR signal.
 */
typedef struct {
    char         name[IR_MAX_NAME_LEN]; /*!< Button label, e.g. "POWER", "VOL+" */
    ir_signal_t  signal;                /*!< IR signal to transmit on press       */
} ir_button_t;

/* ---------------------------------------------------------------------------
 * Device Structure
 * ---------------------------------------------------------------------------*/

/**
 * @brief A logical device entity with a name, type, and button list.
 */
typedef struct {
    char             name[IR_MAX_NAME_LEN];    /*!< Device display name              */
    ir_device_type_t type;                     /*!< Device class                     */
    ir_button_t      buttons[IR_MAX_BUTTONS];  /*!< Stored button signals            */
    uint8_t          button_count;             /*!< Number of populated buttons       */
} ir_device_t;

/* ---------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------------*/

/**
 * @brief  Initialize a device structure with a name and type.
 * @param  dev   Pointer to ir_device_t to initialize.
 * @param  name  Human-readable device name string.
 * @param  type  Device class (IR_DEV_TV, IR_DEV_AIR_CONDITIONER, etc.).
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialize a device structure with a name and type.
 * @param  dev   Pointer to ir_device_t to initialize.
 * @param  name  Human-readable device name string.
 * @param  type  Device class (IR_DEV_TV, IR_DEV_AIR_CONDITIONER, etc.).
 */
void ir_device_init(ir_device_t *dev, const char *name, ir_device_type_t type);

/**
 * @brief  Add a button (name + signal) to a device.
 * @param  dev     Pointer to the device.
 * @param  name    Button label string (max IR_MAX_NAME_LEN - 1 chars).
 * @param  signal  Pointer to the ir_signal_t to copy into the button slot.
 * @retval 1 if button was added, 0 if device is full.
 */
int ir_device_add_button(ir_device_t *dev, const char *name, const ir_signal_t *signal);

/**
 * @brief  Find a button by name and return its signal.
 * @param  dev   Pointer to the device.
 * @param  name  Button name to search for.
 * @retval Pointer to ir_signal_t if found, NULL otherwise.
 */
const ir_signal_t *ir_device_get_signal(const ir_device_t *dev, const char *name);

/**
 * @brief  Find a button by index.
 * @param  dev    Pointer to the device.
 * @param  index  Zero-based index.
 * @retval Pointer to ir_button_t if index is valid, NULL otherwise.
 */
const ir_button_t *ir_device_get_button_by_index(const ir_device_t *dev, uint8_t index);

/**
 * @brief  Get a human-readable device type string.
 * @param  type  ir_device_type_t enum value.
 * @retval Constant C string (e.g., "TV", "Air Conditioner").
 */
const char *ir_device_type_name(ir_device_type_t type);

/**
 * @brief  Print device and all button names to UART via printf (debug).
 * @param  dev  Pointer to device to print.
 */
void ir_device_print(const ir_device_t *dev);

/* ---------------------------------------------------------------------------
 * Global Device Registry
 * ---------------------------------------------------------------------------*/

/**
 * @brief  Add a device to the global registry.
 * @param  dev  Pointer to a populated ir_device_t.
 * @retval 1 if added, 0 if registry is full.
 */
int ir_registry_add(ir_device_t *dev);

/**
 * @brief  Allocate a static device slot from the registry pool.
 * @param  name  Device name.
 * @param  type  Device type.
 * @retval Pointer to allocated ir_device_t, or NULL if pool is full.
 */
ir_device_t *ir_registry_allocate(const char *name, ir_device_type_t type);

/**
 * @brief  Find a device in the registry by name.
 * @param  name  Device name to look up.
 * @retval Pointer to ir_device_t if found, NULL otherwise.
 */
ir_device_t *ir_registry_find(const char *name);

/**
 * @brief  Return the number of devices currently in the registry.
 * @retval Device count.
 */
uint8_t ir_registry_count(void);

/**
 * @brief  Return a pointer to the device at `index` in the registry.
 * @param  index  Zero-based index.
 * @retval Pointer to ir_device_t, or NULL if out of range.
 */
ir_device_t *ir_registry_get(uint8_t index);

#ifdef __cplusplus
}
#endif

#endif /* INC_IR_DEVICE_H_ */
