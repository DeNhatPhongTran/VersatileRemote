/**
 ******************************************************************************
 * @file    ir_signal.c
 * @brief   IR signal utility function implementations.
 ******************************************************************************
 */

#include "ir_signal.h"
#include <string.h>
#include <stdio.h>

void ir_signal_reset(ir_signal_t *sig)
{
    if (sig == NULL) return;
    memset(sig, 0, sizeof(ir_signal_t));
}

int ir_signal_append_timing(ir_signal_t *sig, uint16_t timing)
{
    if (sig == NULL || sig->raw_len >= IR_MAX_RAW_TIMINGS) return 0;
    sig->raw_timings[sig->raw_len++] = timing;
    return 1;
}

const char *ir_protocol_name(ir_protocol_t proto)
{
    switch (proto) {
        case IR_PROTO_NEC:     return "NEC";
        case IR_PROTO_NEC_EXT: return "NEC_EXT";
        case IR_PROTO_SONY12:  return "SONY_12";
        case IR_PROTO_SONY15:  return "SONY_15";
        case IR_PROTO_SONY20:  return "SONY_20";
        case IR_PROTO_RC5:     return "RC5";
        case IR_PROTO_RAW:     return "RAW";
        default:               return "UNKNOWN";
    }
}

void ir_signal_print(const ir_signal_t *sig)
{
    if (sig == NULL) return;
    printf("Protocol : %s\r\n", ir_protocol_name(sig->protocol));
    if (sig->protocol != IR_PROTO_RAW && sig->protocol != IR_PROTO_UNKNOWN) {
        printf("Address  : 0x%08lX\r\n", (unsigned long)sig->address);
        printf("Command  : 0x%08lX\r\n", (unsigned long)sig->command);
        printf("Bits     : %u\r\n", sig->bits);
    }
    if (sig->raw_len > 0) {
        printf("Raw[%3u] : ", sig->raw_len);
        for (uint16_t i = 0; i < sig->raw_len && i < 8; i++) {
            printf("%u ", sig->raw_timings[i]);
        }
        if (sig->raw_len > 8) printf("...");
        printf("\r\n");
    }
}
