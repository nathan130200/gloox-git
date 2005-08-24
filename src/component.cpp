/*
  Copyright (c) 2005 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warrenty.
*/



#include "component.h"

#include "disco.h"
#include "stanza.h"

namespace gloox
{

  Component::Component( const std::string& ns, const std::string& server,
                          const std::string& component, const std::string& password, int port )
    : ClientBase( ns, password, server, port ),
      m_to( component ), m_disco( 0 )
  {
//     m_disco = new Disco( this );
//     m_disco->setVersion( "based on gloox", GLOOX_VERSION );
//     m_disco->setIdentity( "component", "generic" );
  }

  Component::~Component()
  {
//     delete m_disco;
  }

  void Component::handleStartNode()
  {
    printf( "in handleStartNode\n" );
    if( m_sid.empty() )
      return;

    const std::string data = m_sid + m_password;
    char *hash = (char*)calloc( 41, sizeof( char ) );
    iks_sha( data.c_str(), hash );

    Tag *h = new Tag( "handshake", hash );
    send( h );

    free( hash );
  }

  bool Component::handleNormalNode( Stanza *stanza )
  {
    printf( "in handleNormalNode\n" );
    if( stanza->name() == "handshake" )
      notifyOnConnect();
    else
      return false;

    return true;
  }

};
