#include "lz4_compression.h"

/* lz4 compress function
 * @input: input buffer
 * @input_len: input buffer length
 * @output: output buffer, should be big enough to hold the result
 * @output_len: return the compression length
 */
int lz4_compress(const char *input, int input_len, char *output, int &output_len) {
    LZ4_stream_t *lz4_stream = (LZ4_stream_t *)alloca(sizeof(LZ4_stream_t));
    LZ4_resetStream(lz4_stream);
    const int bench = LZ4_COMPRESSBOUND(COMPRESSION_BLOCK);
    char compress_buffer[2][bench];
    int buffer_index = 0;
    int compress_bytes = 0;
    output_len = 0;

    while (compress_bytes < input_len) {
        int input_size = (input_len - compress_bytes < COMPRESSION_BLOCK) ? (input_len - compress_bytes) : COMPRESSION_BLOCK;
        int bytes = LZ4_compress_fast_continue(lz4_stream, input + compress_bytes, compress_buffer[buffer_index], input_size, bench, 1);
        compress_bytes += input_size;
        memmove(output + output_len, &bytes, sizeof(int));
        memmove(output + output_len + sizeof(int), compress_buffer[buffer_index], bytes);
        output_len += bytes + sizeof(int);
        buffer_index = (buffer_index + 1) % 2;
    }

    int zero = 0;
    memmove(output + output_len, &zero, sizeof(int));
    output_len += 4;

    return 0;
}

/* lz4 decompression
 * @input: input buffer
 * @input_len: input buffer length, give her one if you like
 * @output: output buffer, should be large enough to hold the result
 * @output_len: return the result length
 */
int lz4_decompress(const char *input, int input_len, char *output, int &output_len) {
    LZ4_streamDecode_t *lz4_stream = (LZ4_streamDecode_t *)alloca(sizeof(LZ4_streamDecode_t));
    LZ4_setStreamDecode(lz4_stream, NULL, 0);
    output_len = 0;
    int decompress_bytes = 0;
    char decompress_buffer[2][COMPRESSION_BLOCK];
    int buffer_index = 0;

    for(;;) {
        int input_bytes;
        memmove(&input_bytes, input + decompress_bytes, sizeof(int));
        if (input_bytes == 0)
            break;
        decompress_bytes += sizeof(int);
        int bytes = LZ4_decompress_safe_continue(lz4_stream, input + decompress_bytes, decompress_buffer[buffer_index], input_bytes, COMPRESSION_BLOCK);
        decompress_bytes += input_bytes;
        memmove(output + output_len, decompress_buffer[buffer_index], bytes);
        output_len += bytes;
        buffer_index = (buffer_index + 1) % 2;
    }

    return 0;
}
