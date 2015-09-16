// Learning MapReduce by Nitesh Jain
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
//import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;
//import org.apache.hadoop.mapred.TextInputFormat;
import java.io.*;
//import java.util.Arrays;

public class mapreduce {

  public static void main(String[] args) throws Exception {
    if (args.length != 2) {
      System.err.println("Usage: mapreduce <input path> <output path>");
      System.exit(-1);
    }
       
    Job job = new Job();
    job.setJarByClass(mapreduce.class);
    job.setJobName("PeopleYouMayKnow");

    job.setInputFormatClass(TextInputFormat.class);
    job.setOutputFormatClass(TextOutputFormat.class);

    
    
    FileInputFormat.addInputPath(job, new Path(args[0]));
    
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
    
    job.setMapperClass(VolatilityMapper.class);
    job.setReducerClass(VolatilityReducer.class);

    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(Text.class);
    
    System.exit(job.waitForCompletion(true) ? 0 : 1);
   }
}
