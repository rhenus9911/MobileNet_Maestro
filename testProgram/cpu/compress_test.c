#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <time.h>

// 압축할 데이터의 크기 (예: 10MB)
#define DATA_SIZE (10 * 1024 * 1024)

void generate_random_data(unsigned char *data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        data[i] = rand() % 256;
    }
}

void compress_data(const unsigned char *source, size_t source_size, unsigned char *dest, size_t *dest_size) {
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    
    if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        fprintf(stderr, "deflateInit failed\n");
        exit(EXIT_FAILURE);
    }
    
    stream.next_in = (unsigned char *)source;
    stream.avail_in = source_size;
    stream.next_out = dest;
    stream.avail_out = *dest_size;
    
    if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
        fprintf(stderr, "deflate failed\n");
        exit(EXIT_FAILURE);
    }
    
    *dest_size = stream.total_out;
    
    deflateEnd(&stream);
}

void decompress_data(const unsigned char *source, size_t source_size, unsigned char *dest, size_t *dest_size) {
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    
    if (inflateInit(&stream) != Z_OK) {
        fprintf(stderr, "inflateInit failed\n");
        exit(EXIT_FAILURE);
    }
    
    stream.next_in = (unsigned char *)source;
    stream.avail_in = source_size;
    stream.next_out = dest;
    stream.avail_out = *dest_size;
    
    if (inflate(&stream, Z_FINISH) != Z_STREAM_END) {
        fprintf(stderr, "inflate failed\n");
        exit(EXIT_FAILURE);
    }
    
    *dest_size = stream.total_out;
    
    inflateEnd(&stream);
}

int main() {
    unsigned char *source_data = malloc(DATA_SIZE);
    unsigned char *compressed_data = malloc(compressBound(DATA_SIZE));
    unsigned char *decompressed_data = malloc(DATA_SIZE);
    
    if (!source_data || !compressed_data || !decompressed_data) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    generate_random_data(source_data, DATA_SIZE);
    
    size_t compressed_size = compressBound(DATA_SIZE);
    size_t decompressed_size = DATA_SIZE;
    
    clock_t start_time, end_time;
    
    // 압축 성능 테스트
    start_time = clock();
    compress_data(source_data, DATA_SIZE, compressed_data, &compressed_size);
    end_time = clock();
    double compress_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Compression time: %.2f seconds\n", compress_time);
    
    // 압축 해제 성능 테스트
    start_time = clock();
    decompress_data(compressed_data, compressed_size, decompressed_data, &decompressed_size);
    end_time = clock();
    double decompress_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Decompression time: %.2f seconds\n", decompress_time);
    
    // 데이터 검증
    if (decompressed_size != DATA_SIZE || memcmp(source_data, decompressed_data, DATA_SIZE) != 0) {
        fprintf(stderr, "Data mismatch\n");
        return EXIT_FAILURE;
    }
    
    free(source_data);
    free(compressed_data);
    free(decompressed_data);
    
    return EXIT_SUCCESS;
}

