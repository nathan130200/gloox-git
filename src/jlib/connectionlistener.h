/*
 * (c) 2004-2005 by Jakob Schroeter <js@camaya.net>
 */



#ifndef CONNECTIONLISTENER_H__
#define CONNECTIONLISTENER_H__


#include <iksemel.h>

/**
 * A virtual interface.
 * Derived classes can be registered as ConnectionListeners with the JClient
 * @author Jakob Schroeter <js@camaya.net>
 */
class ConnectionListener
{
  public:
    /**
     * Reimplement this function if you want to be notified about
     * successful connections. It will be called after all authentication
     * is finished. Depending on the setting of AutoPresence, a presence
     * stanza is sent or not.
     */
    virtual void onConnect() {};

    /**
     * Reimplement this function if you want to be notified about
     * disconnections.
     */
    virtual void onDisconnect() {};

};

#endif // CONNECTIONLISTENER_H__
