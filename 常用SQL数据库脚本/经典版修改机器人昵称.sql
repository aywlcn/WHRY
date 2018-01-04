
/*----------------------------------------------------------------*/
create table #t(username nvarchar(50)) 
bulk insert #t from 'E:\经典捕鱼\常用SQL数据库脚本\2万用户昵称\15023个昵称.txt' /*机器人名字文本路径*/
with 
( 
rowterminator='\n' 
) 
SELECT id=identity(int,1,1), t.username INTO #regtemp11 FROM #t t ORDER BY id DESC 

/*----------------------------------------------------------------*/
declare @i int,@Count int 
set @i=1 
select @count=count(*) from #regtemp11 
while( @i<@count ) 
BEGIN 
update  THAccountsDB.dbo.AccountsInfo set THAccountsDB.dbo.AccountsInfo.NickName=t.username 
from THAccountsDB.dbo.AccountsInfo,#regtemp11 t 
WHERE UserID IN(SELECT TOP (@i) UserID FROM THAccountsDB.dbo.AccountsInfo) 
AND THAccountsDB.dbo.AccountsInfo.IsAndroid=1
AND THAccountsDB.dbo.AccountsInfo.UserID NOT IN(SELECT TOP (@i-1) ISNULL(UserID,0)  
FROM THAccountsDB.dbo.AccountsInfo) 
--AND THAccountsDB.dbo.AccountsInfo.IsRobot=1 --caihongkaifa
AND t.id=@i 
set @i=@i+1 
END 
DROP TABLE #t 
DROP TABLE #regtemp11

