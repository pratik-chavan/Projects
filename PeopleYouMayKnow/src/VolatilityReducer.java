import java.io.IOException;
import java.util.*;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;


 class VolatilityReducer
  extends Reducer<Text, Text, Text, Text> {
	
	HashMap<String, Integer> h = new HashMap<String , Integer>();
	String[] users_2;
	Integer[] counts;
	
	public static <K, V extends Comparable<? super V>> Map<K, V> 
    sortByValue( Map<K, V> map )
{
    List<Map.Entry<K, V>> list =
        new LinkedList<>( map.entrySet() );
    Collections.sort( list, new Comparator<Map.Entry<K, V>>()
    {
        @Override
        public int compare( Map.Entry<K, V> o1, Map.Entry<K, V> o2 )
        {
            return (o1.getValue()).compareTo( o2.getValue() );
        }
    } );

    Map<K, V> result = new LinkedHashMap<>();
    for (Map.Entry<K, V> entry : list)
    {
        result.put( entry.getKey(), entry.getValue() );
    }
    return result;
}
	
	
	
	
	
	
	
  @Override
  public void reduce(Text key, Iterable<Text> values,
      Context context)
      throws IOException, InterruptedException {

		h.clear();
		String usr = key.toString();
		String arr = values.toString();
	  int cnt = 0;
	  int counter = 0;
	  int counter2 = 0;
	  
	  LinkedList<String> v = new LinkedList<String>(); 
	  
	  for(Text value: values)
	  {
		  v.add(value.toString());
	  }
	  
	  String tp = "";
	  //boolean is_friend = false;
	  
	  for(String value: v)
	  {
		  counter2 += 1;
	  }
	  
	  for(String value: v)
	  {
		  //System.out.println(counter2);
		  String c = value.split(",")[1];
		  String friend = value.toString().split(",")[0];
		  
		  int temp = 1;
		  
		  if(c.equals("-1"))
		  {
			  return;
		  }
		  
		  if(h.containsKey(friend))
		  {
			  int count_temp = h.get(friend);
			  h.put(friend, count_temp + 1);
		  }
		  else
		  {
			  h.put(friend, 1); 
		  }
		  //System.out.println("  Value - " + h.get(friend)); // works fine till here
		  //System.out.println("User key - " + usr + "  Friend to add in HashMap - " + friend + "  Count - " + h.get(friend) + " hashmap size - " + h.size());
		  cnt++;
		  //System.out.println("Size of HashMap : " + h.size());
		  counter++;
		 if(cnt == 10)
		 {
			 tp = value.toString();
			 //System.out.println("In If"); 
			 
		 }
		  if(counter == counter2 - 1 )
		  {
			  users_2 = new String[h.size()];
			  counts = new Integer[h.size()];
		  
			  
			  
			  Iterator it = h.entrySet().iterator();
			  int ind = 0;
			  for (String name: h.keySet()){
			        //Map.Entry pair = (Map.Entry)it.next();
			//        System.out.println(name + " -> " + h.get(name));
			        users_2[ind] = name;
			        counts[ind] =  (Integer) h.get(name);
			        //it.remove(); // avoids a ConcurrentModificationException
			        ind++;
			    }
			  
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
			  
			  	if(users_2.length > 0 && users_2.length < 10)
			  		for(int i = 0; i < users_2.length; i++)
			  			recommended_friends += users_2[i]+ ",";
			  	else
			  		for(int i = 0; i < 10; i++)
			  			recommended_friends += users_2[i]+ ",";
			  	Text user_ID = new Text(usr);
			  	Text recommendations = new Text(recommended_friends);
			  	
			  	context.write( user_ID, recommendations);
			 
		  }
		  
	  
	 
	  /*
	  
	  users_2 = new String[h.size()];
	  counts = new Integer[h.size()];
	  System.out.println("Size of HashMap : " + h.size());
	  //System.out.println("Users size - " + users_2.length);
	  //System.out.println("Counts size - " + counts.length);
	  
	  /*
	  Iterator it = h.entrySet().iterator();
	  int ind = 0;
	  for (String name: h.keySet()){
	        //Map.Entry pair = (Map.Entry)it.next();
	        System.out.println(name + " -> " + h.get(name));
	        //users_2[ind] = name;
	        //counts[ind] =  (Integer) h.get(name);
	        //it.remove(); // avoids a ConcurrentModificationException
	        ind++;
	    }
	    
	  /*
	  for(int a = 0 ; a < users.size(); a++)
	  {  System.out.print(" In Reducer - " + users.get(a) + " ");
		  System.out.println(count.get(a));
	  }
	 
	  for(int a = 0 ; a < users_2.length; a++)
	  {  System.out.print(" In Reducer - " + users_2[a] + " ");
		  System.out.println(counts[a]);
	  }
	  
	   
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
	  
	  	if(users_2.length > 0 && users_2.length < 10)
	  		for(int i = 0; i < users_2.length; i++)
	  			recommended_friends += users_2[i]+ ",";
	  	else
	  		for(int i = 0; i < 10; i++)
	  			recommended_friends += users_2[i]+ ",";
	  	Text user_ID = new Text(usr);
	  	Text recommendations = new Text(recommended_friends);
	  	
	  	context.write( user_ID, recommendations);
	  
	  
	  */
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
  
 }
  
  
  
  
  
  
  
  
  


