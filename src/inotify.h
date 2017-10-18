
#ifndef M2I_INOTIFY_H
#define M2I_INOTIFY_H
#include <unordered_map>
#include <array>
#include <poll.h>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

/* this class is a wrapper around inotify that serves my purposes
 * inotify only monitors directories and provides events for all items, so 
 * to discriminate against files that i want to watch i need a container that
 * holds the list of files.
 * 
 * the watch descriptor is wd, so i need to match up multiple files to each wd
 * or  a '*' for all files.
 */
using watchpair = std::pair< fs::path, void(*)()>;

class Notifier {
public:
    Notifier();
    ~Notifier();
    int watchPath( watchpair );
    int ignorePath( const fs::path & );
    void check();
private:
    void handleEvents();
    int fd = -1; //filedescriptor for accessing the inotify API
    std::unordered_map< int, std::vector< watchpair > > watchers;
    std::array< pollfd, 2 > fds; //array of file descriptors to pass to poll
};

//TODO abstract away this bullshit nested class theirarchy for watchers
/* this seems really icky having a map of vector of pair of path and void (*)();
 * a datastructure or set of datastructures would be nicer but thats a whole
 * nuther class to create and manage. this messy way works for now. */

#endif//M2I_INOTIFY_H
