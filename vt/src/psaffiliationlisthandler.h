/*
  Copyright (c) 2006-2007 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/

#ifndef PSAFFILIATIONLISTHANDLER_H__
#define PSAFFILIATIONLISTHANDLER_H__

#include "jid.h"
#include <string>
#include <map>
#include "pubsub.h"

namespace gloox
{
  /**
   * @brief A virtual interface for receiving (un)subscription result.
   *
   * Derive from this interface and register with the PubSubManagerManager.
   *
   * @author Jakob Schroeter <js@camaya.net>
   */
  class PSAffiliationListHandler
  {
    public:
      /**
       * Receives the Affiliation map for a specific service.
       * @param jid The queried service.
       * @param subMap The map of each node's affiliation subscription.
       */
      virtual void handleAffiliationListResult( const JID& jid, const AffiliationMap& subMap ) = 0;

      /**
       * Receives the Subscription map for a specific service.
       * @param jid The service.
       * @param subMap The map of subscriptions.
       */
      virtual void handleAffiliationListError( const std::string& jid, const std::string& node ) = 0;

      /**
       * Virtual destructor.
       */
      virtual ~PSAffiliationListHandler() {}

  };

}

#endif /* PSAFFILIATIONLISTHANDLER_H__ */
