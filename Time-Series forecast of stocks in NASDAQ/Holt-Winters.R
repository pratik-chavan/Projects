# =====================================================================
# CSE487/587
# Author: your name
# Email: UBID@buffalo.edu
# =====================================================================

# need to install the following two packages in CCR(at least)

# data path /gpfs/courses/cse587/spring2015/data/hw2/data

library(forecast)
library(fpp)

val = c()
stock = c()

# need to read the stocklist, and loop all files
### TO DO
stocklist_file = "/gpfs/courses/cse587/spring2015/data/hw2/stocklist.txt"
stocklist = read.table(stocklist_file , sep = "\n")


for (i in 1:2970)
{
  filename = ""
  #filename = paste("E:/mapreduce/bin/small/" , stocklist[i,1] , sep='')
  filename = paste("/gpfs/courses/cse587/spring2015/data/hw2/data/" , stocklist[i,1] , sep='')
  filename = paste( filename , ".csv" , sep='')
  # if file is not empty
   if(file.info(filename)[1]>0 && !(stocklist[i,1]=="JASOLtd.") && !(stocklist[i,1]=="ULTAInc.")) {
 
    # read one csv file into variable (DO NOT EDIT)
    textData=read.csv(file=filename, header=T)
    
    if(nrow(textData) > 753 )
    {
      
      # convert txt data to time-series data, in day unit (DO NOT EDIT)
      tsData = ts(rev(textData$Adj.Close),start=c(2012, 1),frequency=365)
      
      # define train data (DO NOT EDIT)
      trainData = window(tsData, end=c(2014,14))
      
      # define test data (DO NOT EDIT)
      testData = window(tsData, start=c(2014,15))
      
      # MAE row vector (DO NOT EDIT)
      MAE = matrix(NA,1,length(testData))
      
      # apply Holt-Winters model (DO NOT EDIT)
      fitData = HoltWinters(trainData, gamma = FALSE )
      
      
      # the other two models
      ### TO DO
      
      # apply forecast(DO NOT EDIT)
      forecastData = forecast(fitData, h=length(testData))
      
      # print variable and see what is in the result data set
      #print(forecastData)
      #print(stocklist[i,1])  
      
      # calculate Mean Absolute Error 
      for(i1 in 1:length(testData))
      {
        MAE[1,i1] = abs(forecastData$mean[i1] - testData[i1])
      }
      
      # this is the result you need for stock AAPL
      val = c(val , sum(MAE[1,1:10]))
      st = ""
      st = paste(st , stocklist[i,1], sep='')  
      stock = c(stock , st)
      
      #print(sum(MAE[1,1:10]))
      
      # plot the top 10 minimum sum of MAE in 3 models respectively
      #plot(MAE[1,1:10], col = "blue")
      #lines(MAE[1,1:10], lw = 2, col = "red")
      ### TO DO
    }
  }
}

#hw = sort(hw , decreasing = FALSE)
#print(hw[1:10])

final_df = data.frame(val , stock)
#hw_final = sort(hw_final , decreasing = FALSE)
final = final_df[order(val),] 
print(final[1:10,])

