
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;

import java.util.Date;
import java.io.*;


public class mapreduce {

  public static void main(String[] args) throws Exception {
    
	  long start = new Date().getTime();
	  if (args.length != 2) {
      System.err.println("Usage: mapreduce <input path> <output path>");
      System.exit(-1);
    }

    Job job = new Job();
    job.setJarByClass(mapreduce.class);
    job.setJobName("Volatility");

    job.setInputFormatClass(TextInputFormat.class);
    job.setOutputFormatClass(TextOutputFormat.class);

    FileInputFormat.addInputPath(job, new Path(args[0]));
    
    
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
    
    job.setMapperClass(VolatilityMapper.class);
    job.setReducerClass(VolatilityReducer.class);

    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(Text.class);
    
    boolean status = job.waitForCompletion(true);
	if (status == true) {
		long end = new Date().getTime();
		System.out.println("\n Mapreduce - Stock Volatility job took " + (end-start)/1000 + "seconds\n");
	}
    
    
    System.exit(job.waitForCompletion(true) ? 0 : 1);
   
            
   }
}
