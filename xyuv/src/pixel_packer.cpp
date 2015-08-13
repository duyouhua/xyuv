/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Stian Valentin Svedenborg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <xyuv.h>
#include <xyuv/yuv_image.h>
#include <xyuv/frame.h>

#include "config-parser/minicalc/minicalc.h"
#include "utility.h"
#include "assert.h"

#include <algorithm>

namespace xyuv {

using unorm_t = uint64_t;

static inline unorm_t round_to_unorm(double val) {
    using std::floor;
    return static_cast<unorm_t>(floor(val + 0.5));
}

static unorm_t to_unorm(float value, uint8_t integer_bits, uint8_t fractional_bits,
                        const std::pair<float, float> &range) {
    using std::min;
    XYUV_ASSERT_RANGE(0.0f, 1.0f, value);
    XYUV_ASSERT(integer_bits + fractional_bits <= 8 * sizeof(unorm_t));
    XYUV_ASSERT(integer_bits < 8 * sizeof(unorm_t));

    // What is the largest integer value representable
    unorm_t max = (0x1ull << integer_bits) - 1;
    max <<= fractional_bits;

    double dbl_val = value;

    // Scale value to range boundaries:
    dbl_val *= (range.second - range.first);
    dbl_val += range.first;

    // Convert to unorm
    unorm_t bits = round_to_unorm(dbl_val * max);

    return bits;
}

static float from_unorm(unorm_t unorm, uint8_t integer_bits, uint8_t fractional_bits,
                        const std::pair<float, float> &range) {
    XYUV_ASSERT(integer_bits + fractional_bits <= 8 * sizeof(unorm_t));
    XYUV_ASSERT(integer_bits < 8 * sizeof(unorm_t));

    unorm_t max = (0x1ull << integer_bits) - 1;
    max <<= fractional_bits;

    // Convert to double
    double dbl_val = static_cast<double>(unorm) / max;

    // Convert back to [0,1]
    dbl_val -= range.first;
    dbl_val /= (range.second - range.first);

    return clamp(0.0f, 1.0f, static_cast<float>(dbl_val));
}

static inline void set_bit(uint8_t *buffer, uint64_t offset, bool val) {
    uint8_t &byte = buffer[offset / 8];
    uint8_t mask = static_cast<uint8_t>(0x80 >> (offset % 8));
    byte = static_cast<uint8_t>((val ? mask : 0) | (byte & ~mask));
}

static inline bool get_bit(const uint8_t *buffer, uint64_t offset) {
    uint8_t mask = static_cast<uint8_t>(0x80 >> (offset % 8));
    return (buffer[offset / 8] & mask) != 0;
}

// Offset in bits form MSB to start of region.
static void write_unorm(uint8_t *buffer, uint64_t offset, uint8_t bits, unorm_t value) {
    // TODO: This is highly inefficient. Make it better.
    unorm_t bit_mask = static_cast<unorm_t>(0x1) << (bits - 1);
    for (uint8_t i = 0; i < bits; i++) {
        set_bit(buffer, offset + i, (bit_mask & value) != 0);
        bit_mask >>= 1;
    }
}


// Offset in bits form MSB to start of region.
static unorm_t read_unorm(const uint8_t *buffer, uint64_t offset, uint8_t bits) {

    // TODO: This is highly inefficient. Make it better.
    unorm_t unorm = 0;
    unorm_t bit_mask = static_cast<unorm_t>(0x1) << (bits - 1);
    for (uint8_t i = 0; i < bits; i++) {
        if (get_bit(buffer, offset + i)) {
            unorm |= bit_mask;
        }
        bit_mask >>= 1;
    }

    return unorm;
}

// The block iterator will iterate over each pixel in a block before moving on to the next one.
class block_iterator {
public:
    block_iterator(uint16_t block_w, uint16_t block_h, surface<float> *surf) :
            block_w(block_w), block_h(block_h),
            block_x(0), block_y(0),
            x(0), y(0),
            surf(surf) { }

    block_iterator(uint16_t block_w, uint16_t block_h, const surface<float> *surf) :
            block_w(block_w), block_h(block_h),
            block_x(0), block_y(0),
            x(0), y(0),
            surf(const_cast<surface<float> *>(surf)) { }

    float *advance() {
        float *ret = &(surf->at(block_x + x, block_y + y));
        if (++x == block_w) {
            x = 0;
            if (++y == block_h) {
                y = 0;
                if ((block_x += block_w) >= surf->width()) {
                    block_x = 0;
                    block_y += block_h;
                }
            }
        }
        return ret;
    }

private:
    uint32_t block_w, block_h;
    uint32_t block_x, block_y;
    uint32_t x, y;
    surface<float> *surf;
};

static void encode_channel(uint8_t *base_addr, const channel_block &block, const surface<float> &surf,
                           const std::vector<plane> &planes, const std::pair<float, float> range) {
    block_iterator it(block.w, block.h, &surf);

    uint32_t n_blocks_in_line = (surf.width() / block.w);
    uint32_t n_block_lines = (surf.height() / block.h);

    for (uint32_t line = 0; line < n_block_lines; line++) {
        for (uint32_t b = 0; b < n_blocks_in_line; b++) {
            for (auto &sample : block.samples) {
                float value = *it.advance();

                unorm_t unorm = to_unorm(value, sample.integer_bits, sample.fractional_bits, range);
                write_unorm(base_addr + planes[sample.plane].base_offset + line * planes[sample.plane].line_stride,
                            b * planes[sample.plane].block_stride + sample.offset,
                            sample.integer_bits + sample.fractional_bits, unorm);
            }
        }
    }
}

static xyuv::frame internal_encode_frame(const yuv_image &yuva_in, const xyuv::format &format) {

    // Determine the byte size of each plane.
    bool has_y = !format.channel_blocks[channel::Y].samples.empty();
    bool has_u = !format.channel_blocks[channel::U].samples.empty();
    bool has_v = !format.channel_blocks[channel::V].samples.empty();
    bool has_a = !format.channel_blocks[channel::A].samples.empty();

    std::unique_ptr<uint8_t[]> buffer = std::unique_ptr<uint8_t[]>(new uint8_t[format.size]);

    if (has_y)
        encode_channel(
                buffer.get(),
                format.channel_blocks[channel::Y],
                yuva_in.y_plane,
                format.planes,
                format.conversion_matrix.y_packed_range
        );

    if (has_u)
        encode_channel(
                buffer.get(),
                format.channel_blocks[channel::U],
                yuva_in.u_plane,
                format.planes,
                format.conversion_matrix.u_packed_range
        );
    if (has_v)
        encode_channel(
                buffer.get(),
                format.channel_blocks[channel::V],
                yuva_in.v_plane,
                format.planes,
                format.conversion_matrix.v_packed_range
        );

    if (has_a)
        encode_channel(
                buffer.get(),
                format.channel_blocks[channel::A],
                yuva_in.a_plane,
                format.planes,
                std::make_pair<float, float>(0.0f, 1.0f)
        );


    // Init frame info.
    xyuv::frame frame;
    frame.data = std::move(buffer);
    frame.format = format;

    return frame;
}

static void decode_channel(const uint8_t *base_addr, const channel_block &block, surface<float> *surf,
                           const std::vector<plane> &planes, const std::pair<float, float> range) {
    block_iterator it(block.w, block.h, surf);

    uint32_t n_blocks_in_line = (surf->width() / block.w);
    uint32_t n_block_lines = (surf->height() / block.h);

    for (uint32_t line = 0; line < n_block_lines; line++) {
        for (uint32_t b = 0; b < n_blocks_in_line; b++) {
            for (auto &sample : block.samples) {
                float *value = it.advance();

                unorm_t unorm = read_unorm(
                        base_addr + planes[sample.plane].base_offset + line * planes[sample.plane].line_stride,
                        b * planes[sample.plane].block_stride + sample.offset,
                        sample.integer_bits + sample.fractional_bits);

                *value = from_unorm(unorm, sample.integer_bits, sample.fractional_bits, range);
            }
        }
    }
}


yuv_image decode_frame(const xyuv::frame &frame_in) {

    // Determine the size of each plane.
    bool has_y = !frame_in.format.channel_blocks[channel::Y].samples.empty();
    bool has_u = !frame_in.format.channel_blocks[channel::U].samples.empty();
    bool has_v = !frame_in.format.channel_blocks[channel::V].samples.empty();
    bool has_a = !frame_in.format.channel_blocks[channel::A].samples.empty();

    yuv_image yuva_out = create_yuv_image(
            frame_in.format.image_w,
            frame_in.format.image_h,
            frame_in.format.chroma_siting,
            has_y,
            has_u,
            has_v,
            has_a
    );

    if (has_y)
        decode_channel(
                frame_in.data.get(),
                frame_in.format.channel_blocks[channel::Y],
                &(yuva_out.y_plane),
                frame_in.format.planes,
                frame_in.format.conversion_matrix.y_packed_range
        );
    if (has_u)
        decode_channel(
                frame_in.data.get(),
                frame_in.format.channel_blocks[channel::U],
                &(yuva_out.u_plane),
                frame_in.format.planes,
                frame_in.format.conversion_matrix.u_packed_range
        );
    if (has_v)
        decode_channel(
                frame_in.data.get(),
                frame_in.format.channel_blocks[channel::V],
                &(yuva_out.v_plane),
                frame_in.format.planes,
                frame_in.format.conversion_matrix.v_packed_range
        );
    if (has_a)
        decode_channel(
                frame_in.data.get(),
                frame_in.format.channel_blocks[channel::A],
                &(yuva_out.a_plane),
                frame_in.format.planes,
                std::make_pair<float, float>(0.0f, 1.0f)
        );

    return yuva_out;
}

// This is the entry_point for the encode function, and will make sure the result
// sane.
xyuv::frame encode_frame(const xyuv::yuv_image &yuva_in, const xyuv::format &format) {
    bool dimensions_match = yuva_in.image_w == format.image_w && yuva_in.image_h == format.image_h;

    // Short path.
    if (dimensions_match && yuva_in.siting == format.chroma_siting) {
        return internal_encode_frame(yuva_in, format);
    }

    // Otherwise we will need to do some conversion.
    const yuv_image *image = &yuva_in;
    yuv_image temp;

    if (!dimensions_match) {
        if (!is_444(image->siting.subsampling)) {
            temp = up_sample(*image);
            image = &temp;
        }
        temp = scale_yuv_image(*image, format.image_w, format.image_h);
        image = &temp;
    }

    if (!(image->siting == format.chroma_siting)) {
        if (image->siting.subsampling.macro_px_w > 1 ||
            image->siting.subsampling.macro_px_h > 1) {
            temp = up_sample(*image);
            image = &temp;
        }

        // At this point *image is 444
        if (format.chroma_siting.subsampling.macro_px_w > 1 ||
            format.chroma_siting.subsampling.macro_px_h > 1) {
            temp = down_sample(*image, format.chroma_siting);
            image = &temp;
        }
    }

    return internal_encode_frame(*image, format);
}

} // namespace xyuv