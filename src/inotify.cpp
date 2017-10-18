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



Notifier::Notifier()
{
    /* Create the file descriptor for accessing the inotify API */
    fd = inotify_init1( IN_NONBLOCK );
    if( fd == -1 ){
        LOG( m2i::ERROR ) << "inotify_init1\n";
        return;
    }

    /* Prepare for polling */
    /* Console input */
    fds[0] = { STDIN_FILENO, POLLIN, 0 };
    /* Inotify input */
    fds[1] = { fd, POLLIN, 0 };
}
Notifier::~Notifier(){
    /* Close inotify file descriptor */
    if( close( fd ) != 0){
        LOG( m2i::ERROR ) << "close\n";
        return;
    }
    LOG( m2i::INFO ) << "Listening for events stopped.\n";
    return;
}

int
Notifier::watchPath( watchpair input ){
    if( fd == -1 )return -1;// inotify_init has failed;

    if( input.first.empty() ){
        LOG( m2i::ERROR ) << "empty path\n";
        return -1;
    }
    auto path = input.first;

    /* Mark directories for events
       - file was opened
       - file was closed */
    //wd = watch descriptor
    auto wd = inotify_add_watch(
        fd, path.parent_path().c_str(),
        IN_CLOSE_WRITE | IN_MODIFY
    );
    if( wd == -1 ){
        LOG( m2i::ERROR ) << "inotify_add_watch - Cannot watch "<<path<<"\n";
        return -1;
    }
    auto watch = watchers.find( wd );
    if( watch != watchers.end() ){
        /* we are already watching the path so look for the filename entry and
         * add it if it doesnt exist already. */
        bool found = false;
        for( auto wpair : watch->second ){
            if( wpair.first.filename() == path.filename() ) found = true;
        }
        if( found ) LOG( m2i::WARN ) << "already watching: " << path << "\n";
        else watch->second.push_back( input );
        return 1;
    } else {
        //we arent watching the file so add a new entry to the Listing
        LOG( m2i::INFO ) << "adding inotify path: " << path.parent_path() << "\n";
        std::vector< watchpair > newvector{ input };
        watchers.insert( { wd, newvector } );
    }
    return 0;
}

int
Notifier::ignorePath( const fs::path &path ){
    if( fd == -1 )return -1;// inotify_init has failed;
    if( path.empty() ){
        LOG( m2i::ERROR ) << "empty path\n";
        return -1;
    }

    // FIXME remove inotify watcher
    /*if( inotify_rm_watch( fd, wd ) != 0){
        LOG( m2i::ERROR ) << "inotify_rm_watch\n";
        return -1;
    } */

    return 0;
}

void
Notifier::check(){
    if( fd == -1 ){
        LOG( m2i::ERROR ) << "inotify not initialised\n";
        return;// inotify_init has failed;
    }
    if( watchers.empty() ){
        LOG( m2i::WARN ) << "nothing to check\n";
        return;//nothing to check;
    }

    int poll_num;
    poll_num = poll( fds.data(), fds.size(), 0 );
    if( poll_num == -1 ){
        if( errno == EINTR )return;
        LOG( m2i::ERROR ) << "poll";
        return;
    }
    if( poll_num > 0 ){
        //FIXME this is broke because it prevents the console from handling ctrl+c to quit
        //if( fds[ 0 ].revents & POLLIN ){
        //    /* Console input is available. Empty stdin and quit */
        //    char buf;
        //    while( read( STDIN_FILENO, &buf, 1 ) > 0 ){
        //        if( buf != '\n' ) LOG( m2i::INFO ) << "enter is pressed\n";
        //        // TODO possible location to add console key commands to
        //        // create midi events
        //    }
        //}
        if( fds[ 1 ].revents & POLLIN ){
            /* Inotify events are available */
            handleEvents();
        }
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
        if( len == -1 && errno != EAGAIN ){
            LOG( m2i::ERROR ) << "read\n";
            return;
        }

        /* If the nonblocking read() found no events to read, then
           it returns -1 with errno set to EAGAIN. In that case,
           we exit the loop. */
        if( len <= 0 )break;

        /* Loop over all events in the buffer */
        for( ptr = buf;
             ptr < buf + len;
             ptr += sizeof(struct inotify_event) + event->len )
        {
            event = (const struct inotify_event *) ptr;

            //test we are looking at a file we are watching
            for( auto wpair : watchers[ event->wd ] ){
                if( wpair.first.filename() == event->name ){
                    LOG( m2i::INFO ) << event->name << " has been modified\n";
                    wpair.second();
                    return;
                } 
            }
        }
    }
}
