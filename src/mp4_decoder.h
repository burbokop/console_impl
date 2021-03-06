#ifndef MP4_DECODER_H
#define MP4_DECODER_H

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
#include <libavutil/imgutils.h>
#ifdef __cplusplus
}
#endif

#include <ostream>
#include <vector>
#include "pixelprimitives.h"
#include <cstdint>

class mp4_decoder {
    const char *m_path;
    std::ostream &m_log;
    double m_scale;

    AVFormatContext *m_formatContext = nullptr;
    AVPacket *m_packet = nullptr;
    AVFrame *m_frame = nullptr;
    AVCodecContext *m_codecContext = nullptr;
    SwsContext* m_imageConvertContext = nullptr;
    int m_video_stream_index;
    mutable std::vector<pixel_primitives::bitmap> m_cache;
    std::size_t m_destinationWidth;
    std::size_t m_destinationHeight;
    AVPixelFormat m_destinationFormat;
public:
    mp4_decoder(const char *path, std::ostream &log, double scale);

    pixel_primitives::bitmap &frame(std::size_t index) const;

    std::size_t frame_count() const;

    static std::uint8_t canal8(AVFrame* frame, std::size_t canal_no, std::size_t x, std::size_t y, std::size_t width);
    static std::uint32_t canal32(AVFrame* frame, std::size_t canal_no, std::size_t x, std::size_t y, std::size_t width);

    static int decode_packet(
            std::vector<pixel_primitives::bitmap>& dst_btmps,
            AVPacket *pPacket,
            AVCodecContext *pCodecContext,
            SwsContext* imageConvertContext,
            AVFrame *pFrame,
            std::ostream &log,
            std::size_t destinationWidth,
            std::size_t destinationHeight,
            AVPixelFormat destinationFormat
            );

    ~mp4_decoder();
};

#endif // MP4_DECODER_H
