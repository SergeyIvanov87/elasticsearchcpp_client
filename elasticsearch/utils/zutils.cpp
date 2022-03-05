#include <string.h>
 #include <unistd.h>

#include <stdexcept>
#include "elasticsearch/utils/zutils.hpp"

namespace  elasticsearch
{
namespace utils
{
unique_gzFile create_gzdfile(fd_guard &&fd, const char *mode)
{
    if (!mode)
    {
        throw std::runtime_error("mode is nullptr");
    }

    unique_gzFile compressed_file (gzdopen(fd, mode), gzclose);
    if (!compressed_file)
    {
        throw std::runtime_error("cannot open gzFile by fd: " + std::to_string(fd) + ", error: " +
                                 strerror(errno));
    }
    fd.release();
    return compressed_file;
}

fd_guard::fd_guard(int fd) :
    m_fd(fd)
{
}

fd_guard::fd_guard(fd_guard &&src) :
    m_fd(std::move(src.m_fd))
{
    src.m_fd = INVALID;
}

fd_guard& fd_guard::operator= (fd_guard&& src)
{
    if (&src == this)
    {
        return *this;
    }
    m_fd = src.m_fd;
    src.m_fd = INVALID;
    return *this;
}

fd_guard::~fd_guard()
{
    if(m_fd != INVALID)
    {
        close(m_fd);
    }
}

fd_guard::operator int() const
{
    return m_fd;
}

fd_guard fd_guard::dup()
{
    int new_fd = ::dup(m_fd);
    if (new_fd == -1)
    {
        throw std::runtime_error("cannot dup fd: " + std::to_string(m_fd) + ", error: " +
                                 strerror(errno));
    }
    return fd_guard(new_fd);
}

int fd_guard::release()
{
    int ret_fd = m_fd;
    m_fd = INVALID;
    return ret_fd;
}

fd_guard make_tmpfd()
{
    char tmp_name[7] {"XXXXXX"};
    int fd = mkstemp(tmp_name);
    if (fd == -1)
    {
        throw std::runtime_error("cannot create temporary file, error: " +
                                 std::to_string(errno) + " (" + strerror(errno) + ")");
    }
    unlink(tmp_name); // remove last link to the file: it is still open by fd, but not exist in the fs
    return fd_guard(fd);
}
}
}
