/*
 * Copyright (c) 2007 by Jakob Schroeter <js@camaya.net>
 * This file is part of the gloox library. http://camaya.net/gloox
 *
 * This software is distributed under a license. The full license
 * agreement can be found in the file LICENSE in this distribution.
 * This software may not be copied, modified, sold or distributed
 * other than expressed in the named license agreement.
 *
 * This software is distributed without any warranty.
 */


#ifndef TLSDEFAULT_H__
#define TLSDEFAULT_H__

#include "tlsbase.h"

namespace gloox
{

  class TLSHandler;

  /**
   * @brief This is an abstraction of the various TLS implementations.
   *
   * @author Jakob Schroeter <js@camaya.net>
   * @since 0.9
   */
  class TLSDefault : public TLSBase
  {
    public:
      /**
       *
       */
      TLSDefault( TLSHandler *th, const std::string server );

      /**
       * Virtual Destructor.
       */
      virtual ~TLSDefault();

      // re-implemented from TLSBase
      virtual bool encrypt( const std::string& data );

      // re-implemented from TLSBase
      virtual int decrypt( const std::string& data );

      // re-implemented from TLSBase
      virtual void cleanup();

      // re-implemented from TLSBase
      virtual bool handshake();

      // re-implemented from TLSBase
      bool isSecure() const;

      // re-implemented from TLSBase
      virtual void setCACerts( const StringList& cacerts );

      // re-implemented from TLSBase
      const CertInfo& fetchTLSInfo() const;

      // re-implemented from TLSBase
      virtual void setClientCert( const std::string& clientKey, const std::string& clientCerts );

    private:
      TLSBase* m_impl;
  };
}

#endif // TLSDEFAULT_H__
