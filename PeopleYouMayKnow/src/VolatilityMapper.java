//import java.io.BufferedReader;
//import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;


//import java.util.StringTokenizer;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.mapreduce.Mapper;
//import org.apache.hadoop.mapreduce.InputSplit;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
//import org.apache.hadoop.mapreduce.MapContext;
//import org.apache.hadoop.mapreduce.lib.input.*;

public class VolatilityMapper 
  extends Mapper<LongWritable, Text, Text, Text> {

 
 public String fileName = new String();
/* 
 protected void setup(Context context) throws java.io.IOException, java.lang.InterruptedException
 {
     fileName = ((FileSplit) context.getInputSplit()).getPath().toString();
 }
*/ 
 
  @Override
  public void map(LongWritable key, Text value, Context context)
      throws IOException, InterruptedException {

	  
	String dataRow[] = value.toString().split("\t");
	String current_user;
	String[] friends_of_current_user = null;
	current_user = dataRow[0];
	if (dataRow.length >= 2)
	{
		friends_of_current_user = dataRow[1].split(",");
			for (int i = 0; i<friends_of_current_user.length - 1; i++)
			{	
				for(int j = i + 1; j < friends_of_current_user.length ; j++)
				{
					
					Text user = new Text();
					String mapper_key = friends_of_current_user[i];
					Text val = new Text();
					
					long temp = 1;
					user.set(mapper_key);
					val.set(friends_of_current_user[j] +  "," + "1");
					//System.out.println("Key - " + mapper_key + "  Value - " + friends_of_current_user[j]+ "," +"1" );
					context.write(user , val);
					//}
				}
				
				Text user = new Text();
				Text friend_flag = new Text();
				
				String mapper_key2 = new String();
				mapper_key2 =  dataRow[0] ;
				long temp2 = -1;
				//System.out.println("Key - " + mapper_key2 + "  Value - " + friends_of_current_user[i]+ "," +"-1" );
				user.set(mapper_key2);
				friend_flag.set(friends_of_current_user[i]+ "," +"-1");
				
				context.write(user , friend_flag);
				
			}
			Text user_1 = new Text(dataRow[0]);
			Text last_friend = new Text(friends_of_current_user[friends_of_current_user.length-1]+ "," +"-1" );
			//System.out.println("Key - " + dataRow[0] + "  Value - " + friends_of_current_user[friends_of_current_user.length-1]+ "," +"-1");
			context.write(user_1 , last_friend);
			
	}
  
  }

}

    
