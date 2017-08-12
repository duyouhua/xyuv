/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Stian Valentin Svedenborg
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

#include <gtest/gtest.h>
#include <xyuv.h>
#include <xyuv/structures/format.h>
#include <xyuv/frame.h>
#include <xyuv/yuv_image.h>
#include "TestResources.h"
#include "../xyuv/src/utility.h"
#include "../xyuv/src/to_string.h"
#include "../xyuv/src/block_reorder.h"

static xyuv::format create_block_reordered_format() {
    xyuv::format format;

    format.chroma_siting = Resources::get().config().get_chroma_siting("444");
    format.conversion_matrix = Resources::get().config().get_conversion_matrix("identity");

    format.image_w = 2;
    format.image_h = 2;

    xyuv::plane plane;

    plane.block_order.mega_block_width = 2;
    plane.block_order.mega_block_height = 2;

    plane.block_order.x_mask[0] = xyuv::block_order::NOT_USED;
    plane.block_order.x_mask[1] = 0;

    plane.block_order.y_mask[0] = 0;
    plane.block_order.y_mask[1] = xyuv::block_order::NOT_USED;


    plane.base_offset = 0;
    plane.interleave_mode = xyuv::interleave_pattern::NO_INTERLEAVING;
    plane.block_stride = 8;
    plane.line_stride = xyuv::next_multiple(format.image_w, plane.block_order.mega_block_width);

    format.size = plane.line_stride*format.image_h;
    plane.size = format.size;


    format.planes.push_back(plane);

    xyuv::channel_block & block = format.channel_blocks[xyuv::channel::Y];
    block.w = 1;
    block.h = 1;

    xyuv::sample s0;
    s0.integer_bits = 8;
    s0.fractional_bits = 0;
    s0.offset = 0;
    s0.plane = 0;
    s0.has_continuation = false;

    block.samples.push_back(s0);


    return format;
}


TEST(BlockReorder, BlockOffset) {
    xyuv::block_order bo;

    bo.mega_block_height = 0xffffu +1;
    bo.mega_block_width = 0xffffu +1;

    for (uint8_t i = 0; i < 32; i++) {
        bo.x_mask[i] = i;
        bo.y_mask[i] = i;
    }

    ASSERT_EQ(std::make_pair(0u,0u), xyuv::get_block_order_coords(0xffffffffu, 0xffffffffu, bo));

    for (uint8_t i = 0; i < 32; i++) {
        bo.x_mask[i] = (i & 1) ? xyuv::block_order::NOT_USED : i / 2u;
        bo.y_mask[i] = (i & 1) ? i / 2u : xyuv::block_order::NOT_USED ;
    }

    ASSERT_EQ(std::make_pair(0xffffu, 0xffffu), xyuv::get_block_order_coords(0xffffffffu, 0xffffffffu, bo));
    ASSERT_EQ(std::make_pair(0xffffu, 0xffffu), xyuv::get_block_order_coords(    0xffffu,     0xffffu, bo));
    ASSERT_EQ(std::make_pair(0xffffu, 0x0u   ), xyuv::get_block_order_coords(      0xffu,       0xffu, bo));
    ASSERT_EQ(std::make_pair(  0xffu, 0x0u   ), xyuv::get_block_order_coords(       0xfu,        0xfu, bo));
    ASSERT_EQ(std::make_pair(  0xfeu, 0x0u   ), xyuv::get_block_order_coords(       0xeu,        0xfu, bo));
    ASSERT_EQ(std::make_pair(  0xeeu, 0x0u   ), xyuv::get_block_order_coords(0xeu - 0x4u,        0xfu, bo));
}

TEST(BlockReorder, ReorderTest) {
    xyuv::frame frame = xyuv::create_frame(create_block_reordered_format(), nullptr, 0);

    frame.data[0] = 0;
    frame.data[1] = 2;
    frame.data[2] = 1;
    frame.data[3] = 3;


    xyuv::yuv_image image = decode_frame(frame);

    // Check that lines are loaded correctly.
    auto fmt1 = frame.format;
    fmt1.planes[0].block_order.mega_block_width = 1;
    fmt1.planes[0].block_order.mega_block_height = 1;

    xyuv::frame linear_frame = encode_frame(image, fmt1);
    for (std::size_t i = 0; i < frame.format.size; i++) {
        SCOPED_TRACE("First " + xyuv::to_string(i));
        ASSERT_EQ(static_cast<uint8_t>(i), linear_frame.data[i]);
    }

    // Check that operation is invertible
    xyuv::frame repacked_frame = encode_frame(image, frame.format);
    for (std::size_t i = 0; i < frame.format.size; i++) {
        SCOPED_TRACE("First " + xyuv::to_string(i));
        ASSERT_EQ(static_cast<uint8_t>(frame.data[i]), repacked_frame.data[i]);
    }


}