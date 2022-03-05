#ifndef ELASTICKSEARCH_UTILS_ZUTILS_HPP
#define ELASTICKSEARCH_UTILS_ZUTILS_HPP

#include <zlib.h>

#include <memory>

namespace  elasticsearch
{
namespace utils
{
class fd_guard
{
public:
    enum { INVALID = -1};
    explicit fd_guard(int fd);
    fd_guard(fd_guard &&);
    fd_guard& operator= (fd_guard&& );
    ~fd_guard();

    operator int() const;
    fd_guard dup();
    int release();
private:
    int m_fd;
;
    fd_guard(const fd_guard &) = delete;
    fd_guard& operator= (const fd_guard& ) = delete;
};

fd_guard make_tmpfd();

using unique_gzFile = std::unique_ptr<gzFile_s, decltype(&gzclose)>;
unique_gzFile create_gzdfile(fd_guard &&fd, const char *mode);

}
}
#endif // ELASTICKSEARCH_UTILS_ZUTILS_HPP
