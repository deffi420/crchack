#ifndef CRC_H
#define CRC_H

#include "bigint.h"

/* CRC algorithm parameters */
struct crc_config {
    int width;              /* CRC register width in bits */
    struct bigint poly;     /* generator polynomial */
    struct bigint init;     /* initial register value */
    struct bigint xor_out;  /* final register XOR mask */
    int reflect_in;         /* reverse input bits (LSB first instead of MSB) */
    int reflect_out;        /* reverse final register */
};

/* Calculate CRC checksum of a (j-i+1)-bit message[i..j] */
void crc_bits(const struct crc_config *crc, const void *msg, size_t i, size_t j,
              struct bigint *checksum);

/* Calculate CRC checksum of a len-byte message */
void crc(const struct crc_config *crc, const void *msg, size_t len,
         struct bigint *checksum);

/* Append a message to an existing checksum calculated */
void crc_append(const struct crc_config *crc, const void *msg, size_t len,
                struct bigint *checksum);

/* CRC sparse engine for efficient checksum calculation of sparse inputs */
struct crc_sparse {
    struct crc_config crc;  /* CRC algorithm */
    size_t size;            /* message size in bits */

    struct bigint *D;       /* difference matrix */
    struct bigint *L;       /* left matrix table */
    struct bigint *R;       /* right matrix table */
    struct bigint *PQ;      /* P & Q work matrix */
};

/* New CRC sparse engine for size-bit long message */
struct crc_sparse *crc_sparse_new(const struct crc_config *crc, size_t size);

/* Adjust CRC checksum for a message with bit flip in the given position */
int crc_sparse_1bit(struct crc_sparse *engine, size_t bitpos,
                    struct bigint *checksum);

/* Delete CRC sparse engine */
void crc_sparse_delete(struct crc_sparse *engine);

#endif
