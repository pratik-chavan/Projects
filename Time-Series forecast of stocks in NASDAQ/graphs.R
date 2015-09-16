



lm = matrix(NA,1,10)
lm_stock = c()

lm_stock[1] = "GRVY"
lm_stock[2] = "ELON"
lm_stock[3] = "SMSI"
lm_stock[4] = "PCTI"
lm_stock[5] = "BYFC"
lm_stock[6] = "LFVN"
lm_stock[7] = "TAIY"
lm_stock[8] = "STB"
lm_stock[9] = "STLY"
lm_stock[10] = "BLRX"



lm[1,1] = 0.1130815
lm[1,2] = 0.1708333
lm[1,3] = 0.2753055
lm[1,4] = 0.3298646
lm[1,5] = 0.3330336
lm[1,6] = 0.3604605
lm[1,7] = 0.3694651
lm[1,8] = 0.3798104
lm[1,9] = 0.3906403
lm[1,10] = 0.4163314
  


#plot(lm[1,1:10], xaxt = "n" , col = "blue", xlab = "Stock", ylab = "Sum of MAE", main = "Linear-Regression")
#lines(lm[1,1:10], lw = 2, col = "red")
#axis(1, at=1:10, labels=lm_stock[1:10])








hw = matrix(NA,1,10)
hw_stock = c()

hw_stock[1] = "EDS"
hw_stock[2] = "VLYWW"
hw_stock[3] = "IKAN"
hw_stock[4] = "JOEZ"
hw_stock[5] = "APWC"
hw_stock[6] = "MTSL"
hw_stock[7] = "COCO"
hw_stock[8] = "HNSN"
hw_stock[9] = "TINY"
hw_stock[10] = "IBCA"



hw[1,1] = 0.06022709
hw[1,2] = 0.09000000
hw[1,3] = 0.09451631
hw[1,4] = 0.09452480
hw[1,5] = 0.09639256
hw[1,6] = 0.11008672
hw[1,7] = 0.11565898
hw[1,8] = 0.12703413
hw[1,9] = 0.13458633
hw[1,10] = 0.13481835



#plot(hw[1,1:10], xaxt = "n" , col = "blue", xlab = "Stock", ylab = "Sum of MAE", main = "Holt-Winters")
#lines(hw[1,1:10], lw = 2, col = "red")
#axis(1, at=1:10, labels=hw_stock[1:10])










arima = matrix(NA,1,10)
arima_stock = c()

arima_stock[1] = "COCO"
arima_stock[2] = "APWC"
arima_stock[3] = "FREE"
arima_stock[4] = "IKAN"
arima_stock[5] = "SPU"
arima_stock[6] = "ELON"
arima_stock[7] = "VLYWW"
arima_stock[8] = "MFI"
arima_stock[9] = "ENZN"
arima_stock[10] = "MTSL"



arima[1,1] = 0.04291029
arima[1,2] = 0.06308866
arima[1,3] = 0.07480337
arima[1,4] = 0.09000000
arima[1,5] = 0.09000000
arima[1,6] = 0.11315610
arima[1,7] = 0.11343679
arima[1,8] = 0.12000000
arima[1,9] = 0.12096462
arima[1,10] = 0.12583623



plot(arima[1,1:10], xaxt = "n" , col = "blue", xlab = "Stock", ylab = "Sum of MAE", main = "ARIMA")
lines(arima[1,1:10], lw = 2, col = "red")
axis(1, at=1:10, labels=arima_stock[1:10])
