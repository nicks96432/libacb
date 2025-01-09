#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "cgss_env_ns.h"
#include "takamori/CFileSystem.h"
#include "takamori/exceptions/CArgumentException.h"
#include "takamori/exceptions/CInvalidOperationException.h"
#include "takamori/streams/CFileStream.h"

#define __FILE_READ    ("rb")
#define __FILE_WRITE   ("wb")
#define __FILE_APPEND  ("ab")
#define __FILE_READ_U  ("rb+")
#define __FILE_WRITE_U ("wb+")

CGSS_NS_BEGIN

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
#define __OUT() throw CException("Mode/Access: out of range")
#define __CMB() throw CException("Mode/Access: incompatible")
#define __EXT() throw CException("File exists: " + std::string(fileName))
#define __NEX() throw CException("File doesn't exist: " + std::string(fileName))
    auto m = _mode;
    auto a = _access;
    // CLion cannot recognize bitwise operators defined. GCC can. However the error prompted by
    // CLion is still annoying.
    _isReadable = (bool_t)(static_cast<std::underlying_type_t<FileAccess>>(a) &
                           static_cast<std::underlying_type_t<FileMode>>(FileAccess::Read));
    _isWritable = (bool_t)(static_cast<std::underlying_type_t<FileAccess>>(a) &
                           static_cast<std::underlying_type_t<FileMode>>(FileAccess::Write));
    _isSeekable = (bool_t)(m != FileMode::Append);
    if (m == FileMode::None || a == FileAccess::None) {
        __OUT();
    }
    const char *fileMode = nullptr;
    switch (m) {
    case FileMode::Append:
        switch (a) {
        case FileAccess::Write:
            fileMode = __FILE_APPEND;
            break;
        case FileAccess::Read:
        case FileAccess::ReadWrite:
            __CMB();
        default:
            __OUT();
        }
        break;
    case FileMode::Create:
        switch (a) {
        case FileAccess::Write:
            fileMode = __FILE_WRITE_U;
            break;
        case FileAccess::Read:
            CreateFileInternal(fileName);
            fileMode = __FILE_READ;
            break;
        case FileAccess::ReadWrite:
            CreateFileInternal(fileName);
            fileMode = __FILE_READ_U;
            break;
        default:
            __OUT();
        }
        break;
    case FileMode::CreateNew:
        if (CFileSystem::FileExists(fileName)) {
            __EXT();
        }
        switch (a) {
        case FileAccess::Write:
        case FileAccess::Read:
        case FileAccess::ReadWrite:
            fileMode = __FILE_WRITE_U;
            break;
        default:
            __OUT();
        }
        break;
    case FileMode::OpenExisting:
        if (!CFileSystem::FileExists(fileName)) {
            __NEX();
        }
        switch (a) {
        case FileAccess::Write:
            fileMode = __FILE_WRITE;
            break;
        case FileAccess::Read:
            fileMode = __FILE_READ;
            break;
        case FileAccess::ReadWrite:
            fileMode = __FILE_READ_U;
            break;
        default:
            __OUT();
        }
        break;
    case FileMode::OpenOrCreate:
        if (!CFileSystem::FileExists(fileName)) {
            CreateFileInternal(fileName);
        }
        switch (a) {
        case FileAccess::Write:
            fileMode = __FILE_WRITE;
            break;
        case FileAccess::Read:
            fileMode = __FILE_READ;
            break;
        case FileAccess::ReadWrite:
            fileMode = __FILE_READ_U;
            break;
        default:
            __OUT();
        }
        break;
    default:
        __OUT();
    }
    return std::fopen(fileName, fileMode);

#undef __OUT
#undef __CMB
#undef __EXT
#undef __NEX
}

void CFileStream::CreateFileInternal(const char *fileName) {
    if (CFileSystem::FileExists(fileName)) {
        return;
    }
    auto fp = std::fopen(fileName, "w");
    std::fclose(fp);
}

CGSS_NS_END
