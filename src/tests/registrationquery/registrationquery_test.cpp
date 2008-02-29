#define REGISTRATION_TEST
#include "../../gloox.h"
#include "../../jid.h"
#include "../../dataform.h"
#include "../../tag.h"
#include "../../iq.h"
#include "../../iqhandler.h"
#include "../../stanzaextension.h"
#include "../../stanzaextensionfactory.h"
using namespace gloox;

#include <stdio.h>
#include <locale.h>
#include <string>


namespace gloox
{

  class Disco;
  class Capabilities : public StanzaExtension
  {
    public:
      Capabilities() : StanzaExtension( ExtUser + 1 ) {}
      const std::string& ver() const { return EmptyString; }
      const std::string& node() const { return EmptyString; }
  };

  class ClientBase
  {
    public:
      ClientBase() {}
      virtual ~ClientBase() {}
      const std::string getID() { return "id"; }
      virtual void send( IQ& iq, IqHandler*, int ) = 0;
      virtual void trackID( IqHandler *ih, const std::string& id, int context ) = 0;
      void removeIDHandler( IqHandler* ) {}
  };

}

#define CLIENTBASE_H__
#include "../../registration.h"
#include "../../registration.cpp"

int main( int /*argc*/, char** /*argv*/ )
{
  int fail = 0;
  std::string name;

  // -------
  {
    name = "fetch reg fields";
    Registration::Query sq;
    Tag* t = sq.tag();
    if( !t || t->xml() != "<query xmlns='" + XMLNS_REGISTER + "'/>" )
    {
      ++fail;
      printf( "test '%s' failed\n", name.c_str() );
    }
    delete t;
  }

  // -------
  {
    name = "receive search fields";
    Tag* d = new Tag( "query" );
    d->setXmlns( XMLNS_REGISTER );
    d->addChild( new Tag( "instructions", "foobar" ) );
    d->addChild( new Tag( "first" ) );
    d->addChild( new Tag( "last" ) );
    d->addChild( new Tag( "email" ) );
    d->addChild( new Tag( "nick" ) );
    Registration::Query sq( d );
    Tag* t = sq.tag();
    if( !t || t->xml() != "<query xmlns='" + XMLNS_REGISTER + "'>"
         "<instructions>foobar</instructions>"
         "<first/>"
         "<last/>"
         "<nick/>"
         "<email/>"
         "</query>"
         || sq.instructions() != "foobar"
         || sq.fields() != ( SearchFieldFirst | SearchFieldLast | SearchFieldNick | SearchFieldEmail ) )
    {
      ++fail;
      printf( "test '%s' failed\n", name.c_str() );
    }
    delete t;
    delete d;
  }

  // -------
  {
    name = "receive search form";
    Tag* d = new Tag( "query" );
    d->setXmlns( XMLNS_REGISTER );
    Tag* f = new Tag( d, "x" );
    f->setXmlns( XMLNS_X_DATA );
    f->addAttribute( "type", "form" );
    Registration::Query sq( d );
    Tag* t = sq.tag();
    if( !t || t->xml() != "<query xmlns='" + XMLNS_REGISTER + "'>"
         "<x xmlns='" + XMLNS_X_DATA + "' type='form'/>"
         "</query>"
         || !sq.form() )
    {
      ++fail;
      printf( "test '%s' failed: %s\n", name.c_str(), t->xml().c_str() );
    }
    delete t;
    delete d;
  }

  // -------
  {
    name = "search by form";
    DataForm* form = new DataForm( TypeSubmit );
    Registration::Query sq( form );
    Tag* t = sq.tag();
    if( !t || t->xml() != "<query xmlns='" + XMLNS_REGISTER + "'>"
       "<x xmlns='" + XMLNS_X_DATA + "' type='submit'/>"
       "</query>" )
    {
      ++fail;
      printf( "test '%s' failed\n", name.c_str() );
    }
    delete t;
  }

  // -------
  {
    name = "search by fields";
    SearchFieldStruct sfs( "first", "last", "nick", "email" );
    Registration::Query sq( SearchFieldFirst | SearchFieldLast | SearchFieldNick | SearchFieldEmail, sfs );
    Tag* t = sq.tag();
    if( !t || t->xml() != "<query xmlns='" + XMLNS_REGISTER + "'>"
         "<first>first</first>"
         "<last>last</last>"
         "<nick>nick</nick>"
         "<email>email</email>"
         "</query>" )
    {
      ++fail;
      printf( "test '%s' failed\n", name.c_str() );
    }
    delete t;
  }

  // -------
  {
    name = "receive form result";
    Tag* d = new Tag( "query" );
    d->setXmlns( XMLNS_REGISTER );
    Tag* f = new Tag( d, "x" );
    f->setXmlns( XMLNS_X_DATA );
    f->addAttribute( "type", "result" );
    Registration::Query sq( d );
    Tag* t = sq.tag();
    if( !t || t->xml() != "<query xmlns='" + XMLNS_REGISTER + "'>"
         "<x xmlns='" + XMLNS_X_DATA + "' type='result'/>"
         "</query>" )
    {
      ++fail;
      printf( "test '%s' failed\n", name.c_str() );
    }
    delete t;
    delete d;
  }

  // -------
  {
    name = "receive fields result";
    Tag* d = new Tag( "query" );
    d->setXmlns( XMLNS_REGISTER );
    Tag* i = new Tag( d, "item" );
    i->addAttribute( "jid", "foo@bar" );
    i->addChild( new Tag( "first", "first1" ) );
    i->addChild( new Tag( "last", "last1" ) );
    i->addChild( new Tag( "email", "email1" ) );
    i->addChild( new Tag( "nick", "nick1" ) );
    i = new Tag( d, "item" );
    i->addAttribute( "jid", "foo@bar2" );
    i->addChild( new Tag( "first", "first2" ) );
    i->addChild( new Tag( "last", "last2" ) );
    i->addChild( new Tag( "nick", "nick2" ) );
    i->addChild( new Tag( "email", "email2" ) );
    Registration::Query sq( d );
    Tag* t = sq.tag();
    SearchResultList srl = sq.result();
    if( !t || t->xml() != "<query xmlns='" + XMLNS_REGISTER + "'>"
         "<item jid='foo@bar'>"
         "<first>first1</first>"
         "<last>last1</last>"
         "<nick>nick1</nick>"
         "<email>email1</email></item>"
         "<item jid='foo@bar2'>"
         "<first>first2</first>"
         "<last>last2</last>"
         "<nick>nick2</nick>"
         "<email>email2</email></item>"
         "</query>"
       || srl.size() != 2 )
    {
      ++fail;
      printf( "test '%s' failed: %s\n", name.c_str(), t->xml().c_str() );
    }
    delete t;
  }




  // -------
  name = "Registration::Query/SEFactory test";
  StanzaExtensionFactory sef;
  sef.registerExtension( new Registration::Query() );
  Tag* f = new Tag( "iq" );
  new Tag( f, "query", "xmlns", XMLNS_REGISTER );
  IQ iq( IQ::Get, JID() );
  sef.addExtensions( iq, f );
  const Registration::Query* se = iq.findExtension<Registration::Query>( ExtRegistration );
  if( se == 0 )
  {
    ++fail;
    printf( "test '%s' failed\n", name.c_str() );
  }
  delete f;



  printf( "Registration::Query: " );
  if( fail == 0 )
  {
    printf( "OK\n" );
    return 0;
  }
  else
  {
    printf( "%d test(s) failed\n", fail );
    return 1;
  }

}