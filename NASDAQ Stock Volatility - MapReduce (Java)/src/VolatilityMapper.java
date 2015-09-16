
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.util.Arrays;
import java.util.List;
import java.io.File;

//import java.util.StringTokenizer;
import org.apache.hadoop.io.LongWritable;
//import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.InputSplit;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.mapreduce.RecordReader;


public class VolatilityMapper 
  extends Mapper<LongWritable, Text, Text, Text> {

 
 private Text date = new Text();
 private Text adj = new Text();
 public LongWritable key1 = new LongWritable();
 public Text value1 = new Text();
 public static int c = 0;
 static String PrevMonth = "";
 static String CurrentMonth = "";
 static String PrevDay = "";
 static String CurrentDay = "";
 static String PrevYear = "";
 static String CurrentYear = "";
 static String PrevClose = "";
 static String CurrentClose = "";
 static String PrevCompany = "";
 static String CurrentCompany = "";
 static String FirstMonth = "";
 static String FirstYear = "";
 static String FirstClose = "";
 static boolean isBegin = true;
 static int nol = 0;
 static long splitSize = 0;
 static long valueSize = 0;
 static long currentSize = 0;
 static int sameBeginEnd = 0;
 
 static int tmp3=0;
 
 
static public String fileName = new String();
 static public String filePath = new String();
 protected void setup(Context context) throws java.io.IOException, java.lang.InterruptedException
 {
     fileName = ((FileSplit) context.getInputSplit()).getPath().getName().toString();
     filePath = ((FileSplit) context.getInputSplit()).getPath().toString();
     splitSize = context.getInputSplit().getLength();
 }
 
 
  @Override
  public void map(LongWritable key, Text value, Context context)
      throws IOException, InterruptedException {
	
	String dataRow[] = value.toString().split(",");
    nol++;
    String tmp6 = key.toString();
    valueSize = value.getLength();
    currentSize = Long.parseLong(tmp6);
    
	if (!value.toString().contains("Date"))  // To ignore the first entry which is titles of the columns
    {	

		key1 = context.getCurrentKey();
		value1 = context.getCurrentValue();
		
		String dateCheck = dataRow[0];
       	
    	String day = dateCheck.substring(8);
    	String month = dateCheck.substring(5,7);
    	String year = dateCheck.substring(0,4);
    	System.out.println(day + " "+ month+ " "+ year);
    	String monthYear = month + year;
    	date.set(fileName + monthYear);
    	
    	String b="b";
    	String e = "e";
    	System.out.println(monthYear + fileName + "  " + dataRow[6]);
     
    	CurrentDay = day;
    	CurrentMonth = month;
    	CurrentYear = year;
    	CurrentClose = dataRow[6];
    	FirstMonth = month;
    	FirstYear = year;
    	FirstClose = dataRow[6];
    	CurrentCompany = fileName;
    	
    	System.out.println(CurrentMonth + " "+ PrevMonth + " "+ CurrentDay + " "+ PrevDay + " "+ CurrentClose + " "+ PrevClose );
    	System.out.println("Split size : " + splitSize + " Current size " + currentSize + " Value Size : " + valueSize );
    	if ( tmp3 == 0)   // First input record
    	{
    		System.out.println(" In : tmp3 == 0");
    		tmp3 = 1;
    		PrevMonth = CurrentMonth;
    		PrevDay = CurrentDay;
    		PrevYear = CurrentYear;
    		PrevClose = CurrentClose;
    		PrevCompany = CurrentCompany;
    		date.set(fileName + CurrentMonth + CurrentYear);
    		adj.set(e + CurrentClose);
  		    context.write(date, adj);
    		isBegin = false;
    		
    	}
    	/*
    	else if(!context.nextKeyValue())
    	{	
    		date.set(CurrentCompany + CurrentMonth + CurrentYear);
    		adj.set(b + CurrentClose);
    		System.out.println(" In : !context.nextKeyValue()");
    		System.out.println("Absolutely Simple Logic");
    		
    		context.write(date, adj);
    	}
    		
    	*/	
    	//-------------converting input records into <key , value> pairs for input to reduce job--------------------//
    	else if(currentSize + valueSize == splitSize || currentSize + valueSize == splitSize - 1 )
    	{
    		date.set(CurrentCompany + CurrentMonth + CurrentYear);
    		adj.set(b + CurrentClose);
    		//System.out.println(" In : !context.nextKeyValue()");
    		
    		context.write(date, adj);
    		
    		if(!PrevMonth.equals(CurrentMonth))
    		{
    			date.set(CurrentCompany + CurrentMonth + CurrentYear);
        		adj.set(e + CurrentClose);
        		//System.out.println(" In : !context.nextKeyValue()");
        		
        		context.write(date, adj);
        		
        		date.set(CurrentCompany + PrevMonth + PrevYear);
        		adj.set(b + PrevClose);
        		
        		context.write(date, adj);
        		
        		
    		}
    	}
    		
    	
    	else if ( PrevMonth.equals(CurrentMonth))
    	{
    		PrevMonth = CurrentMonth;
    		PrevDay = CurrentDay;
    		PrevYear = CurrentYear;
    		PrevClose = CurrentClose;
    		sameBeginEnd = 0;
    		System.out.println(" In : PrevMonth.equals(CurrentMonth)");
    	}
    	
    	else if( !PrevYear.equals(CurrentYear))
    	{
    		System.out.println(" In : !PrevYear.equals(CurrentYear)");
    		date.set(fileName + PrevMonth + PrevYear);
    		adj.set(b + PrevClose);
  		    context.write(date, adj);
  		    
  		    date.set(fileName + CurrentMonth + CurrentYear);
  		    adj.set(e + CurrentClose);
		    context.write(date, adj);
		    
		    PrevMonth = CurrentMonth;
    		PrevDay = CurrentDay;
    		PrevYear = CurrentYear;
    		
    		PrevClose = CurrentClose;

    		
    	}
    	else if ( !PrevMonth.equals(CurrentMonth))
    	{
    		System.out.println(" In : !PrevMonth.equals(CurrentMonth)");
    		sameBeginEnd++;
    		System.out.println("SameBeginEnd : " +sameBeginEnd);
    		if(sameBeginEnd == 1)
    		{	
    			date.set(fileName + PrevMonth + CurrentYear);
    			adj.set(b + PrevClose);
    			context.write(date, adj);
  		    
    			date.set(fileName + CurrentMonth + CurrentYear);
    			adj.set(e + CurrentClose);
    			context.write(date, adj);
    		}
    		
    		else if(sameBeginEnd == 2)
    		{	
    			date.set(fileName + PrevMonth + CurrentYear);
    			adj.set(b + PrevClose);
    			context.write(date, adj);
    		}	
		    PrevMonth = CurrentMonth;
    		PrevDay = CurrentDay;
    		
    		
    		PrevClose = CurrentClose;
		    
    	}
  	    	
    }

  }
}

    
