#include <Magick++/Image.h>
#include <xyuv/imagemagick/magick_wrapper.h>
#include <fstream>
#include <xyuv/frame.h>
#include <xyuv/large_buffer.h>
#include <iostream>
#include "XYUVHeader.h"

std::string get_suffix(const std::string & path);

static xyuv::frame LoadConvertFrame_imagemagick(const xyuv::format & format, const std::string & infile_name ) {
    Magick::Image image(infile_name);
    xyuv::magick_wrapper wrapper(image);
    return xyuv::read_frame_from_rgb_image(wrapper, format);
}

static xyuv::frame LoadConvertFrame_xyuv(const xyuv::format & format, const std::string & infile_name ) {
    std::ifstream fin(infile_name, std::ios::binary);

    if (!fin) {
        throw std::runtime_error("Could not open input file: '" + infile_name + "'");
    }

    xyuv::frame frame;
    xyuv::read_frame(&frame, fin);
    return std::move(frame);
}

static xyuv::frame LoadConvertFrame_raw(const xyuv::format & format, const std::string & infile_name ) {
    std::ifstream fin(infile_name, std::ios::binary);

    if (!fin) {
        throw std::runtime_error("Could not open input file: '" + infile_name + "'");
    }

    auto frame = xyuv::create_frame(format, nullptr, 0);

    uint64_t bytes_read = xyuv::read_large_buffer(fin, reinterpret_cast<char*>(frame.data.get()), format.size);

    if (bytes_read != format.size) {
        throw std::runtime_error("Error loading frame data, only " + std::to_string(bytes_read) + " bytes read, expected " + std::to_string(format.size));
    }

    return frame;
}


xyuv::frame XYUVHeader::LoadConvertFrame( const xyuv::format & format, const std::string & infile_name ) {
    static std::map<std::string, std::function<xyuv::frame(const xyuv::format &, const std::string &)>> map{
            {".xyuv", LoadConvertFrame_xyuv},
            {".bin", LoadConvertFrame_raw},
            {".raw", LoadConvertFrame_raw},
            {".yuv", LoadConvertFrame_raw},
            {".png", LoadConvertFrame_imagemagick},
            {".jpg", LoadConvertFrame_imagemagick},
            {".gif", LoadConvertFrame_imagemagick},
            {".tga", LoadConvertFrame_imagemagick},
            {".bmp", LoadConvertFrame_imagemagick},
    };

    std::string suffix = get_suffix(infile_name);

    // Make it lowercase.
    for (char & c : suffix ) {
        c = static_cast<char>(std::tolower(c));
    }

    // If the suffix is in out map, use it
    auto it = map.find(suffix);
    if (it != map.end()) {
        return (it->second)(format, infile_name);
    }
    else {
        std::cout << "[Warning]: Unrecognized file suffix '" + suffix + "' for input file '" + infile_name + "' trying to pass it to imagemagick." << std::endl;
        return LoadConvertFrame_imagemagick(format, infile_name);
    }
}