import java.util.*;
import java.io.*;
import org.apache.commons.codec.binary.Base64;
import jpos.*;
import jpos.events.*;

import java.nio.charset.Charset;

class PosPrinter
{
    public static void main(String args[])
    {
    	System.out.println("Default Charset=" + Charset.defaultCharset());
        try
        {
            if (args.length < 2)
            {
                System.out.println("Usage: <app>  <logic_dev_id> <base64_encode_line_1> <base64_encode_line_2> ....");
                System.out.println("Print <line_1> <line_2> .... on the   <logic_dev_id>.");
            }
            else if (args.length >= 2)
            {
                PosPrinterEngine ppEngine = new PosPrinterEngine();
                String lines[] = new String [args.length - 1];
                //FileWriter outF = new FileWriter("out.txt");
                for (int i = 1; i< args.length; i++)
                {
                    byte[] decoded = Base64.decodeBase64(args[i]);
                    //lines[i-1] = new String(decoded, "UTF-8");
                    lines[i-1] = new String(decoded);
                    //outF.write(lines[i-1]);
                } 
                //outF.close();
                
                ppEngine.open(args[0]);
                ppEngine.print(lines);
                ppEngine.close();
            }
        }
        catch ( JposException e )
        {
            int errorCode         = e.getErrorCode(); 
            int errorCodeExtended = e.getErrorCodeExtended(); 
            String Description    = e.getMessage();
            String exStr = "PosPrinter.Exception: err = "+errorCode+", errEx = "+errorCodeExtended+", desc = " + Description;
            System.out.println( exStr );
        }
        catch ( Exception e )
        {
            e.printStackTrace();
            String exStr = "PosPrinter.Exception: err = "+2000+", errEx = "+2000+", desc = " + "unknown reason fail";
            System.out.println( exStr );
        }
    }
}