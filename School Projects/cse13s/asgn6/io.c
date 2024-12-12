#include <stdio.h>
#include <unistd.h>
#include "code.h"
#include "io.h"
#include "endian.h"

extern uint8_t read_buffer[BLOCK];
extern uint32_t read_index;
extern uint8_t write_buffer[BLOCK];
extern uint32_t write_index;

uint64_t total_bits = 0;
uint64_t total_syms = 0;
uint8_t read_buffer[BLOCK];
uint32_t read_index = 0;
uint8_t write_buffer[BLOCK];
uint32_t write_index = 0;

int read_bytes(int infile, uint8_t *buf, int to_read) {
    /* concerns with the buffer being overwritten */

    int readBytes = 0;
    int temp = 0;
    while (readBytes < to_read && (temp = read(infile, buf, to_read - readBytes)) > 0) {
        readBytes += temp;
    }
    return readBytes;
}

int write_bytes(int outfile, uint8_t *buf, int to_write) {
    /* concerns with the buffer being overwritten */

    // same as read but with write()
    int writtenBytes = 0;
    int temp = 0;
    while (writtenBytes < to_write && (temp = write(outfile, buf, to_write - writtenBytes)) >= 0) {
        /* printf("temp = %d\n", temp); */
        writtenBytes += temp;
    }
    return writtenBytes;
}

void read_header(int infile, FileHeader *header) {
    // since we want to read sizeof(FileHeader) bytes
    // we call our read_bytes() function with to_read set to
    // sizeof(FileHeader)

    /* uint8_t *buffer = (uint8_t*) calloc(BLOCK * sizeof(uint8_t)); */
    read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));

    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
    return;
}

void write_header(int outfile, FileHeader *header) {
    /* uint8_t *buffer = (uint8_t*) calloc(BLOCK * sizeof(uint8_t)); */

    /* check if this is correct */
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));

    return;
}

bool read_sym(int infile, uint8_t *sym) {
    if ((read_index == BLOCK) || (read_buffer[read_index] == 0x00 && read_index == 0)) {
        if (read_index == BLOCK) {
            for (int i = 0; i < BLOCK; i++) {
                read_buffer[i] = 0x00;
            }
        }
        read_bytes(infile, read_buffer, BLOCK);
        read_index = 0;
    }

    *sym = read_buffer[read_index];
    read_index++;

    if (*sym != 0x00) {
        total_syms++;
        return true;
    }

    else {
        return false;
    }
}

void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    // check if little endian, if not, swap bytes
    int bit_index = total_bits % 8;
    while (bitlen > 0) {
        if (write_index == BLOCK) {
            flush_pairs(outfile);
        }

        if (bitlen > (8 - bit_index)) {
            write_buffer[write_index] |= (code << bit_index);
            write_index++;
            code >>= (8 - bit_index);
            bitlen -= (8 - bit_index);
            total_bits += (8 - bit_index);
            bit_index = total_bits % 8;
        }

        else {
            write_buffer[write_index] |= (code << bit_index);
            total_bits += bitlen;
            bitlen = 0;
            bit_index = total_bits % 8; /* see if you can consolidate into 1 line*/
            if (bit_index == 0) {
                write_index++;
            }
        }
    }

    if (write_index == BLOCK) {
        flush_pairs(outfile);
    }

    if (bit_index == 0) {
        write_buffer[write_index] = sym;
        total_bits += 8;
        write_index++;
    }

    else {
        write_buffer[write_index] |= (sym << bit_index);
        write_index++;
        if (write_index == BLOCK) {
            flush_pairs(outfile);
        }
        write_buffer[write_index] = sym >> (8 - bit_index);
        total_bits += 8;
    }

    return;
}

void flush_pairs(int outfile) {
    if (write_index != BLOCK && (total_bits % 8) != 0) {
        write_index++;
    }
    write_bytes(outfile, write_buffer, write_index);
    for (int i = 0; i < BLOCK; i++) {
        write_buffer[i] = 0x00;
    }
    write_index = 0;
    return;
}

bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    *code = 0x0000;
    int bit_index = total_bits % 8;
    uint16_t bitmask = 0;
    unsigned int total_read = 0;
    while (bitlen > 0) {
        if ((read_index == BLOCK) | (read_buffer[read_index] == 0x00 && read_index == 0)) {
            if (read_index == BLOCK) {
                for (int i = 0; i < BLOCK; i++) {
                    read_buffer[i] = 0x00;
                }
            }
            read_bytes(infile, read_buffer, BLOCK);
            read_index = 0;
        }

        if (bitlen > (8 - bit_index)) {
            *code |= (((read_buffer[read_index]) >> bit_index) << total_read); /*split?*/
            read_index++;
            total_read += (8 - bit_index);
            total_bits += (8 - bit_index);
            bitlen -= (8 - bit_index);
            bit_index = total_bits % 8;
            if (read_index == BLOCK) {
                for (int i = 0; i < BLOCK; i++) {
                    read_buffer[i] = 0x00;
                }
                read_bytes(infile, read_buffer, BLOCK);
                read_index = 0;
            }
        }

        else {
            *code |= (((read_buffer[read_index]) >> bit_index) << total_read); /*split?*/
            total_bits += bitlen;
            total_read += bitlen;
            for (unsigned int i = 0; i < total_read; i++) {
                bitmask |= (1 << i);
            }
            *code &= bitmask;
            bitlen = 0;
            bit_index = total_bits % 8; /* consolidate into 1 line? */
            if (bit_index == 0) {
                read_index++;
            }
        }
    }

    if (read_index == BLOCK) {
        for (int i = 0; i < BLOCK; i++) {
            read_buffer[i] = 0x00;
        }
        read_bytes(infile, read_buffer, BLOCK);
        read_index = 0;
    }

    *sym = 0x00;
    total_read = 0;
    while (total_read < 8) {
        if (read_index == BLOCK) {
            for (int i = 0; i < BLOCK; i++) {
                read_buffer[i] = 0x00;
            }
            read_bytes(infile, read_buffer, BLOCK);
            read_index = 0;
        }

        if (bit_index != 0) {
            *sym |= (read_buffer[read_index] >> bit_index);
            total_read += (8 - bit_index);
            total_bits += total_read;
            bit_index = total_bits % 8;
            read_index++;
        }

        else {
            *sym |= (read_buffer[read_index] << total_read);
            total_bits += (8 - total_read);
            total_read += (8 - total_read);
            bit_index = total_bits % 8; /* consolidate into 1 line */
            if (bit_index == 0) {
                read_index++;
            }
        }
    }

    if (*code == STOP_CODE) {
        return false;
    }

    return true;
}

void write_word(int outfile, Word *w) {
    if (write_index == BLOCK) {
        flush_words(outfile);
        write_index = 0;
    }

    for (unsigned int i = 0; i < (w->len); i++) {
        write_buffer[write_index] = (w->syms[i]);
        write_index++;
        if (write_index == BLOCK) {
            flush_words(outfile);
            write_index = 0;
        }
    }

    total_syms += (w->len);

    return;
}

void flush_words(int outfile) {
    write_bytes(outfile, write_buffer, write_index);
    for (int i = 0; i < BLOCK; i++) {
        write_buffer[i] = 0x00;
    }
    return;
}
