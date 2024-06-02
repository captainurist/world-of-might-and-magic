#pragma once

#include <string>

#include "Utility/Memory/Blob.h"

#include "MediaPacket.h"
#include "MediaInfo.h"

struct AVIOContext;
struct AVFormatContext;

class MediaStream {
 public:
    MediaStream() = default;
    ~MediaStream();

    void open(const std::string &path);
    void open(Blob data);
    void close();

    [[nodiscard]] bool isOpen() const {
        return !!_blob;
    }

    const MediaInfo &info() const;

    MediaPacket read();

 private:
    Blob _blob;
    AVFormatContext *_formatCtx = nullptr;
    void *_ioBuffer = nullptr;
    AVIOContext *_ioCtx = nullptr;
};
