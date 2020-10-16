
#ifndef M2I_INOTIFY_H
#define M2I_INOTIFY_H
#include <poll.h>

#include <filesystem>

namespace m2i {

struct watcher {
    int wd;
    std::filesystem::path path;
    void (*function)();
};

class Notifier {
public:
    Notifier();
    ~Notifier();
    int watchPath( watcher );
    void check();
    explicit operator bool() const { return fd != -1; }
private:
    void handleEvents();
    int fd = -1; // file descriptor for accessing the inotify API
    std::vector< watcher > watchers;
    pollfd pfd{}; //file descriptor to pass to poll
};

} //end namespace m2i
#endif//M2I_INOTIFY_H
