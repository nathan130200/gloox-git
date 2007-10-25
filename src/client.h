/*
  Copyright (c) 2004-2007 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef CLIENT_H__
#define CLIENT_H__

#include "clientbase.h"
#include "iqhandler.h"
#include "presence.h"

#include <string>

namespace gloox
{

  class Capabilities;
  class RosterManager;
  class NonSaslAuth;
  class IQ;

  /**
   * @brief This class implements a basic Jabber Client.
   *
   * It supports @ref sasl_auth as well as TLS (Encryption), which can be
   * switched on/off separately. They are used automatically if the server supports them.
   *
   * To use, create a new Client instance and feed it connection credentials, either in the Constructor or
   * afterwards using the setters. You should then register packet handlers implementing the corresponding
   * Interfaces (ConnectionListener, PresenceHandler, MessageHandler, IqHandler, SubscriptionHandler),
   * and call @ref connect() to establish the connection to the server.<br>
   *
   * @note While the MessageHandler interface is still available (and will be in future versions)
   * it is now recommended to use the new @link gloox::MessageSession MessageSession @endlink for any
   * serious messaging.
   *
   * Simple usage example:
   * @code
   * using namespace gloox;
   *
   * void TestProg::doIt()
   * {
   *   Client* j = new Client( "user@server/resource", "password" );
   *   j->registerPresenceHandler( this );
   *   j->disco()->setVersion( "TestProg", "1.0" );
   *   j->disco()->setIdentity( "client", "bot" );
   *   j->connect();
   * }
   *
   * virtual void TestProg::presenceHandler( Presence* presence )
   * {
   *   // handle incoming presence packets here
   * }
   * @endcode
   *
   * However, you can skip the presence handling stuff if you make use of the RosterManager.
   *
   * By default, the library handles a few (incoming) IQ namespaces on the application's behalf. These
   * include:
   * @li jabber:iq:roster: by default the server-side roster is fetched and handled. Use
   * @ref rosterManager() and @ref RosterManager to interact with the Roster.
   * @li XEP-0092 (Software Version): If no version is specified, a name of "based on gloox" with
   * gloox's current version is announced.
   * @li XEP-0030 (Service Discovery): All supported/available services are announced. No items are
   * returned.
   * @note As of gloox 0.9, by default a priority of 0 is sent along with the initial presence.
   * @note As of gloox 0.9, initial presence is automatically sent. Presence: available, Priority: 0.
   * To disable sending of initial Presence use setPresence() with a value of Unavailable
   * prior to connecting.
   *
   * @section sasl_auth SASL Authentication
   *
   * Besides the simple, IQ-based authentication (XEP-0078), gloox supports several SASL (Simple
   * Authentication and Security Layer, RFC 2222) authentication mechanisms.
   * @li DIGEST-MD5: This mechanism is preferred over all other mechanisms if username and password are
   * provided to the Client instance. It is secure even without TLS encryption.
   * @li PLAIN: This mechanism is used if DIGEST-MD5 is not available. It is @b not secure without
   * encryption.
   * @li ANONYMOUS This mechanism is used if neither username nor password are set. The server generates
   * random, temporary username and resource and may restrict available services.
   * @li EXTERNAL This mechanism is currently only available if client certificate and private key
   * are provided. The server tries to figure out who the client is by external means -- for instance,
   * using the provided certificate or even the IP address. (The restriction to certificate/key
   * availability is likely to be lifted in the future.)
   *
   * Of course, all these mechanisms are not tried unless the server offers them.
   *
   * @author Jakob Schroeter <js@camaya.net>
   */
  class GLOOX_API Client : public ClientBase, public IqHandler
  {
    public:

      friend class NonSaslAuth;
      friend class Parser;

      /**
       * Constructs a new Client which can be used for account registration only.
       * SASL and TLS are on by default. The port will be determined by looking up SRV records.
       * Alternatively, you can set the port explicitly by calling @ref setPort().
       * @param server The server to connect to.
       */
      Client( const std::string& server );

      /**
       * Constructs a new Client.
       * SASL and TLS are on by default. This should be the default constructor for most use cases.
       * The server address will be taken from the JID. The actual host will be resolved using SRV
       * records. The domain part of the JID is used as a fallback in case no SRV record is found, or
       * you can set the server address separately by calling @ref setServer().
       * @param jid A full Jabber ID used for connecting to the server.
       * @param password The password used for authentication.
       * @param port The port to connect to. The default of -1 means to look up the port via DNS SRV.
       */
      Client( const JID& jid, const std::string& password, int port = -1 );

      /**
       * Constructs a new Client.
       * SASL and TLS are on by default.
       * The actual host will be resolved using SRV records. The server value is used as a fallback
       * in case no SRV record is found.
       * @param username The username/local part of the JID.
       * @param resource The resource part of the JID.
       * @param password The password to use for authentication.
       * @param server The Jabber ID's server part and the host name to connect to. If those are different
       * for your setup, use the second constructor instead.
       * @param port The port to connect to. The default of -1 means to look up the port via DNS SRV.
       */
      Client( const std::string& username, const std::string& password,
              const std::string& server, const std::string& resource, int port = -1 );

      /**
       * Virtual destructor.
       */
      virtual ~Client();

      /**
       * Use this function to @b re-try to bind a resource only in case you were notified about an
       * error by means of ConnectionListener::onResourceBindError().
       * You may (or should) use setResource() before.
       */
      void bindResource();

      /**
       * Returns the current prepped resource.
       * @return The resource used to connect.
       */
      const std::string& resource() const { return m_jid.resource(); }

      /**
       * Returns the current priority.
       * @return The priority of the current resource.
       */
      int priority() const { return m_priority; }

      /**
       * Sets the username to use to connect to the XMPP server.
       * @param username The username to authenticate with.
       */
      void setUsername( const std::string &username );

      /**
       * Sets the resource to use to connect to the XMPP server.
       * @param resource The resource to use to log into the server.
       */
      void setResource( const std::string &resource ) { m_jid.setResource( resource ); }

      /**
       * Sends directed presence to the given JID. This is a NOOP if there's no active connection.
       * To broadcast presence use setPresence( Presence::PresenceType, int, const std::string& ).
       * @param to The JID to send directed Presence to.
       * @param presence The presence to send.
       * @param priority The priority to include. Legal values: -128 <= priority <= 127
       * @param status The optional status message to include.
       * @note This function does not include any presence extensions (as added by
       * means of addPresenceExtension()) to the stanza.
       */
      void setPresence( const JID& to, Presence::PresenceType pres, int priority,
                        const std::string& status = EmptyString );

      /**
       * Use this function to set the entity's presence, that is, to broadcast presence to all
       * subscribed entities. To send directed presence, use
       * setPresence( const JID&, Presence::PresenceType, int, const std::string& ).
       * If used prior to establishing a connection, the set values will be sent with
       * the initial presence stanza.
       * If used while a connection already is established a repective presence stanza will be
       * sent out immediately.
       * @param presence The Presence value to set.
       * @param priority An optional priority value. Legal values: -128 <= priority <= 127
       * @param status An optional message describing the presence state.
       * @since 0.9
       */
      void setPresence( Presence::PresenceType pres, int priority,
                        const std::string& status = EmptyString );

      /**
       * Use this function to broadcast the entity's presence to all
       * subscribed entities. This is a NOOP if there's no active connection.
       * To send directed presence, use
       * setPresence( const JID&, Presence::PresenceType, int, const std::string& ).
       * If used while a connection already is established a repective presence stanza will be
       * sent out immediately. Use presence() to modify the Presence object.
       * @note When login is finished, initial presence will be sent automatically.
       * So you do not need to call this function after login.
       * @since 1.0
       */
      void setPresence() { sendPresence( m_presence ); }

      /**
       * Returns the current presence.
       * @return The current presence.
       */
      Presence& presence() { return m_presence; }

      /**
       * This is a temporary hack to enforce Non-SASL login. You should not need to use it.
       * @param force Whether to force non-SASL auth. Default @b true.
       * @deprecated Please update the server to properly support SASL instead.
       */
      GLOOX_DEPRECATED void setForceNonSasl( bool force = true ) { m_forceNonSasl = force; }

      /**
       * Disables the automatic roster management.
       * You have to keep track of incoming presence yourself if
       * you want to have a roster.
       */
      void disableRoster();

      /**
       * This function gives access to the @c RosterManager object.
       * @return A pointer to the RosterManager.
       */
      RosterManager* rosterManager() { return m_rosterManager; }

      /**
       * Disconnects from the server.
       */
      void disconnect();

      // re-implemented from IaHandler
      virtual bool handleIq( IQ* iq ) { (void)iq; return false; }

      // re-implemented from IaHandler
      virtual void handleIqID( IQ* iq, int context );

    protected:
      /**
       * Initiates non-SASL login.
       */
      void nonSaslLogin();

    private:
      virtual void handleStartNode() {}
      virtual bool handleNormalNode( Tag* tag );
      virtual void disconnect( ConnectionError reason );
      int getStreamFeatures( Tag* tag );
      int getSaslMechs( Tag* tag );
      int getCompressionMethods( Tag* tag );
      void processResourceBind( IQ* iq );
      void processCreateSession( IQ* iq );
      void sendPresence( const Presence& pres );
      void createSession();
      void negotiateCompression( StreamFeature method );
      void connected();
      virtual void rosterFilled();
      virtual void cleanup();

      void init();

      enum TrackContext
      {
        ResourceBind,
        SessionEstablishment
      };

      RosterManager* m_rosterManager;
      NonSaslAuth* m_auth;

      Presence m_presence;

      bool m_resourceBound;
      bool m_forceNonSasl;
      bool m_manageRoster;
      bool m_doAuth;

      int m_streamFeatures;
      int m_priority;

  };

}

#endif // CLIENT_H__
