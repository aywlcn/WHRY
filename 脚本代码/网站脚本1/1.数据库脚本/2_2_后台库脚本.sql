USE [RYPlatformManagerDB]
GO
/****** Object:  Table [dbo].[Base_ModulePermission]    Script Date: 02/03/2015 14:52:35 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Base_ModulePermission](
	[ModuleID] [int] NOT NULL,
	[PermissionTitle] [nvarchar](128) NOT NULL,
	[PermissionValue] [bigint] NOT NULL,
	[Nullity] [tinyint] NOT NULL,
	[StateFlag] [int] NOT NULL,
	[ParentID] [int] NOT NULL,
 CONSTRAINT [PK_Base_ModulePermission_1] PRIMARY KEY CLUSTERED 
(
	[ModuleID] ASC,
	[PermissionValue] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'模块标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_ModulePermission', @level2type=N'COLUMN',@level2name=N'ModuleID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'模块名称' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_ModulePermission', @level2type=N'COLUMN',@level2name=N'PermissionTitle'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'禁止标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_ModulePermission', @level2type=N'COLUMN',@level2name=N'Nullity'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'父模块标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_ModulePermission', @level2type=N'COLUMN',@level2name=N'ParentID'
GO
/****** Object:  Table [dbo].[Base_RolePermission]    Script Date: 02/03/2015 14:52:35 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Base_RolePermission](
	[RoleID] [int] NOT NULL,
	[ModuleID] [int] NOT NULL,
	[ManagerPermission] [bigint] NOT NULL,
	[OperationPermission] [bigint] NOT NULL,
	[StateFlag] [int] NOT NULL,
 CONSTRAINT [PK_Base_RolePermission] PRIMARY KEY CLUSTERED 
(
	[RoleID] ASC,
	[ModuleID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_RolePermission', @level2type=N'COLUMN',@level2name=N'RoleID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'模块标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_RolePermission', @level2type=N'COLUMN',@level2name=N'ModuleID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'管理权限' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_RolePermission', @level2type=N'COLUMN',@level2name=N'ManagerPermission'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'操作权限' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_RolePermission', @level2type=N'COLUMN',@level2name=N'OperationPermission'
GO
/****** Object:  Table [dbo].[Base_Roles]    Script Date: 02/03/2015 14:52:35 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Base_Roles](
	[RoleID] [int] IDENTITY(1,1) NOT NULL,
	[RoleName] [nvarchar](128) NOT NULL,
	[Description] [nvarchar](256) NOT NULL,
 CONSTRAINT [PK_Base_Roles] PRIMARY KEY CLUSTERED 
(
	[RoleID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色流水标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Roles', @level2type=N'COLUMN',@level2name=N'RoleID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色名称' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Roles', @level2type=N'COLUMN',@level2name=N'RoleName'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色描述' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Roles', @level2type=N'COLUMN',@level2name=N'Description'
GO
/****** Object:  Table [dbo].[Base_UserRole]    Script Date: 02/03/2015 14:52:35 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Base_UserRole](
	[UserID] [int] NOT NULL,
	[RoleID] [int] NOT NULL,
 CONSTRAINT [PK_Base_UserRole_1] PRIMARY KEY CLUSTERED 
(
	[UserID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'用户标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_UserRole', @level2type=N'COLUMN',@level2name=N'UserID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_UserRole', @level2type=N'COLUMN',@level2name=N'RoleID'
GO
/****** Object:  Table [dbo].[GrantInfo]    Script Date: 02/03/2015 14:52:35 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[GrantInfo](
	[SiteID] [int] IDENTITY(1,1) NOT NULL,
	[GrantRoom] [int] NOT NULL,
	[GrantStartDate] [datetime] NOT NULL,
	[GrantEndDate] [datetime] NOT NULL,
	[GrantObjet] [nvarchar](50) NOT NULL,
	[MaxGrant] [int] NOT NULL,
	[DayMaxGrant] [int] NOT NULL,
 CONSTRAINT [PK_GrantInfo] PRIMARY KEY CLUSTERED 
(
	[SiteID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'记录标识(-1,积分房间，-2金币房间)' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantInfo', @level2type=N'COLUMN',@level2name=N'SiteID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'赠送房间(-1,金币房间;-2,积分房间;其他,GAMEID)' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantInfo', @level2type=N'COLUMN',@level2name=N'GrantRoom'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'赠送开始时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantInfo', @level2type=N'COLUMN',@level2name=N'GrantStartDate'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'赠送结束时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantInfo', @level2type=N'COLUMN',@level2name=N'GrantEndDate'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'赠送对象(0,全体用户;1,蓝钻会员;2,红钻会员;3,黄钻会员;4,紫钻会员)' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantInfo', @level2type=N'COLUMN',@level2name=N'GrantObjet'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'日赠送最大额度' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantInfo', @level2type=N'COLUMN',@level2name=N'MaxGrant'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'单日最大赠送量' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantInfo', @level2type=N'COLUMN',@level2name=N'DayMaxGrant'
GO
/****** Object:  Table [dbo].[GrantTimeCountsInfo]    Script Date: 02/03/2015 14:52:35 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[GrantTimeCountsInfo](
	[GrantID] [int] IDENTITY(1,1) NOT NULL,
	[GrantCouts] [int] NOT NULL,
	[GrantScore] [int] NOT NULL,
	[GrantGameScore] [int] NOT NULL,
	[GrantLoveliness] [int] NOT NULL,
	[GrantType] [int] NOT NULL,
	[GrantExp] [int] NOT NULL,
	[SiteID] [nchar](10) NOT NULL,
 CONSTRAINT [PK_GrantTimeCountsInfo] PRIMARY KEY CLUSTERED 
(
	[GrantID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'赠送时段' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantTimeCountsInfo', @level2type=N'COLUMN',@level2name=N'GrantCouts'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'赠送积分数目' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantTimeCountsInfo', @level2type=N'COLUMN',@level2name=N'GrantScore'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'赠送金币数目' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantTimeCountsInfo', @level2type=N'COLUMN',@level2name=N'GrantGameScore'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'赠送魅力' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantTimeCountsInfo', @level2type=N'COLUMN',@level2name=N'GrantLoveliness'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'赠送类型(0,根据在线时间;1,根据游戏时间;2,根据整体时间)' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantTimeCountsInfo', @level2type=N'COLUMN',@level2name=N'GrantType'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'增送经验' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'GrantTimeCountsInfo', @level2type=N'COLUMN',@level2name=N'SiteID'
GO
/****** Object:  Table [dbo].[SystemSecurity]    Script Date: 02/03/2015 14:52:35 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SystemSecurity](
	[RecordID] [int] IDENTITY(1,1) NOT NULL,
	[OperatingTime] [datetime] NOT NULL,
	[OperatingName] [nvarchar](50) NOT NULL,
	[OperatingIP] [nvarchar](50) NOT NULL,
	[OperatingAccounts] [nvarchar](50) NOT NULL
) ON [PRIMARY]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'操作帐号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'SystemSecurity', @level2type=N'COLUMN',@level2name=N'OperatingAccounts'
GO
/****** Object:  Table [dbo].[Base_Users]    Script Date: 2016/7/12 11:16:20 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[Base_Users](
	[UserID] [int] IDENTITY(1,1) NOT NULL,
	[Username] [nvarchar](50) NOT NULL,
	[Password] [nvarchar](50) NOT NULL,
	[RoleID] [int] NOT NULL,
	[Nullity] [tinyint] NOT NULL CONSTRAINT [DF_Base_Users_Flag]  DEFAULT ((0)),
	[PreLogintime] [datetime] NOT NULL CONSTRAINT [DF_Base_Users_PreLogintime]  DEFAULT (getdate()),
	[PreLoginIP] [nvarchar](50) NOT NULL CONSTRAINT [DF_Base_Users_PreLoginIP]  DEFAULT ('000.000.000.000'),
	[LastLogintime] [datetime] NOT NULL CONSTRAINT [DF_Base_Users_LastLogintime]  DEFAULT (getdate()),
	[LastLoginIP] [nvarchar](50) NOT NULL CONSTRAINT [DF_Base_Users_LastLoginIP]  DEFAULT ('000.000.000.000'),
	[LoginTimes] [int] NOT NULL CONSTRAINT [DF_Base_Users_LoginTimes]  DEFAULT ((0)),
	[IsBand] [int] NOT NULL CONSTRAINT [DF_Base_Users_IsBand]  DEFAULT ((1)),
	[BandIP] [nvarchar](50) NOT NULL CONSTRAINT [DF_Base_Users_BandIP]  DEFAULT ('000.000.000.000'),
	[IsAssist] [tinyint] NOT NULL CONSTRAINT [DF_Base_Users_IsAssist]  DEFAULT ((0)),
 CONSTRAINT [PK_Base_Users] PRIMARY KEY CLUSTERED 
(
	[UserID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'管理员ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Users', @level2type=N'COLUMN',@level2name=N'UserID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'帐号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Users', @level2type=N'COLUMN',@level2name=N'Username'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'密码' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Users', @level2type=N'COLUMN',@level2name=N'Password'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'角色ID' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Users', @level2type=N'COLUMN',@level2name=N'RoleID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'冻结标志 0-启用，1-冻结' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Users', @level2type=N'COLUMN',@level2name=N'Nullity'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'绑定IP 0-绑定 1-未绑定' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Users', @level2type=N'COLUMN',@level2name=N'IsBand'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否有运营助手权限(0:无权限,1:有权限)' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Users', @level2type=N'COLUMN',@level2name=N'IsAssist'
GO
/****** Object:  Table [dbo].[Base_Module]    Script Date: 2016/9/14 10:12:19 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[Base_Module](
	[ModuleID] [int] NOT NULL,
	[ParentID] [int] NOT NULL CONSTRAINT [DF_Base_Module_ParentID]  DEFAULT ((0)),
	[Title] [nvarchar](20) NOT NULL CONSTRAINT [DF_Base_Module_Title]  DEFAULT (''),
	[Link] [nvarchar](256) NOT NULL CONSTRAINT [DF_Base_Module_Link]  DEFAULT (''),
	[OrderNo] [int] NOT NULL CONSTRAINT [DF_Base_Module_OrderNo]  DEFAULT ((0)),
	[Nullity] [bit] NOT NULL,
	[IsMenu] [bit] NOT NULL CONSTRAINT [DF_Base_Module_IsMenu]  DEFAULT ((0)),
	[Description] [nvarchar](256) NOT NULL CONSTRAINT [DF_Base_Module_Description]  DEFAULT (''),
	[ManagerPopedom] [int] NOT NULL CONSTRAINT [DF_Base_Module_ManagerPopedom]  DEFAULT ((0)),
 CONSTRAINT [PK_Base_Module] PRIMARY KEY CLUSTERED 
(
	[ModuleID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'模块标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Module', @level2type=N'COLUMN',@level2name=N'ModuleID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'上级模块标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Module', @level2type=N'COLUMN',@level2name=N'ParentID'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'模块名称' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Module', @level2type=N'COLUMN',@level2name=N'Title'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'模块链接地址' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Module', @level2type=N'COLUMN',@level2name=N'Link'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'排序数值' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Module', @level2type=N'COLUMN',@level2name=N'OrderNo'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否启用 0: 启用 ; 1:禁用' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Module', @level2type=N'COLUMN',@level2name=N'Nullity'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'是否菜单 0:不是 ; 1:是' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Module', @level2type=N'COLUMN',@level2name=N'IsMenu'
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'模块说明' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Base_Module', @level2type=N'COLUMN',@level2name=N'Description'
GO
