#ifndef MIDI2INPUT_ALSA_H
#define MIDI2INPUT_ALSA_H

//header to initialise jack interface and keep it clean
#include <alsa/asoundlib.h>
#include "midi.h"
#include "util.h"

namespace snd {

/* If i take the opinion that declaring an object doesnt initialise it, and that
 * explicit initialisation is required, which is the case for all the standard
 * types, then the constructor doesnt do anything interesting. but the
 * destructor will cleanup. This means that the Seq object can be allocated, but
 * left in an uninitialised state, at which point taking action on it would be
 * undefined, the same as normal types.
 */
class Seq {
public:
    Seq() = default;
    int open();
    void close();

    int connect( const std::string &client_name, const std::string &port_name );

    midi_event event_receive();
    int event_pending();
    void event_send( const midi_event &event );

    explicit operator bool() const { return seq; }

    ~Seq();
private:
    int iport_id = -1;
    int oport_id = -1;
    int client_id = -1;
    snd_seq_t *seq = nullptr;
};

}// end namespace snd

#endif // MIDI2INPUT_JACK_H
