/*
  Copyright (c) 2006 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef VCARDHANDLER_H__
#define VCARDHANDLER_H__

namespace gloox
{

  class VCard;

  /**
   * @brief A virtual interface that helps requesting Jabber VCards.
   *
   * Derive from this interface and register with the VCardManager.
   * See @link gloox::VCardManager VCardManager @endlink for info on how to
   * fetch VCards.
   *
   * @author Jakob Schroeter <js@camaya.net>
   * @since 0.8
   */
  class GLOOX_API VCardHandler
  {
    public:
      /**
       * Describes possible operation contexts.
       */
      enum VCardContext
      {
        FetchVCard,                 /**< Operation involves fetching a VCard. */
        StoreVCard                  /**< Operation involves storing a VCard. */
      };

      /**
       * Virtual destructor.
       */
      virtual ~VCardHandler() {};

      /**
       * This function is called when a VCard has been successfully fetched.
       * The VCardHandler becomes owner of the VCard object and is responsible for deleting it.
       * @param jid The JID to which this VCard belongs.
       * @param vcard The fetched VCard. This may be 0 if there is no VCard for this contact.
       */
      virtual void handleVCard( const JID& jid, const VCard *vcard ) = 0;

      /**
       * This function is called to indicate the result of a VCard store operation
       * or any error that occurs.
       * @param context The operation which yielded the result.
       * @param jid The JID involved.
       * @param se The error, if any. If equal to @c StanzaErrorUndefined no error occured.
       */
      virtual void handleVCardResult( VCardContext context, const JID& jid,
                                      StanzaError se = StanzaErrorUndefined  ) = 0;

  };

}

#endif // VCARDHANDLER_H__
