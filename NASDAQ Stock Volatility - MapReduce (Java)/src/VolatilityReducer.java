import java.io.IOException;

//import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

 class CompanyVolatility
{
	 String company;
	 double volatility;
	
}

public class VolatilityReducer
  extends Reducer<Text, Text, Text, Text> {
	
  static int tmp2 = 0;	  // Used to set prevCompany = CurrentCompany for the first company
  static int IndexCounter = 0;		// Used to keep track of number of months in each company
  static boolean companyChange = false;		// Used to check if company has been changed to reset some variables
  static String PrevCompany = "";	
  static String CurrentCompany = "";
  static double totalMonthlyReturn = 0;
  static double vol = 0;
  static int badEntry = 0;
  static int badEntry1 = 1;
  static int writtenOnce = 1;
  static int badIndex= 0;
  static boolean tmp4 = false;
  static Text xi = new Text();

  
  
  double xarray[] = new double[36];	
  String karray[] = new String[36];
  
  static String[] company = new String[30000];
  static double [] volatility = new double[30000];
  static String[] company1 = new String[30000];
  static double [] volatility1 = new double[30000];
  
  static int finalIndex = 0;
  
  int index = 0;
  
  protected void cleanup(Context context) throws java.io.IOException, java.lang.InterruptedException
  {
	  writtenOnce = 0;
	  // System.out.println("In the finalindex if : "+k+ "  "+   "  Cur:   " +CurrentCompany);
	  
	  
	  
	  //---------------Calculating the volatility of the last company ----------------------//
	  System.out.println("IndexCounter : " + IndexCounter);
	  totalMonthlyReturn = totalMonthlyReturn / (IndexCounter + 1 );
	  for(int i=0 ; i<=IndexCounter; i++)
	  {
		vol = vol + (xarray[i] - totalMonthlyReturn) * (xarray[i] - totalMonthlyReturn);   
	  }  
	  
	  System.out.println(vol + "  "+ totalMonthlyReturn);
	  vol = vol / (IndexCounter );
	  System.out.println(vol);
	  vol = Math.sqrt(vol);
	  System.out.println(vol);
	  
	  company[finalIndex] = PrevCompany;
	  volatility[finalIndex] = vol;
	  company1[finalIndex] = PrevCompany;
	  volatility1[finalIndex] = vol;

	  
	  //--------------------Sorting the stocks as per volatility -------------------------//
	  
	  
	  //--------Ignoring stocks with 0 volatility----------//
	  for(int i = 0 ; i <= finalIndex ; i++)
    		  if(volatility[i]==0.0)
    			  volatility[i] = 99999.0;

	  for(int i = 0 ; i <= finalIndex ; i++)
		  if(volatility1[i]==0.0)
			  volatility1[i] = -99999;
	  
	  
	  //-------Actual Sorting--------//
	  for(int i = 0 ; i <= finalIndex ; i++)
	  for(int j = 0; j<i ; j++)
	  {
		  if(volatility[i]<volatility[j])
		  {
			  String tmp3;
			  double tmp4;
			  
			  
			  tmp4 = volatility[i];
			  volatility[i] = volatility[j];
			  volatility[j] = tmp4;
			  
			  tmp3 = company[i];
			  company[i] = company[j];
			  company[j] = tmp3;
			  
		  }
		  
		  if(volatility1[i]>volatility1[j])
		  {
			  String tmp3;
			  double tmp4;
			  
			  
			  tmp4 = volatility1[i];
			  volatility1[i] = volatility1[j];
			  volatility1[j] = tmp4;
			  
			  tmp3 = company1[i];
			  company1[i] = company1[j];
			  company1[j] = tmp3;
			  
		  }
			  
	  }
	  
	  for(int i = 0 ; i<=finalIndex ; i++)
		  System.out.println(i + " " + company[i] + volatility[i] );
	  
	  System.out.println();
	  
	  for(int i = 0 ; i<=finalIndex ; i++)
		  System.out.println(i + " " + company1[i] + volatility1[i] );
	  
	  
	  //----------Printing top 10 and bottom 10 stocks as per volatilities--------//
	  
	  Text key4 = new Text("The top 10 Stocks With the Lowest (min) Volatility : ");
	  Text key5 = new Text("The top 10 Stocks With the Highest (max) Volatility : ");
	  Text tempVal = new Text("");
	  context.write(key4,  tempVal);
	  for(int i = 0; i< 10; i++)
	  {
		  String tmp5 = Double.toString(volatility[i]);
		  xi.set(tmp5);
		  Text key3 = new Text();
		  key3.set(company[i]);
		 context.write(key3, xi);
	  } 
	  
	  context.write(key5,  tempVal);
	  
	  for(int i = 0; i< 10; i++)
	  {
		  String tmp5 = Double.toString(volatility1[i]);
		  xi.set(tmp5);
		  Text key3 = new Text();
		  key3.set(company1[i]);
		 context.write(key3, xi);
	  } 
	  
  
  
  }
  @Override
  public void reduce(Text key, Iterable<Text> values,
      Context context)
      throws IOException, InterruptedException {
    
      double x = 0;
      vol = 0;
      
      double begin ,end;
      
      begin = 0;
      end = 0;
      String k=""; 
      k = key.toString();
      CurrentCompany = k.substring(0, k.length() - 6);
      
      
      
      if((tmp4 && !PrevCompany.equals(CurrentCompany)))
      {
    	  //--------------------------calculating volatility for a company-----------------------// 
    	  System.out.println("IndexCounter : " + IndexCounter);
    	  totalMonthlyReturn = totalMonthlyReturn / (IndexCounter + 1 );
    	  for(int i=0 ; i<=IndexCounter; i++)
    	  {
    		vol = vol + (xarray[i] - totalMonthlyReturn) * (xarray[i] - totalMonthlyReturn);   
    	  }  
    	  
    	  System.out.println(vol + "  "+ totalMonthlyReturn);
    	  vol = vol / (IndexCounter );
    	  System.out.println(vol);
    	  vol = Math.sqrt(vol);
    	  System.out.println(vol);
    	  
    	  String tmp = Double.toString(vol);
    	  xi.set(tmp);
    	  Text key2 = new Text();
    	  key2.set(PrevCompany);
    	  
    	  System.out.println("Writing to  output : "+k+ "  "+ key.toString() + begin+ "  "+ end+ "  " + "Volatilit " + vol + "  Index Counter: "+   IndexCounter + "   Prev:  " + PrevCompany + "  Cur:   " +CurrentCompany);
    	  System.out.println("Current company : " + CurrentCompany + "Previous Company" + PrevCompany );
    	  //  context.write(key2 , xi );
    	  
    	  company[finalIndex] = PrevCompany;
    	  volatility[finalIndex] = vol;
    	  company1[finalIndex] = PrevCompany;
    	  volatility1[finalIndex] = vol;
    	  PrevCompany = CurrentCompany;
    	  finalIndex++;
    	  System.out.println("Final Index : " + finalIndex);
    	  IndexCounter = 0;
    	  companyChange  = false;
    	  totalMonthlyReturn = 0;
    	  badEntry = 0;
      }
      
      //-----First Record-----//
      else if(tmp2 == 0)
      {
    	  PrevCompany = CurrentCompany;
    	  tmp2++;
    	  tmp4 = true;
      }	  
      else if (PrevCompany.equals(CurrentCompany))
      {
    	  IndexCounter++;    	  
      }
   
      
      //----------Collecting and parsing data from input records----------//
      for (Text value : values) {
    	  String s = value.toString();
    	  double val = Double.parseDouble(s.substring(1));
    	  
    	  if (s.substring(0,1).equals("b"))
    			  begin = val;
    	  else if (s.substring(0,1).equals("e"))
    		  	end = val;
    
      }
      //-------------Calculating monthly rate of return-------------------//
      x = (end - begin) / begin;
      xarray[IndexCounter] = x;
      karray[IndexCounter] = k;
     
      System.out.println("Key : "+ k + " Begin : "+ begin + " End: "+ end + " IndexCounter : " + IndexCounter);
     
      totalMonthlyReturn += xarray[IndexCounter];
      

  }

}



















//
/*

import java.io.IOException;

import java.util.*;



import org.apache.hadoop.io.IntWritable;

import org.apache.hadoop.io.Text;

import org.apache.hadoop.mapreduce.Reducer;





public class VolatilityReducer

  extends Reducer<Text, Text, Text, Text> {

	

	

  @Override

  public void reduce(Text key, Iterable<Text> values,

      Context context)

      throws IOException, InterruptedException {

	  

	  

	  ArrayList<String> users = new ArrayList<String>();

	ArrayList<Integer> count = new ArrayList<Integer>();

		

		String usr = key.toString();

	 

	  int cnt = 0;

	  //boolean is_friend = false;

	  for(Text value: values)

	  {

		  String c = value.toString().split(",")[1];

		  String friend = value.toString().split(",")[0];

		  //System.out.println("Key - " + usr + "  Value - " + friend + "," + c);

		  int temp = 1;

		  

		  if(c.equals("-1"))

		  {

			  return;

		  }

		  

		  if(users.contains(friend))

		  {

			  

			  int index = users.indexOf(friend);

			  /*

			  for(int x = 0; x < users.size(); x++)

				  if(users.get(x) == friend)

				  {

					  index = x;

					  break;

				  }

			  */

			  //System.out.println("In If - " + "friend - "+ friend  + " Index - " + index + " Count - " + count.get(index)+1);

			  count.add(index, count.get(index)+1);

			  

		  }

		  else

		  {

			  

			  users.add(friend);

			  int index = users.indexOf(friend);

			  /*

			  for(int x = 0; x < users.size(); x++)

				  if(users.get(x) == friend)

				  {

					  index = x;

					  break;

				  }

			*/

			  //System.out.println("In else - Index - " + users.indexOf(friend));

			  count.add(index , 1);

			  

		  }

		  cnt++;

	  

	  }

	  



	  String[] users_2 = new String[users.size()];

	  Integer[] counts = new Integer[count.size()];

	  

	  users_2 = users.toArray(users_2);

	  counts = count.toArray(counts);

	  

	  for(int a = 0 ; a < users.size(); a++)

	  {  System.out.print(" In Reducer - " + users.get(a) + " ");

		  System.out.println(count.get(a));

	  }

	  /*

	  for(int a = 0 ; a < users_2.length; a++)

	  {  System.out.print(" In Reducer - " + users_2[a] + " ");

		  System.out.println(counts[a]);

	  }

	  */

	  

	  for(int k = 0 ; k < users_2.length - 1; k++)

		  for(int j = k; j< users_2.length  ; j++)

		  {

			  if(counts[k]<counts[j])

			  {

				  String tmp3;

				  int tmp4;

				  

				  

				  tmp4 = counts[k];

				  counts[k] = counts[j];

				  counts[j] = tmp4;

				  

				  tmp3 = users_2[k];

				  users_2[k] = users_2[j];

				  users_2[j] = tmp3;

				  

			  }

			  

			  if(counts[k]>counts[j])

			  {

				  String tmp3;

				  int tmp4;

				  

				  

				  tmp4 = counts[k];

				  counts[k] = counts[j];

				  counts[j] = tmp4;

				  

				  tmp3 = users_2[k];

				  users_2[k] = users_2[j];

				  users_2[j] = tmp3;

				  

			  }

				  

		  }

	  

	  	String recommended_friends = "";

	  

	  	if(users.size() > 0 && users.size() < 10)

	  		for(int i = 0; i < users.size(); i++)

	  			recommended_friends += users.get(i);

	  	else

	  		for(int i = 0; i < users.size(); i++)

	  			recommended_friends += users.get(i);

	  	Text user_ID = new Text(usr);

	  	Text recommendations = new Text(recommended_friends);

	  	

	  	context.write( user_ID, recommendations);

	  

	  

	  

	  /*

	  if(!is_friend)

	  {

		  String pair = key.toString();

		  String user1 = pair.split(",")[0];

		  String user2 = pair.split(",")[1];

		  boolean new_user_1 = true;

		  boolean new_user_2 = true;

		  

		  for (int i = 0; i < list.size(); i++) {

	            if(list.get(i).user.equals(user1))

	            {	new_user_1 = false;

	            	break;

	            }

	            else if (list.get(i).user.equals(user2))

	            {

	            	new_user_2 = false;

	            	break;

	            }	

	        }

		  

		  

		  if(new_user_1)

		  {

			  ArrayList<String> temp1 = new ArrayList<String>();

			  ArrayList<Integer> temp2 = new ArrayList<Integer>();

			  

			  temp1.add(user2);

			  temp2.add(count);

			  list.add(new Recommendations(user1 , temp1 , temp2 ));

		  }

		  

		  if(new_user_2)

		  {

			  ArrayList<String> temp1 = new ArrayList<String>();

			  ArrayList<Integer> temp2 = new ArrayList<Integer>();

			  

			  temp1.add(user1);

			  temp2.add(count);

			  list.add(new Recommendations(user2 , temp1 , temp2 ));

		  }

		  

		  if(!new_user_1)

		  {

			  Recommendations r = null;  

			  for (int i = 0; i < list.size(); i++) {

		          

				  if(list.get(i).user.equals(user1))

				   {	

					  r = list.get(i);

					  break;

				   }

			  }

			  

			  r.possible_friends.add(user2);

			  r.number_of_mutual_friends.add(count);

			  

			  

		  }

		  

		  if(!new_user_2)

		  {

			  Recommendations r = null;  

			  for (int i = 0; i < list.size(); i++) {

		          

				  if(list.get(i).user.equals(user2))

				   {	

					  r = list.get(i);

					  break;

				   }

			  }

			  

			  r.possible_friends.add(user1);

			  r.number_of_mutual_friends.add(count);

			    

		  }  

	  }

	  

   

	  */

  

  }

  

  

  

  

  

  /*

  

	  

	  protected void cleanup(Context context) throws java.io.IOException, java.lang.InterruptedException

	  {

		  for(int i = 0; i < list.size(); i++)

		  {

			  Text user_ID = new Text();

			  Text recommended_friends = new Text();

			  

			  String user_ID_key = "";

			  String recommmended_friends_value = "";

			  

			  Recommendations r = list.get(i);

			  if(r.possible_friends.size() == 0)

			  {

				  user_ID_key = r.user;

				  user_ID.set(user_ID_key);

				  

				  recommended_friends.set(recommmended_friends_value);

				  

				  context.write(user_ID, recommended_friends);

			  }	  

			  

			  else if(r.possible_friends.size() < 10)

			  {

				  user_ID_key = r.user;

				  user_ID.set(user_ID_key);

				  

				  String[] users = new String[r.possible_friends.size()];

				  Integer[] counts = new Integer[r.possible_friends.size()];

				  

				  users = r.possible_friends.toArray(users);

				  counts = r.number_of_mutual_friends.toArray(counts);

				  

				  for(int k = 0 ; k < users.length ; k++)

					  for(int j = 0; j<i ; j++)

					  {

						  if(counts[k]<counts[j])

						  {

							  String tmp3;

							  int tmp4;

							  

							  

							  tmp4 = counts[k];

							  counts[k] = counts[j];

							  counts[j] = tmp4;

							  

							  tmp3 = users[k];

							  users[k] = users[j];

							  users[j] = tmp3;

							  

						  }

						  

						  if(counts[k]>counts[j])

						  {

							  String tmp3;

							  int tmp4;

							  

							  

							  tmp4 = counts[k];

							  counts[k] = counts[j];

							  counts[j] = tmp4;

							  

							  tmp3 = users[k];

							  users[k] = users[j];

							  users[j] = tmp3;

							  

						  }

							  

					  }

					  

				  

				  for(int m = 0; m < r.possible_friends.size(); m++)

					  recommmended_friends_value += users[m] + " ";

					  

				  recommended_friends.set(recommmended_friends_value);

				  System.out.println(user_ID_key + " - " + recommmended_friends_value);

				  context.write(user_ID, recommended_friends);

			  }	

			  

			  

			  else if(r.possible_friends.size() >= 10)

			  {

				  user_ID_key = r.user;

				  user_ID.set(user_ID_key);

				  

				  String[] users = new String[r.possible_friends.size()];

				  Integer[] counts = new Integer[r.possible_friends.size()];

				  

				  users = r.possible_friends.toArray(users);

				  counts = r.number_of_mutual_friends.toArray(counts);

				  

				  for(int k = 0 ; k < users.length ; k++)

					  for(int j = 0; j<i ; j++)

					  {

						  if(counts[k]<counts[j])

						  {

							  String tmp3;

							  int tmp4;

							  

							  

							  tmp4 = counts[k];

							  counts[k] = counts[j];

							  counts[j] = tmp4;

							  

							  tmp3 = users[k];

							  users[k] = users[j];

							  users[j] = tmp3;

							  

						  }

						  

						  if(counts[k]>counts[j])

						  {

							  String tmp3;

							  int tmp4;

							  

							  

							  tmp4 = counts[k];

							  counts[k] = counts[j];

							  counts[j] = tmp4;

							  

							  tmp3 = users[k];

							  users[k] = users[j];

							  users[j] = tmp3;

							  

						  }

							  

					  }

					  

				  

				  for(int m = 0; m < 10; m++)

					  recommmended_friends_value += users[m] + " ";

					  

				  recommended_friends.set(recommmended_friends_value);

				  System.out.println(user_ID_key + " - "+  recommmended_friends_value);

				  context.write(user_ID, recommended_friends);

			  }	

			    

			  

		  }

		      

	    

	  }

	  */

	  

}

  

  

  

  

  

  

  

  

  

  

  






*/