#include "../jclient.h"
#include "../prep.h"
#include "../connectionlistener.h"
#include "../annotationshandler.h"
#include "../disco.h"
#include "../annotations.h"
using namespace gloox;

#include <stdio.h>
#include <locale.h>
#include <string>

class AnnotationsTest : public AnnotationsHandler, ConnectionListener
{
  public:
    AnnotationsTest() {};
    ~AnnotationsTest() {};

    void start()
    {
      setlocale( LC_ALL, "" );

      j = new JClient();
      j->setServer( "example.org" );
      j->setResource( "gloox" );
      j->setUsername( "hurkhurk" );
      j->setPassword( "hurkhurks" );
      j->setAutoPresence( true );
      j->setInitialPriority( 5 );

      j->registerConnectionListener(this );
      j->disco()->setVersion( "annotationsTest", GLOOX_VERSION );
      j->disco()->setIdentity( "client", "bot" );

      j->setDebug( true );
      j->set_log_hook();

      a = new Annotations( j );
      a->registerAnnotationsHandler( this );

      j->connect( true );

      delete( j );
    };

    virtual void onConnect()
    {
      a->requestAnnotations();
    };

    virtual void handleAnnotations( AnnotationsList aList )
    {
      printf( "received notes...\n" );
      AnnotationsList mybList;

      annotationsListItem bItem;
      bItem.jid = "romeo@montague.org";
      bItem.note = "my lover & friend. 2 > 3";
      mybList.push_back( bItem );

      bItem.jid = "juliet@capulet.com";
      bItem.note = "oh my sweetest love...";
      bItem.cdate = "20040924T15:23:21";
      bItem.mdate = "20040924T15:23:21";
      mybList.push_back( bItem );

      a->storeAnnotations( mybList );
    };

  private:
    JClient *j;
    Annotations *a;
};

int main( int argc, char* argv[] )
{
  AnnotationsTest *t = new AnnotationsTest();
  t->start();
  delete( t );
}
