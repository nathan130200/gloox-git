/*
 * Copyright (c) 2004-2009 by Jakob Schroeter <js@camaya.net>
 * This file is part of the gloox library. http://camaya.net/gloox
 *
 * This software is distributed under a license. The full license
 * agreement can be found in the file LICENSE in this distribution.
 * This software may not be copied, modified, sold or distributed
 * other than expressed in the named license agreement.
 *
 * This software is distributed without any warranty.
 */


#include "connectiontls.h"
#include "tlsdefault.h"

namespace gloox
{

  ConnectionTLS::ConnectionTLS( ConnectionDataHandler* cdh, ConnectionBase* conn, const LogSink& log )
    : ConnectionBase( cdh ),
      m_connection( conn ), m_log( log ), m_handshaked( false )
  {
    if( m_connection )
      m_connection->registerConnectionDataHandler( this );
  }

  ConnectionTLS::ConnectionTLS( ConnectionBase* conn, const LogSink& log )
    : ConnectionBase( 0 ),
      m_connection( conn ), m_log( log ), m_handshaked( false )
  {
    if( m_connection )
      m_connection->registerConnectionDataHandler( this );
  }

  ConnectionTLS::~ConnectionTLS()
  {
    delete m_connection;
    delete m_tls;
  }

  ConnectionError ConnectionTLS::connect()
  {
    if( !m_connection )
      return ConnNotConnected;

    m_tls = new TLSDefault( this, m_connection->server() );
    if( !m_tls )
      return ConnTlsNotAvailable;

    if( !m_tls->init() )
      return ConnTlsFailed;

    m_state = StateConnecting;

    if( m_connection->state() != StateConnected )
      return m_connection->connect();

    if( m_tls->handshake() )
      return ConnNoError;
    else
      return ConnTlsFailed;
 }

  ConnectionError ConnectionTLS::recv( int timeout )
  {
    if( m_connection->state() == StateConnected )
    {
      return m_connection->recv( timeout );
    }
    else
    {
      m_log.log( LogLevelWarning, LogAreaClassConnectionTLS,
                 "Attempt to receive data on a connection that is not connected (or is connecting)" );
      return ConnNotConnected;
    }
  }

  bool ConnectionTLS::send( const std::string& data )
  {
    if( m_handshaked && m_state == StateConnected )
    {
      // m_log.log(LogLevelDebug, LogAreaClassConnectionTLS, "Encrypting data...");
      printf( "Encrypting data...\n----------------\n<%s>\n----------\n", data.c_str() );
      m_tls->encrypt( data );
      return true;
    }
    else
    {
      m_log.log( LogLevelWarning, LogAreaClassConnectionTLS,
                 "Attempt to send data on a connection that is not connected (or is connecting)" );
      return false;
    }
  }

  ConnectionError ConnectionTLS::receive()
  {
    if( m_connection )
      return m_connection->receive();
    else
      return ConnNotConnected;
  }

  void ConnectionTLS::disconnect()
  {
    if( m_connection )
      m_connection->disconnect();

    cleanup();
  }

  void ConnectionTLS::cleanup()
  {
    if( m_connection )
      m_connection->cleanup();
    if( m_tls )
      m_tls->cleanup();
    delete m_tls;
    m_tls = 0;
    m_state = StateDisconnected;
    m_handshaked = false;
  }

  void ConnectionTLS::getStatistics( int& totalIn, int& totalOut )
  {
    if( m_connection )
      m_connection->getStatistics( totalIn, totalOut );
  }

  ConnectionBase* ConnectionTLS::newInstance() const
  {
    ConnectionBase* newConn = 0;
    if( m_connection )
      newConn = m_connection->newInstance();
    return new ConnectionTLS( m_handler, newConn, m_log );
  }

  void ConnectionTLS::handleReceivedData( const ConnectionBase* /*connection*/, const std::string& data )
  {
    m_log.log( LogLevelDebug, LogAreaClassConnectionTLS, "Decrypting received data..." );
    if( m_tls )
      m_tls->decrypt( data );
  }

  void ConnectionTLS::handleConnect( const ConnectionBase* /*connection*/ )
  {
    if( m_handshaked )
      return;

    m_log.log( LogLevelDebug, LogAreaClassConnectionTLS, "Beginning TLS handshake...." );

    if( m_tls )
      m_tls->handshake();
  }

  void ConnectionTLS::handleDisconnect( const ConnectionBase* /*connection*/, ConnectionError reason )
  {
    if( m_handler )
      m_handler->handleDisconnect( this, reason );

    cleanup();
  }

  void ConnectionTLS::handleEncryptedData( const TLSBase* /*tls*/, const std::string& data )
  {
    // m_log.log(LogLevelDebug, LogAreaClassConnectionTLS,
    printf( "Sending encrypted data...\n" );
    if( m_connection )
      m_connection->send( data );
  }

  void ConnectionTLS::handleDecryptedData( const TLSBase* /*tls*/, const std::string& data )
  {
    if( m_handler )
    {
      m_log.log( LogLevelDebug, LogAreaClassConnectionTLS, "Handling decrypted data... \n" + data );
      m_handler->handleReceivedData( this, data );
    }
    else
    {
      m_log.log( LogLevelDebug, LogAreaClassConnectionTLS, "Data received and decrypted but no handler" );
    }
  }

  void ConnectionTLS::handleHandshakeResult( const TLSBase* /*tls*/, bool success, CertInfo& certinfo )
  {
    if( success )
    {
      m_handshaked = true;
      m_state = StateConnected;
      m_log.log( LogLevelDebug, LogAreaClassConnectionTLS, "TLS handshake succeeded" );
      if( m_handler )
        m_handler->handleConnect( this );
    }
    else
    {
      m_state = StateDisconnected;
      m_log.log( LogLevelWarning, LogAreaClassConnectionTLS, "TLS handshake failed" );
      disconnect();
    }
  }

}
