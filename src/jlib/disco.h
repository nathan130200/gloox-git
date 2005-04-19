/*
  Copyright (c) 2004-2005 by Jakob Schroeter <js@camaya.net>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/



#ifndef DISCO_H__
#define DISCO_H__

#include "iqhandler.h"

class JClient;

class Disco : public IqHandler
{
  public:
    /**
     * Constructor.
     * Creates a new Disco client that registers as IqHandler with @c JClient.
     * @param parent The JClient used for XMPP communication
     */
    Disco( JClient* parent );

    /**
     * Virtual destructor.
     */
    virtual ~Disco();

    /**
     * reimplemented from IqHandler.
     */
    virtual void handleIq( const char* xmlns, ikspak* pak );

  private:
    JClient* m_parent;

};

#endif // DISCO_H__
