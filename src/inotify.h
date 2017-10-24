
#ifndef M2I_INOTIFY_H
#define M2I_INOTIFY_H
#include <poll.h>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace m2i {

struct watcher {
    int wd;
    fs::path path;
    void (*function)();
};

class Notifier {
public:
    Notifier();
    ~Notifier();
    int watchPath( watcher );
    int ignorePath( const fs::path & );
    void check();
    explicit operator bool() { return fd == -1 ? false : true; }
private:
    void handleEvents();
    int fd = -1; // filedescriptor for accessing the inotify API
    std::vector< watcher > watchers;
    pollfd pfd; //file descriptor to pass to poll
};

} //end namespace m2i
#endif//M2I_INOTIFY_H
