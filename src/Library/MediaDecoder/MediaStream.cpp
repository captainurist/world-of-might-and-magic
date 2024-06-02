#include "MediaStream.h"

#include <cassert>

extern "C" {
#include <libavformat/avformat.h> // NOLINT: not a C system header.
}

#include "Utility/Exception.h"

static constexpr size_t AVIO_BUFFER_SIZE = 32 * 1024;

MediaStream::~MediaStream() {

}

void MediaStream::open(const std::string &path) {
    // We are being lazy here and route everything through our custom IO over memory-mapped files.
    open(Blob::fromFile(path));
}

void MediaStream::open(Blob data) {
    close();

    _blob = std::move(data);

    // Some notes on error handling.

    _formatCtx = avformat_alloc_context();
    assert(_formatCtx); // We don't handle out of memory errors.

    _ioBuffer = av_malloc(AVIO_BUFFER_SIZE);
    assert(_ioBuffer)
    if (!_ioBuffer) {
        Close();
        return false;
    }

    avio_ctx = avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size, 0,
                                  this, &read_packet, nullptr, &seek);
    if (!avio_ctx) {
        Close();
        return false;
    }



    AVFormatContext *pFormatContext;


}

void MediaStream::close() {

}

MediaInfo MediaStream::info() const {
    assert(isOpen())
}

MediaPacket MediaStream::read() {

}
