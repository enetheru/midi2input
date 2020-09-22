#include <cerrno>
#include <poll.h>
#include <cstdlib>
#include <sys/inotify.h>
#include <unistd.h>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include <spdlog/spdlog.h>

#include "inotify.h"

namespace m2i {

Notifier::Notifier(){
    /* Create the file descriptor for accessing the inotify API */
    fd = inotify_init1( IN_NONBLOCK );
    if( fd == -1 ){
        spdlog::error( "INOTIFY: init1" );
        return;
    }
    /* Prepare for polling */
    pfd = { fd, POLLIN, 0 };
}

Notifier::~Notifier(){
    /* Close inotify file descriptor */
    if( close( fd ) != 0){
        spdlog::error( "INOTIFY: close" );
        return;
    }
}

int
Notifier::watchPath( watcher input ){
    if( input.path.empty() ){
        spdlog::error( "INOTIFY: empty path" );
        return -1;
    }
    auto path = input.path;

    /* Mark directories for events
       - file was opened
       - file was closed */
    //wd = watch descriptor
    input.wd = inotify_add_watch(
        fd, input.path.parent_path().c_str(),
        IN_CLOSE_WRITE | IN_MODIFY
    );
    if( input.wd == -1 ){
        spdlog::error( "INOTIFY: Cannot watch {}", path.c_str() );
        return -1;
    }

    //search the list of existing watchers for an exact match
    for( const auto& watch : watchers ){
        if( watch.wd == input.wd ){
            if( watch.path == input.path ){
                spdlog::warn( "INOTIFY: already watching: {}", path.c_str() );
            }
        }
    }
    //we arent watching the file so add a new entry to the Listing
    spdlog::info( "INOTIFY: adding path: {}", path.parent_path().c_str() );
    watchers.push_back( input );
    return 0;
}

void
Notifier::check(){
    if( watchers.empty() ){
        spdlog::warn("INOTIFY: nothing to check" );
        return;//nothing to check;
    }

    int poll_num;
    poll_num = poll( &pfd, 1, 0 );
    if( poll_num == -1 ){
        if( errno == EINTR )return;
        spdlog::error("INOTIFY: poll" );
        return;
    }
    if( poll_num > 0 ){
        if( pfd.revents & POLLIN ){
            /* Inotify events are available */
            handleEvents();
        }
        /* Note: look in git history for example on how to use keyboard input
         * from stdin, could be used to have keyboard input to midi events.
         */
    }
}

void
Notifier::handleEvents(){
    /* Some systems cannot read integer variables if they are not
       properly aligned. On other systems, incorrect alignment may
       decrease performance. Hence, the buffer used for reading from
       the inotify file descriptor should have the same alignment as
       struct inotify_event. */
    char buf[ 4096 ] __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    ssize_t len;
    char *ptr;

    /* Loop while events can be read from inotify file descriptor. */
    for(;;){
        /* Read some events. */
        len = read( fd, buf, sizeof buf );
        /* If the nonblocking read() found no events to read, then
           it returns -1 with errno set to EAGAIN. In that case,
           we exit the loop. */
        if( len == -1 && errno != EAGAIN ){
            spdlog::error( "INOTIFY: read" );
            return;
        }

        if( len <= 0 )break;

        /* Loop over all events in the buffer */
        for( ptr = buf; ptr < buf + len;
             ptr += sizeof(struct inotify_event) + event->len )
        {
            event = (const struct inotify_event *) ptr;

            //test we are looking at a file we are watching
            for( const auto& watcher : watchers ){
                if( watcher.path.filename() == event->name ){
                    spdlog::info( "INOTIFY: {} has been modified", event->name );
                    // so run the associated function.
                    watcher.function();
                    return;
                }
            }
        }
    }
}

}//end namespace m2i
