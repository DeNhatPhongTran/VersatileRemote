/**
 ******************************************************************************
 * @file    ir_signal.h
 * @brief   Core data structures for storing and representing infrared signals.
 *
 * Supports both protocol-decoded signals (NEC, Sony SIRC, RC5) and raw
 * pulse-timing signals (e.g. for Air Conditioners or unknown protocols).
 * All timing values are in microseconds (us).
 ******************************************************************************
 */

#ifndef INC_IR_SIGNAL_H_
#define INC_IR_SIGNAL_H_

#include <stdint.h>
#include <stddef.h>

/* ---------------------------------------------------------------------------
 * Configuration Constants
 * ---------------------------------------------------------------------------*/

/** Maximum number of raw mark/space timings in one frame.
 *  AC remotes typically send 200-300 pulse transitions.
 *  TV remotes (NEC) send ~68 transitions. 256 covers most cases. */
#define IR_MAX_RAW_TIMINGS  256u

/** Gap duration (us) with no signal that marks the end of an IR frame. */
#define IR_FRAME_GAP_US     15000u

/* ---------------------------------------------------------------------------
 * Protocol Enum
 * ---------------------------------------------------------------------------*/

/**
 * @brief Identifies the encoding protocol of an IR signal.
 *
 * IR_PROTO_RAW     : No known protocol detected; raw timings are stored.
 * IR_PROTO_NEC     : NEC 32-bit protocol (most common TV remotes).
 * IR_PROTO_NEC_EXT : NEC Extended (16-bit address instead of 8-bit).
 * IR_PROTO_SONY12  : Sony SIRC 12-bit protocol.
 * IR_PROTO_SONY15  : Sony SIRC 15-bit protocol.
 * IR_PROTO_SONY20  : Sony SIRC 20-bit protocol.
 * IR_PROTO_RC5     : Philips RC5 bi-phase Manchester protocol.
 * IR_PROTO_UNKNOWN : Not yet decoded.
 */
typedef enum {
    IR_PROTO_UNKNOWN = 0,
    IR_PROTO_RAW,
    IR_PROTO_NEC,
    IR_PROTO_NEC_EXT,
    IR_PROTO_SONY12,
    IR_PROTO_SONY15,
    IR_PROTO_SONY20,
    IR_PROTO_RC5
} ir_protocol_t;

/* ---------------------------------------------------------------------------
 * IR Signal Structure
 * ---------------------------------------------------------------------------*/

/**
 * @brief Unified representation of a captured or stored IR signal.
 *
 * For decoded protocols (NEC/Sony/RC5), `address` and `command` hold the
 * decoded data and `raw_len` is 0.
 *
 * For raw/unknown signals (e.g. AC remotes), `protocol` is IR_PROTO_RAW,
 * `raw_timings` holds alternating mark (IR burst) and space (silence)
 * durations in microseconds, and `raw_len` is the number of entries.
 * The array always starts with a MARK.
 *
 * `address` and `command` may also be populated for decoded protocols
 * even when raw_timings are stored (for cache/diagnostic purposes).
 */
typedef struct {
    ir_protocol_t  protocol;          /*!< Protocol used to encode this signal          */
    uint32_t       address;           /*!< Protocol address field (decoded)              */
    uint32_t       command;           /*!< Protocol command/button code (decoded)        */
    uint8_t        bits;              /*!< Number of protocol data bits (decoded)        */

    /* Raw pulse timing storage */
    uint16_t       raw_timings[IR_MAX_RAW_TIMINGS]; /*!< Mark/space durations in us   */
    uint16_t       raw_len;           /*!< Number of valid entries in raw_timings        */
} ir_signal_t;

/* ---------------------------------------------------------------------------
 * Utility Declarations
 * ---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Reset an ir_signal_t to a clean state (all zeros).
 * @param  sig  Pointer to the signal structure to reset.
 */
void ir_signal_reset(ir_signal_t *sig);

/**
 * @brief  Append a timing value to the raw buffer of a signal.
 * @param  sig      Pointer to the signal.
 * @param  timing   Duration in microseconds to append.
 * @retval 1 if appended successfully, 0 if buffer is full.
 */
int ir_signal_append_timing(ir_signal_t *sig, uint16_t timing);

/**
 * @brief  Get a human-readable protocol name string.
 * @param  proto  Protocol enum value.
 * @retval Constant C string (e.g., "NEC", "RAW").
 */
const char *ir_protocol_name(ir_protocol_t proto);

/**
 * @brief  Print signal details to UART via printf (debug).
 * @param  sig  Pointer to the signal to print.
 */
void ir_signal_print(const ir_signal_t *sig);

/**
 * @brief  Transmit the IR signal (modulated at appropriate frequency).
 * @param  sig  Pointer to the signal to transmit.
 */
void ir_transmit(const ir_signal_t *sig);

#ifdef __cplusplus
}
#endif

#endif /* INC_IR_SIGNAL_H_ */
