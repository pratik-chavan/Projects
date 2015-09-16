stock = LOAD 'hdfs:///pigdata' USING PigStorage(',','-tagFile') as (name:chararray, dt:chararray, a,b,c,d,e, Adj_Close:float);

stock2 = FOREACH stock GENERATE $0, $1, $7;

stock5 = FOREACH stock2 GENERATE $0,STRSPLIT($1, '-' , 3), $2;

stock5 = FOREACH stock5 GENERATE $0,flatten($1),$2; 

stock18 = stock5;

stock121 = FOREACH stock5 GENERATE $0,(int)(chararray)$1 , (int)(chararray)$2, (int)(chararray)$3, (float)$4;

stock18 = stock121;

stock19 = GROUP stock18 BY ($0,$1,$2);

stock20 = FOREACH stock19 GENERATE $0, MIN($1.$3), MAX($1.$3), $0.$1, $0.$2,  flatten($1.$4);

stock22 = FOREACH stock18 GENERATE ($0, $3, $2, $1), $4; 

stock23 = FOREACH stock20 GENERATE ($0.$0, $1, $4, $3); 
stock24 = DISTINCT stock23;

stock21 = FOREACH stock20 GENERATE ($0.$0 ,$2, $4, $3); 
stock26 = DISTINCT stock21;

stock25end = JOIN stock22 BY $0, stock26 BY $0;
stock25begin = JOIN stock22 BY $0, stock24 BY $0;

stock25end = FOREACH stock25end GENERATE $0,$1;
stock25begin = FOREACH stock25begin GENERATE $0, $1;

begin = FOREACH stock25begin GENERATE ($0.$0, $0.$2, $0.$3), $1;

end = FOREACH stock25end GENERATE ($0.$0, $0.$2, $0.$3), $1;

monthly = JOIN begin by $0, end by $0;

monthly = FOREACH monthly GENERATE $0 as month_year, $1 as begin, $3 as end;

monthly = FOREACH monthly GENERATE month_year, (end - begin)/begin as monthly_ROR;

mt = GROUP monthly BY $0.$0;
month = FOREACH mt GENERATE monthly.month_year as month_year, AVG(monthly.monthly_ROR) as average;
month = FOREACH month GENERATE flatten(month_year), average;

m = JOIN monthly by $0, month by $0;
m = FOREACH m GENERATE $0 as month_year, $1 as monthly_ROR, $3 as average;
monthly = m;

monthly = FOREACH monthly GENERATE month_year, monthly_ROR, average, (monthly_ROR-average)*(monthly_ROR-average) as x;

mt = GROUP monthly BY $0.$0;

month = FOREACH mt GENERATE monthly.month_year, SUM(monthly.x)/(COUNT(monthly.month_year)-1) as vol;
month = FOREACH month GENERATE flatten($0), vol;

m = JOIN monthly by $0, month by $0;
monthly = m;

monthly = FOREACH monthly GENERATE $0.$0, $5 as v;

monthly = DISTINCT monthly;
monthly = FOREACH monthly GENERATE $0, SQRT($1) as vol;

m1 = ORDER monthly BY vol;

describe m1
monthly = FILTER m1 BY $1 > 0;

biggest = RANK monthly BY $1 DESC;
smallest = RANK monthly BY $1;

biggest = FILTER biggest BY $0< 11;
smallest = FILTER smallest BY $0< 11;

biggest = FOREACH biggest GENERATE $1, $2;
smallest = FOREACH smallest GENERATE $1, $2;

dump biggest
dump smallest

STORE biggest into 'hdfs:///pigdata/hw3_out/biggest';
STORE smallest into 'hdfs:///pigdata/hw3_out/smallest';

