#include <errno.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include <string>
#include <vector>
#include <map>
#include "util.h"
#include "inotify.h"

namespace m2i {

Notifier::Notifier(){
    /* Create the file descriptor for accessing the inotify API */
    fd = inotify_init1( IN_NONBLOCK );
    if( fd == -1 ){
        LOG( m2i::ERROR ) << "inotify_init1\n";
        return;
    }
    /* Prepare for polling */
    pfd = { fd, POLLIN, 0 };
}

Notifier::~Notifier(){
    /* Close inotify file descriptor */
    if( close( fd ) != 0){
        LOG( m2i::ERROR ) << "close\n";
        return;
    }
    return;
}

int
Notifier::watchPath( watcher input ){
    if( input.path.empty() ){
        LOG( m2i::ERROR ) << "empty path\n";
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
        LOG( m2i::ERROR ) << "inotify_add_watch - Cannot watch " << path << "\n";
        return -1;
    }

    //search the list of existing watchers for an exact match
    for( auto watch : watchers ){
        if( watch.wd == input.wd ){
            if( watch.path == input.path ){
                LOG( m2i::WARN ) << "already watching: " << path << "\n";
            }
        }
    }
    //we arent watching the file so add a new entry to the Listing
    LOG( m2i::INFO ) << "adding inotify path: " << path.parent_path() << "\n";
    watchers.push_back( input );
    return 0;
}

int
Notifier::ignorePath( const fs::path &path ){
    if( path.empty() ){
        LOG( m2i::ERROR ) << "empty path\n";
        return -1;
    }

    for( auto iter = watchers.begin(); iter != watchers.end(); iter++ ){
        if( iter->path == path ){
            if( inotify_rm_watch( fd, iter->wd ) != 0 ){
                LOG( m2i::ERROR ) << "inotify_rm_watch\n";
                return -1;
            }
            watchers.erase( iter );
            return 0;
        }
    }
    LOG( m2i::WARN ) << "inotify is not watching: " << path << "\n";
    return 0;
}

void
Notifier::check(){
    if( watchers.empty() ){
        LOG( m2i::WARN ) << "nothing to check\n";
        return;//nothing to check;
    }

    int poll_num;
    poll_num = poll( &pfd, 1, 0 );
    if( poll_num == -1 ){
        if( errno == EINTR )return;
        LOG( m2i::ERROR ) << "poll";
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

    return;
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
            LOG( m2i::ERROR ) << "read\n";
            return;
        }

        if( len <= 0 )break;

        /* Loop over all events in the buffer */
        for( ptr = buf; ptr < buf + len;
             ptr += sizeof(struct inotify_event) + event->len )
        {
            event = (const struct inotify_event *) ptr;

            //test we are looking at a file we are watching
            for( auto watcher : watchers ){
                if( watcher.path.filename() == event->name ){
                    LOG( m2i::INFO ) << event->name << " has been modified\n";
                    // so run the associated function.
                    watcher.function();
                    return;
                }
            }
        }
    }
}

}//end namespace m2i
