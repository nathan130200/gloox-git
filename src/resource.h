/*
  Copyright (c) 2004-2008 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef RESOURCE_H__
#define RESOURCE_H__

#include "presence.h"

#include <string>

namespace gloox
{

  class Presence;

  /**
   * @brief Holds resource attributes.
   *
   * This holds the information of a single resource of a contact that is online.
   *
   * @author Jakob Schroeter <js@caaya.net>
   * @since 0.8
   */
  class GLOOX_API Resource
  {

    friend class RosterItem;

    public:
      /**
       * Constructor.
       * @param priority The resource's priority.
       * @param msg The resource's status message.
       * @param presence The resource's presence status.
       */
      Resource( int priority, const std::string& msg, Presence::PresenceType presence )
        : m_priority( priority ), m_message( msg ), m_presence( presence ) {}

      /**
       * Virtual destrcutor.
       */
      virtual ~Resource()
      {
//         util::clearList( m_extensionList );
        // FIXME
        StanzaExtensionList::iterator it = m_extensions.begin();
        StanzaExtensionList::iterator it2;
        while( it != m_extensions.end() )
        {
          it2 = it++;
          delete (*it2);
          m_extensions.erase( it2 );
        }
        // ~
      }

      /**
       * Lets you fetch the resource's priority.
       * @return The resource's priority.
       */
      int priority() const { return m_priority; }

      /**
       * Lets you fetch the resource's status message.
       * @return The resource's status message.
       */
      const std::string& message() const { return m_message; }

      /**
       * Lets you fetch the resource's last presence.
       * @return The resource's presence status.
       */
      Presence::PresenceType presence() const { return m_presence; }

      /**
       * Returns the StanzaExtensions that were sent with the last presence stanza
       * by the resource.
       * @return A list of stanza extensions.
       */
      const StanzaExtensionList& extensions() const { return m_extensions; }

    private:
      void setPriority( int priority ) { m_priority = priority; }
      void setMessage( std::string message ) { m_message = message; }
      void setStatus( Presence::PresenceType presence ) { m_presence = presence; }
      void setExtensions( const StanzaExtensionList& exts )
      {
        StanzaExtensionList::const_iterator it = exts.begin();
        for( ; it != exts.end(); ++it )
        {
          Tag* t = (*it)->tag();
          m_extensions.push_back( (*it)->newInstance( t ) ); // FIXME suboptimal at best
          delete t;
        }
      }

      int m_priority;
      std::string m_message;
      std::string m_name;
      Presence::PresenceType m_presence;
      StanzaExtensionList m_extensions;

  };

}

#endif // RESOURCE_H__
