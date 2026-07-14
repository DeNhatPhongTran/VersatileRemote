/**
 ******************************************************************************
 * @file    ir_protocols.c
 * @brief   IR protocol decoder and encoder implementations.
 ******************************************************************************
 */

#include "ir_protocols.h"
#include <string.h>
#include <stdio.h>

int ir_timing_match(uint16_t measured, uint32_t expected)
{
    uint32_t delta = (expected * IR_TOLERANCE_PERCENT) / 100u;
    return ((uint32_t)measured >= (expected - delta)) &&
           ((uint32_t)measured <= (expected + delta));
}

int ir_decode_nec(ir_signal_t *sig)
{
    if (sig->raw_len < 66) return 0;

    if (!ir_timing_match(sig->raw_timings[0], NEC_LEAD_MARK)) return 0;

    uint8_t is_repeat = 0;
    if (ir_timing_match(sig->raw_timings[1], NEC_LEAD_SPACE)) {
        is_repeat = 0;
    } else if (ir_timing_match(sig->raw_timings[1], NEC_REPEAT_SPACE)) {
        is_repeat = 1;
    } else {
        return 0;
    }

    if (is_repeat) {
        sig->protocol = IR_PROTO_NEC;
        return 1;
    }

    uint32_t data = 0;
    for (uint8_t i = 0; i < 32; i++) {
        uint16_t mark  = sig->raw_timings[2 + i * 2];
        uint16_t space = sig->raw_timings[3 + i * 2];

        if (!ir_timing_match(mark, NEC_BIT_MARK)) return 0;

        if (ir_timing_match(space, NEC_ONE_SPACE)) {
            data |= (1UL << i);
        } else if (ir_timing_match(space, NEC_ZERO_SPACE)) {
            /* bit stays 0 */
        } else {
            return 0;
        }
    }

    uint8_t addr     = (uint8_t)(data & 0xFF);
    uint8_t addr_inv = (uint8_t)((data >> 8) & 0xFF);
    uint8_t cmd      = (uint8_t)((data >> 16) & 0xFF);
    uint8_t cmd_inv  = (uint8_t)((data >> 24) & 0xFF);

    if ((addr ^ addr_inv) != 0xFF) {
        sig->protocol = IR_PROTO_NEC_EXT;
        sig->address  = (uint32_t)addr | ((uint32_t)addr_inv << 8);
        sig->command  = cmd;
        sig->bits     = 32;
    } else {
        if ((cmd ^ cmd_inv) != 0xFF) return 0;
        sig->protocol = IR_PROTO_NEC;
        sig->address  = addr;
        sig->command  = cmd;
        sig->bits     = 32;
    }

    return 1;
}

int ir_encode_nec(ir_signal_t *sig)
{
    uint8_t addr = (uint8_t)(sig->address & 0xFF);
    uint8_t cmd  = (uint8_t)(sig->command  & 0xFF);

    sig->raw_len = 0;

    sig->raw_timings[sig->raw_len++] = (uint16_t)NEC_LEAD_MARK;
    sig->raw_timings[sig->raw_len++] = (uint16_t)NEC_LEAD_SPACE;

    uint8_t addr_inv = ~addr;
    uint8_t cmd_inv  = ~cmd;
    uint32_t data    = (uint32_t)addr |
                       ((uint32_t)addr_inv << 8) |
                       ((uint32_t)cmd      << 16) |
                       ((uint32_t)cmd_inv  << 24);

    for (uint8_t i = 0; i < 32; i++) {
        sig->raw_timings[sig->raw_len++] = (uint16_t)NEC_BIT_MARK;
        if ((data >> i) & 1u) {
            sig->raw_timings[sig->raw_len++] = (uint16_t)NEC_ONE_SPACE;
        } else {
            sig->raw_timings[sig->raw_len++] = (uint16_t)NEC_ZERO_SPACE;
        }
    }

    sig->raw_timings[sig->raw_len++] = (uint16_t)NEC_BIT_MARK;

    return 1;
}

int ir_decode_sony(ir_signal_t *sig)
{
    if (sig->raw_len < 25) return 0;

    if (!ir_timing_match(sig->raw_timings[0], SONY_LEAD_MARK)) return 0;
    if (!ir_timing_match(sig->raw_timings[1], SONY_LEAD_SPACE)) return 0;

    uint8_t nbits;
    if (sig->raw_len >= 41 && sig->raw_len <= 43) {
        nbits = 20;
    } else if (sig->raw_len >= 31 && sig->raw_len <= 33) {
        nbits = 15;
    } else if (sig->raw_len >= 24 && sig->raw_len <= 26) {
        nbits = 12;
    } else {
        return 0;
    }

    uint32_t data = 0;
    for (uint8_t i = 0; i < nbits; i++) {
        uint16_t mark = sig->raw_timings[2 + i * 2];
        if (ir_timing_match(mark, SONY_ONE_MARK)) {
            data |= (1UL << i);
        } else if (ir_timing_match(mark, SONY_ZERO_MARK)) {
            /* bit stays 0 */
        } else {
            return 0;
        }
        if (i < (uint8_t)(nbits - 1u)) {
            if (!ir_timing_match(sig->raw_timings[3 + i * 2], SONY_BIT_SPACE)) {
                return 0;
            }
        }
    }

    sig->bits    = nbits;
    sig->command = data & 0x7Fu;

    switch (nbits) {
        case 12:
            sig->protocol = IR_PROTO_SONY12;
            sig->address  = (data >> 7) & 0x1Fu;
            break;
        case 15:
            sig->protocol = IR_PROTO_SONY15;
            sig->address  = (data >> 7) & 0xFFu;
            break;
        case 20:
            sig->protocol = IR_PROTO_SONY20;
            sig->address  = (data >> 7) & 0x1Fu;
            sig->address |= ((data >> 12) & 0xFFu) << 8;
            break;
        default:
            return 0;
    }

    return 1;
}

int ir_encode_sony(ir_signal_t *sig)
{
    uint8_t nbits = sig->bits;
    if (nbits != 12 && nbits != 15 && nbits != 20) return 0;

    uint32_t data = (sig->command & 0x7Fu);
    if (nbits == 12) {
        data |= ((sig->address & 0x1Fu) << 7);
    } else if (nbits == 15) {
        data |= ((sig->address & 0xFFu) << 7);
    } else {
        data |= ((sig->address & 0x1Fu) << 7);
        data |= (((sig->address >> 8) & 0xFFu) << 12);
    }

    sig->raw_len = 0;
    sig->raw_timings[sig->raw_len++] = (uint16_t)SONY_LEAD_MARK;
    sig->raw_timings[sig->raw_len++] = (uint16_t)SONY_LEAD_SPACE;

    for (uint8_t i = 0; i < nbits; i++) {
        if ((data >> i) & 1u) {
            sig->raw_timings[sig->raw_len++] = (uint16_t)SONY_ONE_MARK;
        } else {
            sig->raw_timings[sig->raw_len++] = (uint16_t)SONY_ZERO_MARK;
        }
        if (i < (uint8_t)(nbits - 1u)) {
            sig->raw_timings[sig->raw_len++] = (uint16_t)SONY_BIT_SPACE;
        }
    }

    return 1;
}

int ir_decode_rc5(ir_signal_t *sig)
{
    if (sig->raw_len < 10) return 0;

    if (!ir_timing_match(sig->raw_timings[0], RC5_BIT_PERIOD) &&
        !ir_timing_match(sig->raw_timings[0], RC5_BIT_PERIOD * 2)) {
        return 0;
    }

    uint8_t bits[28] = {0};
    uint8_t nbit = 0;

    for (uint16_t i = 0; i < sig->raw_len && nbit < 28; i++) {
        uint16_t t = sig->raw_timings[i];
        if (ir_timing_match(t, RC5_BIT_PERIOD)) {
            bits[nbit++] = (i % 2 == 0) ? 1u : 0u;
        } else if (ir_timing_match(t, RC5_BIT_PERIOD * 2)) {
            uint8_t v = (i % 2 == 0) ? 1u : 0u;
            bits[nbit++] = v;
            bits[nbit++] = v;
        } else {
            return 0;
        }
    }

    if (nbit != 28) return 0;

    uint16_t data = 0;
    for (uint8_t b = 0; b < 14; b++) {
        uint8_t h1 = bits[b * 2];
        uint8_t h2 = bits[b * 2 + 1];
        if (h1 == 0 && h2 == 1) {
            data |= (1u << (13 - b));
        } else if (h1 == 1 && h2 == 0) {
            /* bit stays 0 */
        } else {
            return 0;
        }
    }

    sig->protocol = IR_PROTO_RC5;
    sig->address  = (data >> 6) & 0x1Fu;
    sig->command  = data & 0x3Fu;
    sig->bits     = 14;

    return 1;
}

int ir_encode_rc5(ir_signal_t *sig)
{
    uint16_t data = (1u << 13) | (1u << 12) |
                    ((sig->address & 0x1Fu) << 6) |
                    (sig->command & 0x3Fu);

    sig->raw_len = 0;
    uint8_t last_level = 0;

    for (int8_t b = 13; b >= 0; b--) {
        uint8_t bit_val = (data >> b) & 1u;
        uint8_t h1 = bit_val ? 0u : 1u;
        uint8_t h2 = bit_val ? 1u : 0u;

        if (sig->raw_len > 0 && last_level == h1) {
            sig->raw_timings[sig->raw_len - 1] += (uint16_t)RC5_BIT_PERIOD;
        } else {
            sig->raw_timings[sig->raw_len++] = (uint16_t)RC5_BIT_PERIOD;
        }
        last_level = h1;

        if (last_level == h2) {
            sig->raw_timings[sig->raw_len - 1] += (uint16_t)RC5_BIT_PERIOD;
        } else {
            sig->raw_timings[sig->raw_len++] = (uint16_t)RC5_BIT_PERIOD;
        }
        last_level = h2;
    }

    return 1;
}

int ir_decode(ir_signal_t *sig)
{
    if (sig == NULL || sig->raw_len == 0) return 0;

    if (ir_decode_nec(sig))  return 1;
    if (ir_decode_sony(sig)) return 1;
    if (ir_decode_rc5(sig))  return 1;

    sig->protocol = IR_PROTO_RAW;
    return 0;
}

int ir_encode(ir_signal_t *sig)
{
    if (sig == NULL) return 0;

    switch (sig->protocol) {
        case IR_PROTO_NEC:
        case IR_PROTO_NEC_EXT:
            return ir_encode_nec(sig);
        case IR_PROTO_SONY12:
        case IR_PROTO_SONY15:
        case IR_PROTO_SONY20:
            return ir_encode_sony(sig);
        case IR_PROTO_RC5:
            return ir_encode_rc5(sig);
        case IR_PROTO_RAW:
            return (sig->raw_len > 0) ? 1 : 0;
        default:
            return 0;
    }
}
