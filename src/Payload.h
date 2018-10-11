/**
 * Common payload.
 */
#ifndef THEAPIPAYLOAD_h
#define THEAPIPAYLOAD_h

namespace theapi {

  class Payload {
    public:
      // Message types
      // ASCII printable characters so serial printing is simple.
      enum MsgType {
        GENERIC = 49,
        GARDEN,
        PING,
        CMD,
        ACK,
        SIGNAL,
        SOLAR,
      };
  };

}

// end namespace
#endif
