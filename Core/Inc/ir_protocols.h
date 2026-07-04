/**
 ******************************************************************************
 * @file    ir_protocols.h
 * @brief   IR protocol decoder and encoder declarations.
 *
 * Supports NEC (standard + extended), Sony SIRC (12/15/20-bit), RC5, and
 * a raw fallback for unknown/AC protocols.
 *
 * Usage:
 *   1. Capture raw timings into an ir_signal_t via ir_signal_append_timing().
 *   2. Call ir_decode() to attempt protocol identification and decode.
 *   3. Call ir_encode() to convert a decoded ir_signal_t back to raw timings
 *      for retransmission.
 ******************************************************************************
 */

#ifndef INC_IR_PROTOCOLS_H_
#define INC_IR_PROTOCOLS_H_

#include "ir_signal.h"

/* ---------------------------------------------------------------------------
 * Tolerance configuration (±%)
 * Most protocols allow ±20-25% timing tolerance.
 * ---------------------------------------------------------------------------*/
#define IR_TOLERANCE_PERCENT  25u

/* ---------------------------------------------------------------------------
 * NEC Protocol Timing Constants (us)
 * ---------------------------------------------------------------------------*/
#define NEC_LEAD_MARK       9000u   /*!< Leading mark (burst)                  */
#define NEC_LEAD_SPACE      4500u   /*!< Leading space after burst              */
#define NEC_REPEAT_SPACE    2250u   /*!< Space for repeat frame                 */
#define NEC_BIT_MARK         560u   /*!< Mark for every bit                    */
#define NEC_ONE_SPACE       1690u   /*!< Space for logic '1'                   */
#define NEC_ZERO_SPACE       560u   /*!< Space for logic '0'                   */

/* ---------------------------------------------------------------------------
 * Sony SIRC Protocol Timing Constants (us)
 * ---------------------------------------------------------------------------*/
#define SONY_LEAD_MARK      2400u   /*!< Leading mark (burst)                  */
#define SONY_LEAD_SPACE      600u   /*!< Leading space after burst              */
#define SONY_ONE_MARK       1200u   /*!< Mark for logic '1'                    */
#define SONY_ZERO_MARK       600u   /*!< Mark for logic '0'                    */
#define SONY_BIT_SPACE       600u   /*!< Space after every bit                 */

/* ---------------------------------------------------------------------------
 * RC5 Protocol Timing Constants (us)
 * ---------------------------------------------------------------------------*/
#define RC5_BIT_PERIOD       889u   /*!< Half-bit period (889us per half)      */

/* ---------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------------*/

/**
 * @brief  Attempt to decode raw timings in `sig` using all supported protocols.
 *
 * If a known protocol is detected, `sig->protocol`, `sig->address`,
 * `sig->command`, and `sig->bits` are populated.
 * If no protocol matches, `sig->protocol` is set to IR_PROTO_RAW and
 * the raw timings are preserved unchanged.
 *
 * @param  sig  Pointer to ir_signal_t populated with raw_timings.
 * @retval 1 if a known protocol was decoded, 0 if stored as RAW.
 */
int ir_decode(ir_signal_t *sig);

/**
 * @brief  Encode a decoded signal back into raw mark/space timings.
 *
 * For decoded protocols (NEC/Sony/RC5), regenerates the raw_timings array
 * from address and command. For IR_PROTO_RAW, the raw_timings are used as-is.
 *
 * @param  sig  Pointer to ir_signal_t with protocol/address/command set.
 * @retval 1 if encoding was successful, 0 on error.
 */
int ir_encode(ir_signal_t *sig);

/* ---------------------------------------------------------------------------
 * Protocol-specific decoders (used internally, exposed for testing)
 * ---------------------------------------------------------------------------*/

/**
 * @brief  Attempt NEC protocol decode from raw timings.
 * @param  sig  Pointer to signal with raw_timings filled.
 * @retval 1 if NEC frame detected and decoded, 0 otherwise.
 */
int ir_decode_nec(ir_signal_t *sig);

/**
 * @brief  Attempt Sony SIRC protocol decode from raw timings.
 * @param  sig  Pointer to signal with raw_timings filled.
 * @retval 1 if SIRC frame detected and decoded, 0 otherwise.
 */
int ir_decode_sony(ir_signal_t *sig);

/**
 * @brief  Attempt RC5 protocol decode from raw timings.
 * @param  sig  Pointer to signal with raw_timings filled.
 * @retval 1 if RC5 frame detected and decoded, 0 otherwise.
 */
int ir_decode_rc5(ir_signal_t *sig);

/* ---------------------------------------------------------------------------
 * Protocol-specific encoders (used internally, exposed for testing)
 * ---------------------------------------------------------------------------*/

/**
 * @brief  Encode an NEC signal into raw_timings.
 * @param  sig  Pointer to signal with protocol/address/command set.
 * @retval 1 on success.
 */
int ir_encode_nec(ir_signal_t *sig);

/**
 * @brief  Encode a Sony SIRC signal into raw_timings.
 * @param  sig  Pointer to signal with protocol/address/command/bits set.
 * @retval 1 on success.
 */
int ir_encode_sony(ir_signal_t *sig);

/**
 * @brief  Encode an RC5 signal into raw_timings.
 * @param  sig  Pointer to signal with protocol/address/command set.
 * @retval 1 on success.
 */
int ir_encode_rc5(ir_signal_t *sig);

/* ---------------------------------------------------------------------------
 * Timing comparison helper (exposed for testing)
 * ---------------------------------------------------------------------------*/

/**
 * @brief  Check if a measured timing is within tolerance of an expected value.
 * @param  measured   Actual timing in us.
 * @param  expected   Reference timing in us.
 * @retval 1 if within IR_TOLERANCE_PERCENT, 0 otherwise.
 */
int ir_timing_match(uint16_t measured, uint32_t expected);

#endif /* INC_IR_PROTOCOLS_H_ */
