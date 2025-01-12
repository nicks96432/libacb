#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <utility>

#include "acb_env_ns.h"
#include "takamori/CFileSystem.h"
#include "takamori/exceptions/CArgumentException.h"
#include "takamori/exceptions/CInvalidOperationException.h"
#include "takamori/streams/CFileStream.h"

#define FILE_READ    ("rb")
#define FILE_WRITE   ("wb")
#define FILE_APPEND  ("ab")
#define FILE_READ_U  ("rb+")
#define FILE_WRITE_U ("wb+")

ACB_NS_BEGIN

CFileStream::CFileStream(const char *fileName): MyClass(fileName, FileMode::OpenOrCreate) {}

CFileStream::CFileStream(const char *fileName, FileMode mode)
    : MyClass(fileName, mode, FileAccess::ReadWrite) {}

CFileStream::CFileStream(const char *fileName, FileMode mode, FileAccess access)
    : _isReadable(FALSE), _isWritable(FALSE), _isSeekable(FALSE) {
    _mode   = mode;
    _access = access;
    _fp     = OpenFile(fileName);
}

CFileStream::CFileStream(const std::string &fileName): MyClass(fileName.c_str()) {}

CFileStream::CFileStream(const std::string &fileName, FileMode mode)
    : MyClass(fileName.c_str(), mode) {}

CFileStream::CFileStream(const std::string &fileName, FileMode mode, FileAccess access)
    : MyClass(fileName.c_str(), mode, access) {}

CFileStream::~CFileStream() {
    if (_fp) {
        std::fclose(_fp);
    }
    _fp = nullptr;
}

auto CFileStream::Read(void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count)
    -> std::size_t {
    if (!buffer) {
        throw CArgumentException("FileStream::Read()");
    }
    if (!IsReadable()) {
        throw CInvalidOperationException("FileStream::Read()");
    }
    const auto actualCount = std::min(bufferSize - offset, count);
    const auto byteBuffer  = static_cast<std::uint8_t *>(buffer);
    const auto actualRead  = std::fread(byteBuffer + offset, 1, actualCount, _fp);
    return actualRead;
}

auto CFileStream::Write(
    const void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count
) -> std::size_t {
    if (!buffer) {
        throw CArgumentException("FileStream::Write()");
    }
    if (!IsWritable()) {
        throw CInvalidOperationException("FileStream::Write()");
    }
    const auto actualCount = std::min(bufferSize - offset, count);
    const auto byteBuffer  = static_cast<const std::uint8_t *>(buffer);
    std::fwrite(byteBuffer + offset, 1, actualCount, _fp);
    return actualCount;
}

auto CFileStream::IsWritable() const -> bool_t {
    return _isWritable;
}

auto CFileStream::IsReadable() const -> bool_t {
    return _isReadable;
}

auto CFileStream::IsSeekable() const -> bool_t {
    return _isSeekable;
}

auto CFileStream::GetPosition() -> std::uint64_t {
    if (_fp == nullptr) {
        return 0;
    } else {
        return (std::uint64_t)ftell(_fp);
    }
}

void CFileStream::SetPosition(std::uint64_t value) {
    std::fseek(_fp, (long)value, SEEK_SET);
}

auto CFileStream::GetLength() -> std::uint64_t {
    if (_fp == nullptr) {
        return 0;
    }

    auto position = ftell(_fp);
    std::fseek(_fp, 0, SEEK_END);
    auto r = ftell(_fp);
    std::fseek(_fp, position, SEEK_SET);
    return (std::uint64_t)r;
}

void CFileStream::SetLength(std::uint64_t value) {
    std::fseek(_fp, (long)value, SEEK_SET);
}

void CFileStream::Flush() {
    std::fflush(_fp);
}

auto CFileStream::OpenFile(const char *fileName) -> std::FILE * {
#define OUT_OF_RANGE()   throw CException("Mode/Access: out of range")
#define INCOMPATIBLE()   throw CException("Mode/Access: incompatible")
#define FILE_EXISTS()    throw CException("File exists: " + std::string(fileName))
#define FILE_NOT_EXIST() throw CException("File doesn't exist: " + std::string(fileName))
    auto m = _mode;
    auto a = _access;
    // CLion cannot recognize bitwise operators defined. GCC can. However the error prompted by
    // CLion is still annoying.
    _isReadable = static_cast<bool_t>(std::to_underlying(a) & std::to_underlying(FileAccess::Read));
    _isWritable =
        static_cast<bool_t>(std::to_underlying(a) & std::to_underlying(FileAccess::Write));
    _isSeekable = static_cast<bool_t>(m != FileMode::Append);
    if (m == FileMode::None || a == FileAccess::None) {
        OUT_OF_RANGE();
    }
    const char *fileMode = nullptr;
    switch (m) {
    case FileMode::Append:
        switch (a) {
        case FileAccess::Write:
            fileMode = FILE_APPEND;
            break;
        case FileAccess::Read:
        case FileAccess::ReadWrite:
            INCOMPATIBLE();
        default:
            OUT_OF_RANGE();
        }
        break;
    case FileMode::Create:
        switch (a) {
        case FileAccess::Write:
            fileMode = FILE_WRITE_U;
            break;
        case FileAccess::Read:
            CreateFileInternal(fileName);
            fileMode = FILE_READ;
            break;
        case FileAccess::ReadWrite:
            CreateFileInternal(fileName);
            fileMode = FILE_READ_U;
            break;
        default:
            OUT_OF_RANGE();
        }
        break;
    case FileMode::CreateNew:
        if (CFileSystem::FileExists(fileName)) {
            FILE_EXISTS();
        }
        switch (a) {
        case FileAccess::Write:
        case FileAccess::Read:
        case FileAccess::ReadWrite:
            fileMode = FILE_WRITE_U;
            break;
        default:
            OUT_OF_RANGE();
        }
        break;
    case FileMode::OpenExisting:
        if (!CFileSystem::FileExists(fileName)) {
            FILE_NOT_EXIST();
        }
        switch (a) {
        case FileAccess::Write:
            fileMode = FILE_WRITE;
            break;
        case FileAccess::Read:
            fileMode = FILE_READ;
            break;
        case FileAccess::ReadWrite:
            fileMode = FILE_READ_U;
            break;
        default:
            OUT_OF_RANGE();
        }
        break;
    case FileMode::OpenOrCreate:
        if (!CFileSystem::FileExists(fileName)) {
            CreateFileInternal(fileName);
        }
        switch (a) {
        case FileAccess::Write:
            fileMode = FILE_WRITE;
            break;
        case FileAccess::Read:
            fileMode = FILE_READ;
            break;
        case FileAccess::ReadWrite:
            fileMode = FILE_READ_U;
            break;
        default:
            OUT_OF_RANGE();
        }
        break;
    default:
        OUT_OF_RANGE();
    }
    return std::fopen(fileName, fileMode);

#undef OUT_OF_RANGE
#undef INCOMPATIBLE
#undef FILE_EXISTS
#undef FILE_NOT_EXIST
}

void CFileStream::CreateFileInternal(const char *fileName) {
    if (CFileSystem::FileExists(fileName)) {
        return;
    }
    auto fp = std::fopen(fileName, "w");
    std::fclose(fp);
}

ACB_NS_END
