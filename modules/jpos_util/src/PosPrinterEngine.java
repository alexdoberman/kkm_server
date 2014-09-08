import jpos.*;
import jpos.events.*;

import java.util.*;


/////////////////////////////////////////////////////////////////////////////
class PosPrinterEngine extends Object implements OutputCompleteListener ,StatusUpdateListener
{
    public POSPrinter m_posPrinter;

    public void outputCompleteOccurred( OutputCompleteEvent e )
    {
        System.out.println( "--- ppoc "
                    + "  " + String.valueOf( e.getOutputID() )
                    + "  " + e.getSource().toString() );
    }

    public void statusUpdateOccurred( StatusUpdateEvent e )
    {
        System.out.println( "--- ppsu "
                    + "  " + String.valueOf( e.getStatus() )
                    + "  " + e.getSource().toString() );
    }

    void waitx( int stime )
    {
        try
        {
            Thread.sleep( stime );
        }
        catch ( Exception e )
        {
            System.out.println( "waitx error: " + e );
        }
    }

    public void open(String sDevName) throws JposException
    {
        System.out.println( "open printer " + sDevName +"-->" );
            
        m_posPrinter = new POSPrinter();

        m_posPrinter.addOutputCompleteListener( this );
        m_posPrinter.addStatusUpdateListener( this );

        m_posPrinter.open( sDevName );
        m_posPrinter.claim( 1000 );
        m_posPrinter.setDeviceEnabled( true );
        m_posPrinter.setCharacterSet( 1251 );

            
        System.out.println( sDevName + " opened" );
        System.out.println( "    CharacterSetList=" + m_posPrinter.getCharacterSetList() );
        System.out.println( "    CharacterSet=" + m_posPrinter.getCharacterSet() );
        System.out.println( "    RecLineCharsList=" + m_posPrinter.getRecLineCharsList() );
        System.out.println( "    RecLineChars=" + m_posPrinter.getRecLineChars() );
            
        System.out.println( "open printer <--" );
    }

    public void close() throws JposException
    {
        System.out.println( "close printer -->" );

        m_posPrinter.removeOutputCompleteListener( this );
        m_posPrinter.removeStatusUpdateListener( this );

        m_posPrinter.setDeviceEnabled( false );
        m_posPrinter.release();
        m_posPrinter.close();
            
        System.out.println( "close printer <--" );
    }
    
    public void print(String lines[]) throws JposException
    {
        System.out.println( "print -->" );
            
        m_posPrinter.setAsyncMode( false );
        for(int i=0; i<lines.length; i++)
        {
            System.out.println( "print line  = "+ i+ " : "+lines[i] );
            m_posPrinter.printNormal( POSPrinterConst.PTR_S_RECEIPT ,lines[i]);
        }
            
        System.out.println( "print <--" );
        //waitx( 3000 );
    } 


} // class PosPrinterEngine
