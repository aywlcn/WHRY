

USE [RYAccountsDB]
GO
DECLARE @FirstName NVARCHAR(20)
DECLARE @Name NVARCHAR(20)
DECLARE @CountBegin INT
DECLARE @CountEnd	INT
DECLARE @FaceID INT
DECLARE @Gender INT


/*------------------------编辑区域开始----------------------------------------*/

SET @CountBegin = 40252			----生成账号的开始数
SET @CountEnd = 40262			----生成账号的结束数
SET @FirstName='Robot'		----生成账号的前缀

/*------------------------编辑区域结束----------------------------------------*/


WHILE(@CountBegin <= @CountEnd)
BEGIN
	SET @Name = ''
	SET @Name = @FirstName+LTRIM(STR(@CountBegin))
	SET @Gender = RAND()*2
	SET @FaceID= RAND()*100
	



	--机器人默认登陆游戏密码为32位大写MD5加密，当前密码为：qq525813575...
	--EXEC GSP_GP_RegisterAccounts @Name,@Name,N'',N'',N'',@FaceID,@Gender,N'',N'',N'127.0.0.1',N'80D32F3CDBC8256A46F10C650FFFB463',N''
	EXEC GSP_GP_RegisterAccounts @Name,@Name,0,N'',@FaceID,@Gender,N'',N'',0,N'127.0.0.1',N'F4B766F4C9FD6D85F9E3EC4756AC1CA0',N''

	--修改帐号为默认机器人
	UPDATE AccountsInfo SET IsAndroid=1 WHERE Accounts=@Name SET @CountBegin = @CountBegin + 1
END

