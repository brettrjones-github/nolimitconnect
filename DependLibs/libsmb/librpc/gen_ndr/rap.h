/* header auto-generated by pidl */

#ifndef _PIDL_HEADER_rap
#define _PIDL_HEADER_rap

#include <stdint.h>

#include "libcli/util/ntstatus.h"

#ifndef _HEADER_rap
#define _HEADER_rap

#define RAP_WshareEnum	( 0 )
#define RAP_WshareGetInfo	( 1 )
#define RAP_WshareSetInfo	( 2 )
#define RAP_WshareAdd	( 3 )
#define RAP_WshareDel	( 4 )
#define RAP_NetShareCheck	( 5 )
#define RAP_WsessionEnum	( 6 )
#define RAP_WsessionGetInfo	( 7 )
#define RAP_WsessionDel	( 8 )
#define RAP_WconnectionEnum	( 9 )
#define RAP_WfileEnum	( 10 )
#define RAP_WfileGetInfo	( 11 )
#define RAP_WfileClose	( 12 )
#define RAP_WserverGetInfo	( 13 )
#define RAP_WserverSetInfo	( 14 )
#define RAP_WserverDiskEnum	( 15 )
#define RAP_WserverAdminCommand	( 16 )
#define RAP_NetAuditOpen	( 17 )
#define RAP_WauditClear	( 18 )
#define RAP_NetErrorLogOpen	( 19 )
#define RAP_WerrorLogClear	( 20 )
#define RAP_NetCharDevEnum	( 21 )
#define RAP_NetCharDevGetInfo	( 22 )
#define RAP_WCharDevControl	( 23 )
#define RAP_NetCharDevQEnum	( 24 )
#define RAP_NetCharDevQGetInfo	( 25 )
#define RAP_WCharDevQSetInfo	( 26 )
#define RAP_WCharDevQPurge	( 27 )
#define RAP_WCharDevQPurgeSelf	( 28 )
#define RAP_WMessageNameEnum	( 29 )
#define RAP_WMessageNameGetInfo	( 30 )
#define RAP_WMessageNameAdd	( 31 )
#define RAP_WMessageNameDel	( 32 )
#define RAP_WMessageNameFwd	( 33 )
#define RAP_WMessageNameUnFwd	( 34 )
#define RAP_WMessageBufferSend	( 35 )
#define RAP_WMessageFileSend	( 36 )
#define RAP_WMessageLogFileSet	( 37 )
#define RAP_WMessageLogFileGet	( 38 )
#define RAP_WServiceEnum	( 39 )
#define RAP_WServiceInstall	( 40 )
#define RAP_WServiceControl	( 41 )
#define RAP_WAccessEnum	( 42 )
#define RAP_WAccessGetInfo	( 43 )
#define RAP_WAccessSetInfo	( 44 )
#define RAP_WAccessAdd	( 45 )
#define RAP_WAccessDel	( 46 )
#define RAP_WGroupEnum	( 47 )
#define RAP_WGroupAdd	( 48 )
#define RAP_WGroupDel	( 49 )
#define RAP_WGroupAddUser	( 50 )
#define RAP_WGroupDelUser	( 51 )
#define RAP_WGroupGetUsers	( 52 )
#define RAP_WUserEnum	( 53 )
#define RAP_WUserAdd	( 54 )
#define RAP_WUserDel	( 55 )
#define RAP_WUserGetInfo	( 56 )
#define RAP_WUserSetInfo	( 57 )
#define RAP_WUserPasswordSet	( 58 )
#define RAP_WUserGetGroups	( 59 )
#define RAP_WWkstaSetUID	( 62 )
#define RAP_WWkstaGetInfo	( 63 )
#define RAP_WWkstaSetInfo	( 64 )
#define RAP_WUseEnum	( 65 )
#define RAP_WUseAdd	( 66 )
#define RAP_WUseDel	( 67 )
#define RAP_WUseGetInfo	( 68 )
#define RAP_WPrintQEnum	( 69 )
#define RAP_WPrintQGetInfo	( 70 )
#define RAP_WPrintQSetInfo	( 71 )
#define RAP_WPrintQAdd	( 72 )
#define RAP_WPrintQDel	( 73 )
#define RAP_WPrintQPause	( 74 )
#define RAP_WPrintQContinue	( 75 )
#define RAP_WPrintJobEnum	( 76 )
#define RAP_WPrintJobGetInfo	( 77 )
#define RAP_WPrintJobSetInfo_OLD	( 78 )
#define RAP_WPrintJobDel	( 81 )
#define RAP_WPrintJobPause	( 82 )
#define RAP_WPrintJobContinue	( 83 )
#define RAP_WPrintDestEnum	( 84 )
#define RAP_WPrintDestGetInfo	( 85 )
#define RAP_WPrintDestControl	( 86 )
#define RAP_WProfileSave	( 87 )
#define RAP_WProfileLoad	( 88 )
#define RAP_WStatisticsGet	( 89 )
#define RAP_WStatisticsClear	( 90 )
#define RAP_NetRemoteTOD	( 91 )
#define RAP_WNetBiosEnum	( 92 )
#define RAP_WNetBiosGetInfo	( 93 )
#define RAP_NetServerEnum	( 94 )
#define RAP_I_NetServerEnum	( 95 )
#define RAP_WServiceGetInfo	( 96 )
#define RAP_WPrintQPurge	( 103 )
#define RAP_NetServerEnum2	( 104 )
#define RAP_WAccessGetUserPerms	( 105 )
#define RAP_WGroupGetInfo	( 106 )
#define RAP_WGroupSetInfo	( 107 )
#define RAP_WGroupSetUsers	( 108 )
#define RAP_WUserSetGroups	( 109 )
#define RAP_WUserModalsGet	( 110 )
#define RAP_WUserModalsSet	( 111 )
#define RAP_WFileEnum2	( 112 )
#define RAP_WUserAdd2	( 113 )
#define RAP_WUserSetInfo2	( 114 )
#define RAP_WUserPasswordSet2	( 115 )
#define RAP_I_NetServerEnum2	( 116 )
#define RAP_WConfigGet2	( 117 )
#define RAP_WConfigGetAll2	( 118 )
#define RAP_WGetDCName	( 119 )
#define RAP_NetHandleGetInfo	( 120 )
#define RAP_NetHandleSetInfo	( 121 )
#define RAP_WStatisticsGet2	( 122 )
#define RAP_WBuildGetInfo	( 123 )
#define RAP_WFileGetInfo2	( 124 )
#define RAP_WFileClose2	( 125 )
#define RAP_WNetServerReqChallenge	( 126 )
#define RAP_WNetServerAuthenticate	( 127 )
#define RAP_WNetServerPasswordSet	( 128 )
#define RAP_WNetAccountDeltas	( 129 )
#define RAP_WNetAccountSync	( 130 )
#define RAP_WUserEnum2	( 131 )
#define RAP_WWkstaUserLogon	( 132 )
#define RAP_WWkstaUserLogoff	( 133 )
#define RAP_WLogonEnum	( 134 )
#define RAP_WErrorLogRead	( 135 )
#define RAP_NetPathType	( 136 )
#define RAP_NetPathCanonicalize	( 137 )
#define RAP_NetPathCompare	( 138 )
#define RAP_NetNameValidate	( 139 )
#define RAP_NetNameCanonicalize	( 140 )
#define RAP_NetNameCompare	( 141 )
#define RAP_WAuditRead	( 142 )
#define RAP_WPrintDestAdd	( 143 )
#define RAP_WPrintDestSetInfo	( 144 )
#define RAP_WPrintDestDel	( 145 )
#define RAP_WUserValidate2	( 146 )
#define RAP_WPrintJobSetInfo	( 147 )
#define RAP_TI_NetServerDiskEnum	( 148 )
#define RAP_TI_NetServerDiskGetInfo	( 149 )
#define RAP_TI_FTVerifyMirror	( 150 )
#define RAP_TI_FTAbortVerify	( 151 )
#define RAP_TI_FTGetInfo	( 152 )
#define RAP_TI_FTSetInfo	( 153 )
#define RAP_TI_FTLockDisk	( 154 )
#define RAP_TI_FTFixError	( 155 )
#define RAP_TI_FTAbortFix	( 156 )
#define RAP_TI_FTDiagnoseError	( 157 )
#define RAP_TI_FTGetDriveStats	( 158 )
#define RAP_TI_FTErrorGetInfo	( 160 )
#define RAP_NetAccessCheck	( 163 )
#define RAP_NetAlertRaise	( 164 )
#define RAP_NetAlertStart	( 165 )
#define RAP_NetAlertStop	( 166 )
#define RAP_NetAuditWrite	( 167 )
#define RAP_NetIRemoteAPI	( 168 )
#define RAP_NetServiceStatus	( 169 )
#define RAP_NetServerRegister	( 170 )
#define RAP_NetServerDeregister	( 171 )
#define RAP_NetSessionEntryMake	( 172 )
#define RAP_NetSessionEntryClear	( 173 )
#define RAP_NetSessionEntryGetInfo	( 174 )
#define RAP_NetSessionEntrySetInfo	( 175 )
#define RAP_NetConnectionEntryMake	( 176 )
#define RAP_NetConnectionEntryClear	( 177 )
#define RAP_NetConnectionEntrySetInfo	( 178 )
#define RAP_NetConnectionEntryGetInfo	( 179 )
#define RAP_NetFileEntryMake	( 180 )
#define RAP_NetFileEntryClear	( 181 )
#define RAP_NetFileEntrySetInfo	( 182 )
#define RAP_NetFileEntryGetInfo	( 183 )
#define RAP_AltSrvMessageBufferSend	( 184 )
#define RAP_AltSrvMessageFileSend	( 185 )
#define RAP_wI_NetRplWkstaEnum	( 186 )
#define RAP_wI_NetRplWkstaGetInfo	( 187 )
#define RAP_wI_NetRplWkstaSetInfo	( 188 )
#define RAP_wI_NetRplWkstaAdd	( 189 )
#define RAP_wI_NetRplWkstaDel	( 190 )
#define RAP_wI_NetRplProfileEnum	( 191 )
#define RAP_wI_NetRplProfileGetInfo	( 192 )
#define RAP_wI_NetRplProfileSetInfo	( 193 )
#define RAP_wI_NetRplProfileAdd	( 194 )
#define RAP_wI_NetRplProfileDel	( 195 )
#define RAP_wI_NetRplProfileClone	( 196 )
#define RAP_wI_NetRplBaseProfileEnum	( 197 )
#define RAP_WIServerSetInfo	( 201 )
#define RAP_WPrintDriverEnum	( 205 )
#define RAP_WPrintQProcessorEnum	( 206 )
#define RAP_WPrintPortEnum	( 207 )
#define RAP_WNetWriteUpdateLog	( 208 )
#define RAP_WNetAccountUpdate	( 209 )
#define RAP_WNetAccountConfirmUpdate	( 210 )
#define RAP_WConfigSet	( 211 )
#define RAP_WAccountsReplicate	( 212 )
#define RAP_SamOEMChgPasswordUser2_P	( 214 )
#define RAP_NetServerEnum3	( 215 )
#define RAP_WprintDriverGetInfo	( 250 )
#define RAP_WprintDriverSetInfo	( 251 )
#define RAP_WaliasAdd	( 252 )
#define RAP_WaliasDel	( 253 )
#define RAP_WaliasGetInfo	( 254 )
#define RAP_WaliasSetInfo	( 255 )
#define RAP_WaliasEnum	( 256 )
#define RAP_WuserGetLogonAsn	( 257 )
#define RAP_WuserSetLogonAsn	( 258 )
#define RAP_WuserGetAppSel	( 259 )
#define RAP_WuserSetAppSel	( 260 )
#define RAP_WappAdd	( 261 )
#define RAP_WappDel	( 262 )
#define RAP_WappGetInfo	( 263 )
#define RAP_WappSetInfo	( 264 )
#define RAP_WappEnum	( 265 )
#define RAP_WUserDCDBInit	( 266 )
#define RAP_WDASDAdd	( 267 )
#define RAP_WDASDDel	( 268 )
#define RAP_WDASDGetInfo	( 269 )
#define RAP_WDASDSetInfo	( 270 )
#define RAP_WDASDEnum	( 271 )
#define RAP_WDASDCheck	( 272 )
#define RAP_WDASDCtl	( 273 )
#define RAP_WuserRemoteLogonCheck	( 274 )
#define RAP_WUserPasswordSet3	( 275 )
#define RAP_WCreateRIPLMachine	( 276 )
#define RAP_WDeleteRIPLMachine	( 277 )
#define RAP_WGetRIPLMachineInfo	( 278 )
#define RAP_WSetRIPLMachineInfo	( 279 )
#define RAP_WEnumRIPLMachine	( 280 )
#define RAP_I_ShareAdd	( 281 )
#define RAP_AliasEnum	( 282 )
#define RAP_WaccessApply	( 283 )
#define RAP_WPrt16Query	( 284 )
#define RAP_WPrt16Set	( 285 )
#define RAP_WUserDel100	( 286 )
#define RAP_WUserRemoteLogonCheck2	( 287 )
#define RAP_WRemoteTODSet	( 294 )
#define RAP_WprintJobMoveAll	( 295 )
#define RAP_W16AppParmAdd	( 296 )
#define RAP_W16AppParmDel	( 297 )
#define RAP_W16AppParmGet	( 298 )
#define RAP_W16AppParmSet	( 299 )
#define RAP_W16RIPLMachineCreate	( 300 )
#define RAP_W16RIPLMachineGetInfo	( 301 )
#define RAP_W16RIPLMachineSetInfo	( 302 )
#define RAP_W16RIPLMachineEnum	( 303 )
#define RAP_W16RIPLMachineListParmEnum	( 304 )
#define RAP_W16RIPLMachClassGetInfo	( 305 )
#define RAP_W16RIPLMachClassEnum	( 306 )
#define RAP_W16RIPLMachClassCreate	( 307 )
#define RAP_W16RIPLMachClassSetInfo	( 308 )
#define RAP_W16RIPLMachClassDelete	( 309 )
#define RAP_W16RIPLMachClassLPEnum	( 310 )
#define RAP_W16RIPLMachineDelete	( 311 )
#define RAP_W16WSLevelGetInfo	( 312 )
#define RAP_WserverNameAdd	( 313 )
#define RAP_WserverNameDel	( 314 )
#define RAP_WserverNameEnum	( 315 )
#define RAP_I_WDASDEnum	( 316 )
#define RAP_WDASDEnumTerminate	( 317 )
#define RAP_WDASDSetInfo2	( 318 )
#define MAX_API	( 318 )
#define RAP_GROUPNAME_LEN	( 21 )
#define RAP_USERNAME_LEN	( 21 )
#define RAP_SHARENAME_LEN	( 13 )
#define RAP_UPASSWD_LEN	( 16 )
#define RAP_SPASSWD_LEN	( 9 )
#define RAP_MACHNAME_LEN	( 16 )
#define RAP_SRVCNAME_LEN	( 16 )
#define RAP_SRVCCMNT_LEN	( 64 )
#define RAP_DATATYPE_LEN	( 10 )
#define RAP_WFileEnum2_REQ	( "zzWrLehb8g8" )
#define RAP_WFileGetInfo2_REQ	( "DWrLh" )
#define RAP_WFileClose2_REQ	( "D" )
#define RAP_NetGroupEnum_REQ	( "WrLeh" )
#define RAP_NetGroupAdd_REQ	( "WsT" )
#define RAP_NetGroupDel_REQ	( "z" )
#define RAP_NetGroupAddUser_REQ	( "zz" )
#define RAP_NetGroupDelUser_REQ	( "zz" )
#define RAP_NetGroupGetUsers_REQ	( "zWrLeh" )
#define RAP_NetGroupSetUsers_REQ	( "zWsTW" )
#define RAP_NetUserAdd2_REQ	( "WsTWW" )
#define RAP_NetUserEnum_REQ	( "WrLeh" )
#define RAP_NetUserEnum2_REQ	( "WrLDieh" )
#define RAP_NetUserGetGroups_REQ	( "zWrLeh" )
#define RAP_NetUserSetGroups_REQ	( "zWsTW" )
#define RAP_NetUserPasswordSet_REQ	( "zb16b16w" )
#define RAP_NetUserPasswordSet2_REQ	( "zb16b16WW" )
#define RAP_SAMOEMChgPasswordUser2_REQ	( "B516B16" )
#define RAP_NetUserValidate2_REQ	( "Wb62WWrLhWW" )
#define RAP_NetServerEnum2_REQ	( "WrLehDz" )
#define RAP_WserverGetInfo_REQ	( "WrLh" )
#define RAP_NetWkstatGetInfo	( "WrLh" )
#define RAP_WShareAdd_REQ	( "WsT" )
#define RAP_WShareEnum_REQ	( "WrLeh" )
#define RAP_WShareDel_REQ	( "zW" )
#define RAP_WWkstaGetInfo_REQ	( "WrLh" )
#define RAP_NetPrintQEnum_REQ	( "WrLeh" )
#define RAP_NetPrintQGetInfo_REQ	( "zWrLh" )
#define RAP_NetServerAdminCommand_REQ	( "zhrLeh" )
#define RAP_NetServiceEnum_REQ	( "WrLeh" )
#define RAP_NetServiceControl_REQ	( "zWWrL" )
#define RAP_NetServiceInstall_REQ	( "zF88sg88T" )
#define RAP_NetServiceGetInfo_REQ	( "zWrLh" )
#define RAP_NetSessionEnum_REQ	( "WrLeh" )
#define RAP_NetSessionGetInfo_REQ	( "zWrLh" )
#define RAP_NetSessionDel_REQ	( "zW" )
#define RAP_NetConnectionEnum_REQ	( "zWrLeh" )
#define RAP_NetWkstaUserLogoff_REQ	( "zzWb38WrLh" )
#define RAP_FILE_INFO_L2	( "D" )
#define RAP_FILE_INFO_L3	( "DWWzz" )
#define RAP_GROUP_INFO_L0	( "B21" )
#define RAP_GROUP_INFO_L1	( "B21Bz" )
#define RAP_GROUP_USERS_INFO_0	( "B21" )
#define RAP_GROUP_USERS_INFO_1	( "B21BN" )
#define RAP_USER_INFO_L0	( "B21" )
#define RAP_USER_INFO_L1	( "B21BB16DWzzWz" )
#define RAP_SERVER_INFO_L0	( "B16" )
#define RAP_SERVER_INFO_L1	( "B16BBDz" )
#define RAP_SERVER_INFO_L2	( "B16BBDzDDDWWzWWWWWWWB21BzWWWWWWWWWWWWWWWWWWWWWWz" )
#define RAP_SERVER_INFO_L3	( "B16BBDzDDDWWzWWWWWWWB21BzWWWWWWWWWWWWWWWWWWWWWWzDWz" )
#define RAP_SERVICE_INFO_L0	( "B16" )
#define RAP_SERVICE_INFO_L2	( "B16WDWB64" )
#define RAP_SHARE_INFO_L0	( "B13" )
#define RAP_SHARE_INFO_L1	( "B13BWz" )
#define RAP_SHARE_INFO_L2	( "B13BWzWWWzB9B" )
#define RAP_PRINTQ_INFO_L2	( "B13BWWWzzzzzWN" )
#define RAP_SMB_PRINT_JOB_L1	( "WB21BB16B10zWWzDDz" )
#define RAP_SESSION_INFO_L2	( "zzWWWDDDz" )
#define RAP_CONNECTION_INFO_L1	( "WWWWDzz" )
#define RAP_USER_LOGOFF_INFO_L1	( "WDW" )
#define RAP_WKSTA_INFO_L1	( "WDzzzzBBDWDWWWWWWWWWWWWWWWWWWWzzWzzW" )
#define RAP_WKSTA_INFO_L10	( "zzzBBzz" )
enum rap_status
#ifndef USE_UINT_ENUMS
 {
	NERR_Success=(int)(0)
}
#else
 { __donnot_use_enum_rap_status=0x7FFFFFFF}
#define NERR_Success ( 0 )
#endif
;

struct rap_group_info_1 {
	uint8_t group_name[21];
	uint8_t reserved1;
	const char * comment;/* [unique,flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
};

struct rap_user_info_1 {
	uint8_t user_name[21];
	uint8_t reserved1;
	uint8_t passwrd[16];
	uint32_t pwage;
	uint16_t priv;
	const char * home_dir;/* [unique,flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
	const char * comment;/* [unique,flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
	uint16_t userflags;
	const char * logon_script;/* [unique,flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
};

struct rap_service_info_2 {
	uint8_t service_name[16];
	uint16_t status;
	uint32_t installcode;
	uint16_t process_num;
	const char * comment;/* [unique,flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
};

struct rap_share_info_0 {
	uint8_t share_name[13];
};

struct rap_share_info_1 {
	uint8_t share_name[13];
	uint8_t reserved1;
	uint16_t share_type;
	const char * comment;/* [unique,flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
};

struct rap_share_info_2 {
	uint8_t share_name[13];
	uint8_t reserved1;
	uint16_t share_type;
	const char * comment;/* [unique,flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
	uint16_t perms;
	uint16_t maximum_users;
	uint16_t active_users;
	const char * path;/* [unique,flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
	uint8_t password[9];
	uint8_t reserved2;
};

union rap_share_info {
	struct rap_share_info_0 info0;/* [case(0)] */
	struct rap_share_info_1 info1;/* [case] */
	struct rap_share_info_2 info2;/* [case(2)] */
}/* [nodiscriminant] */;

struct rap_server_info_0 {
	uint8_t name[16];
};

struct rap_server_info_1 {
	uint8_t name[16];
	uint8_t version_major;
	uint8_t version_minor;
	uint32_t servertype;
	const char * comment;/* [unique,flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
};

union rap_server_info {
	struct rap_server_info_0 info0;/* [case(0)] */
	struct rap_server_info_1 info1;/* [case] */
}/* [nodiscriminant] */;

enum rap_PrintJStatusCode
#ifndef USE_UINT_ENUMS
 {
	PRJ_QS_QUEUED=(int)(0x0000),
	PRJ_QS_PAUSED=(int)(0x0001),
	PRJ_QS_SPOOLING=(int)(0x0002),
	PRJ_QS_PRINTING=(int)(0x0003),
	PRJ_QS_ERROR=(int)(0x0010)
}
#else
 { __donnot_use_enum_rap_PrintJStatusCode=0x7FFFFFFF}
#define PRJ_QS_QUEUED ( 0x0000 )
#define PRJ_QS_PAUSED ( 0x0001 )
#define PRJ_QS_SPOOLING ( 0x0002 )
#define PRJ_QS_PRINTING ( 0x0003 )
#define PRJ_QS_ERROR ( 0x0010 )
#endif
;

struct rap_PrintJobInfo0 {
	uint16_t JobID;
};

struct rap_PrintJobInfo1 {
	uint16_t JobID;
	const char *UserName;/* [charset(DOS)] */
	uint8_t Pad;
	const char *NotifyName;/* [charset(DOS)] */
	const char *DataType;/* [charset(DOS)] */
	const char * PrintParameterString;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintParameterStringHigh;
	uint16_t JobPosition;
	enum rap_PrintJStatusCode JobStatus;
	const char * JobStatusString;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t JobStatusStringHigh;
	time_t TimeSubmitted;
	uint32_t JobSize;
	const char * JobCommentString;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t JobCommentStringHigh;
};

struct rap_PrintJobInfo2 {
	uint16_t JobID;
	uint16_t Priority;
	const char * UserName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t UserNameHigh;
	uint16_t JobPosition;
	enum rap_PrintJStatusCode JobStatus;
	time_t TimeSubmitted;
	uint32_t JobSize;
	const char * JobCommentString;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t JobCommentStringHigh;
	const char * DocumentName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t DocumentNameHigh;
};

struct rap_PrintJobInfo3 {
	uint16_t JobID;
	uint16_t Priority;
	const char * UserName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t UserNameHigh;
	uint16_t JobPosition;
	enum rap_PrintJStatusCode JobStatus;
	time_t TimeSubmitted;
	uint32_t JobSize;
	const char * JobCommentString;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t JobCommentStringHigh;
	const char * DocumentName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t DocumentNameHigh;
	const char * NotifyName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t NotifyNameHigh;
	const char * DataType;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t DataTypeHigh;
	const char * PrintParameterString;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintParameterStringHigh;
	const char * StatusString;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t StatusStringHigh;
	const char * QueueName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t QueueNameHigh;
	const char * PrintProcessorName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintProcessorNameHigh;
	const char * PrintProcessorParams;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintProcessorParamsHigh;
	const char * DriverName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t DriverNameHigh;
	const char * DriverDataOffset;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t DriverDataOffsetHigh;
	const char * PrinterNameOffset;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrinterNameOffsetHigh;
};

union rap_printj_info {
	struct rap_PrintJobInfo0 info0;/* [case(0)] */
	struct rap_PrintJobInfo1 info1;/* [case] */
	struct rap_PrintJobInfo2 info2;/* [case(2)] */
	struct rap_PrintJobInfo3 info3;/* [case(3)] */
}/* [public,nodiscriminant] */;

enum rap_PrintQStatusCode
#ifndef USE_UINT_ENUMS
 {
	PRQ_ACTIVE=(int)(0x0000),
	PRQ_PAUSE=(int)(0x0001),
	PRQ_ERROR=(int)(0x0002),
	PRQ_PENDING=(int)(0x0003)
}
#else
 { __donnot_use_enum_rap_PrintQStatusCode=0x7FFFFFFF}
#define PRQ_ACTIVE ( 0x0000 )
#define PRQ_PAUSE ( 0x0001 )
#define PRQ_ERROR ( 0x0002 )
#define PRQ_PENDING ( 0x0003 )
#endif
;

struct rap_PrintQueue0 {
	const char *PrintQName;/* [charset(DOS)] */
};

struct rap_PrintQueue1 {
	const char *PrintQName;/* [charset(DOS)] */
	uint8_t Pad1;
	uint16_t Priority;
	uint16_t StartTime;
	uint16_t UntilTime;
	const char * SeparatorPageFilename;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t SeparatorPageFilenameHigh;
	const char * PrintProcessorDllName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintProcessorDllNameHigh;
	const char * PrintDestinationsName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintDestinationsNameHigh;
	const char * PrintParameterString;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintParameterStringHigh;
	const char * CommentString;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t CommentStringHigh;
	enum rap_PrintQStatusCode PrintQStatus;
	uint16_t PrintJobCount;
};

struct rap_PrintQueue2 {
	struct rap_PrintQueue1 queue;
	struct rap_PrintJobInfo1 *job;
};

struct rap_PrintQueue3 {
	const char * PrintQueueName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintQueueNameHigh;
	uint16_t Priority;
	uint16_t StartTime;
	uint16_t UntilTime;
	uint16_t Pad;
	const char * SeparatorPageFilename;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t SeparatorPageFilenameHigh;
	const char * PrintProcessorDllName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintProcessorDllNameHigh;
	const char * PrintParameterString;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintParameterStringHigh;
	const char * CommentString;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t CommentStringHigh;
	enum rap_PrintQStatusCode PrintQStatus;
	uint16_t PrintJobCount;
	const char * Printers;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintersHigh;
	const char * DriverName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t DriverNameHigh;
	const char * PrintDriverData;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintDriverDataHigh;
}/* [public] */;

struct rap_PrintQueue4 {
	struct rap_PrintQueue3 queue;
	struct rap_PrintJobInfo2 *job;
};

struct rap_PrintQueue5 {
	const char * PrintQueueName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintQueueNameHigh;
};

union rap_printq_info {
	struct rap_PrintQueue0 info0;/* [case(0)] */
	struct rap_PrintQueue1 info1;/* [case] */
	struct rap_PrintQueue2 info2;/* [case(2)] */
	struct rap_PrintQueue3 info3;/* [case(3)] */
	struct rap_PrintQueue4 info4;/* [case(4)] */
	struct rap_PrintQueue5 info5;/* [case(5)] */
}/* [public,nodiscriminant] */;

enum rap_JobInfoParamNum
#ifndef USE_UINT_ENUMS
 {
	RAP_PARAM_JOBNUM=(int)(0x0001),
	RAP_PARAM_USERNAME=(int)(0x0002),
	RAP_PARAM_NOTIFYNAME=(int)(0x0003),
	RAP_PARAM_DATATYPE=(int)(0x0004),
	RAP_PARAM_PARAMETERS_STRING=(int)(0x0005),
	RAP_PARAM_JOBPOSITION=(int)(0x0006),
	RAP_PARAM_JOBSTATUS=(int)(0x0007),
	RAP_PARAM_JOBSTATUSSTR=(int)(0x0008),
	RAP_PARAM_TIMESUBMITTED=(int)(0x0009),
	RAP_PARAM_JOBSIZE=(int)(0x000a),
	RAP_PARAM_JOBCOMMENT=(int)(0x000b)
}
#else
 { __donnot_use_enum_rap_JobInfoParamNum=0x7FFFFFFF}
#define RAP_PARAM_JOBNUM ( 0x0001 )
#define RAP_PARAM_USERNAME ( 0x0002 )
#define RAP_PARAM_NOTIFYNAME ( 0x0003 )
#define RAP_PARAM_DATATYPE ( 0x0004 )
#define RAP_PARAM_PARAMETERS_STRING ( 0x0005 )
#define RAP_PARAM_JOBPOSITION ( 0x0006 )
#define RAP_PARAM_JOBSTATUS ( 0x0007 )
#define RAP_PARAM_JOBSTATUSSTR ( 0x0008 )
#define RAP_PARAM_TIMESUBMITTED ( 0x0009 )
#define RAP_PARAM_JOBSIZE ( 0x000a )
#define RAP_PARAM_JOBCOMMENT ( 0x000b )
#endif
;

union rap_JobInfoParam {
	uint16_t value;/* [case(RAP_PARAM_JOBNUM)] */
	const char * string;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),case(RAP_PARAM_USERNAME)] */
	uint32_t value4;/* [case(RAP_PARAM_TIMESUBMITTED)] */
}/* [nodiscriminant] */;

struct rap_PrintDest0 {
	const char *PrintDestName;/* [charset(DOS)] */
};

struct rap_PrintDest1 {
	const char *PrintDestName;/* [charset(DOS)] */
	const char *UserName;/* [charset(DOS)] */
	uint16_t JobId;
	uint16_t Status;
	const char * StatusStringName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintQueueNameHigh;
	uint16_t Time;
};

struct rap_PrintDest2 {
	const char * PrinterName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrinterNameHigh;
};

struct rap_PrintDest3 {
	const char * PrinterName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrinterNameHigh;
	const char * UserName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t UserNameHigh;
	const char * LogAddr;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t LogAddrHigh;
	uint16_t JobId;
	uint16_t Status;
	const char * StatusStringName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t PrintQueueNameHigh;
	const char * Comment;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t CommentHigh;
	const char * Drivers;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t DriversHigh;
	uint16_t Time;
	uint16_t Pad1;
};

union rap_printdest_info {
	struct rap_PrintDest0 info0;/* [case(0)] */
	struct rap_PrintDest1 info1;/* [case] */
	struct rap_PrintDest2 info2;/* [case(2)] */
	struct rap_PrintDest3 info3;/* [case(3)] */
}/* [public,nodiscriminant] */;

struct rap_NetUserInfo0 {
	const char *Name;/* [charset(DOS)] */
};

enum rap_UserPriv
#ifndef USE_UINT_ENUMS
 {
	USER_PRIV_GUEST=(int)(0),
	USER_PRIV_USER=(int)(1),
	USER_PRIV_ADMIN=(int)(2)
}
#else
 { __donnot_use_enum_rap_UserPriv=0x7FFFFFFF}
#define USER_PRIV_GUEST ( 0 )
#define USER_PRIV_USER ( 1 )
#define USER_PRIV_ADMIN ( 2 )
#endif
;

struct rap_NetUserInfo1 {
	const char *Name;/* [charset(DOS)] */
	uint8_t Pad;
	uint8_t Password[16];
	time_t PasswordAge;
	enum rap_UserPriv Priv;
	const char * HomeDir;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t HomeDirHigh;
	const char * Comment;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t CommentHigh;
	uint16_t Flags;
	const char * ScriptPath;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t ScriptPathHigh;
};

enum rap_AuthFlags
#ifndef USE_UINT_ENUMS
 {
	AF_OP_PRINT=(int)(0),
	AF_OP_COMM=(int)(1),
	AF_OP_SERVER=(int)(2),
	AF_OP_ACCOUNTS=(int)(3)
}
#else
 { __donnot_use_enum_rap_AuthFlags=0x7FFFFFFF}
#define AF_OP_PRINT ( 0 )
#define AF_OP_COMM ( 1 )
#define AF_OP_SERVER ( 2 )
#define AF_OP_ACCOUNTS ( 3 )
#endif
;

struct rap_LogonHours {
	uint8_t LogonHours[21];
}/* [public] */;

struct rap_NetUserInfo2 {
	const char *Name;/* [charset(DOS)] */
	uint8_t Pad;
	uint8_t Password[16];
	time_t PasswordAge;
	enum rap_UserPriv Priv;
	const char * HomeDir;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t HomeDirHigh;
	const char * Comment;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t CommentHigh;
	uint16_t Flags;
	const char * ScriptPath;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t ScriptPathHigh;
	enum rap_AuthFlags AuthFlags;
	const char * FullName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t FullNameHigh;
	const char * UsrComment;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t UsrCommentHigh;
	const char * pParms;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t pParmsHigh;
	const char * WorkStations;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t WorkStationsHigh;
	time_t LastLogon;
	time_t LastLogOff;
	time_t AcctExpires;
	uint32_t MaxStorage;
	uint16_t UnitsPerWeek;
	struct rap_LogonHours *LogonHours;/* [relative_short] */
	uint16_t LogonHoursHigh;
	uint16_t BadPwCount;
	uint16_t NumLogons;
	const char * LogonServer;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t LogonServerHigh;
	uint16_t CountryCode;
	uint16_t CodePage;
};

struct rap_NetUserInfo10 {
	const char *Name;/* [charset(DOS)] */
	uint8_t Pad;
	const char * Comment;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t CommentHigh;
	const char * UsrComment;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t UsrCommentHigh;
	const char * FullName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t FullNameHigh;
};

struct rap_NetUserInfo11 {
	const char *Name;/* [charset(DOS)] */
	uint8_t Pad;
	const char * Comment;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t CommentHigh;
	const char * UsrComment;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t UsrCommentHigh;
	const char * FullName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t FullNameHigh;
	enum rap_UserPriv Priv;
	enum rap_AuthFlags AuthFlags;
	time_t PasswordAge;
	const char * HomeDir;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t HomeDirHigh;
	const char * Parms;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t ParmsHigh;
	time_t LastLogon;
	time_t LastLogOff;
	uint16_t BadPWCount;
	uint16_t NumLogons;
	const char * LogonServer;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t LogonServerHigh;
	uint16_t CountryCode;
	const char * WorkStations;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t WorkStationsHigh;
	uint32_t MaxStorage;
	uint16_t UnitsPerWeek;
	struct rap_LogonHours *LogonHours;/* [relative_short] */
	uint16_t LogonHoursHigh;
	uint16_t CodePage;
};

union rap_netuser_info {
	struct rap_NetUserInfo0 info0;/* [case(0)] */
	struct rap_NetUserInfo1 info1;/* [case] */
	struct rap_NetUserInfo2 info2;/* [case(2)] */
	struct rap_NetUserInfo10 info10;/* [case(10)] */
	struct rap_NetUserInfo11 info11;/* [case(11)] */
}/* [public,nodiscriminant] */;

struct rap_session_info_2 {
	const char * ComputerName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t ComputerNameHigh;
	const char * UserName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t UserNameHigh;
	uint16_t num_conns;
	uint16_t num_opens;
	uint16_t num_users;
	uint32_t sess_time;
	uint32_t idle_time;
	uint32_t user_flags;
	const char * CliTypeName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM),relative_short] */
	uint16_t CliTypeNameHigh;
};

union rap_session_info {
	struct rap_session_info_2 info2;/* [case(2)] */
}/* [public,nodiscriminant] */;

struct rap_TimeOfDayInfo {
	uint32_t TimeSinceJan11970;
	uint32_t TimeSinceBoot;
	uint8_t Hours;
	uint8_t Minutes;
	uint8_t Seconds;
	uint8_t Hundreds;
	uint16_t TimeZone;
	uint16_t ClockFrequency;
	uint8_t Day;
	uint8_t Month;
	uint16_t Year;
	uint8_t Weekday;
}/* [public] */;


struct rap_NetShareEnum {
	struct {
		uint16_t level;
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t count;
		uint16_t available;
		union rap_share_info *info;/* [ref,switch_is(level)] */
	} out;

};


struct rap_NetShareAdd {
	struct {
		uint16_t level;
		uint16_t bufsize;
		union rap_share_info info;/* [switch_is(level)] */
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetServerEnum2 {
	struct {
		uint16_t level;
		uint16_t bufsize;
		uint32_t servertype;
		const char * domain;/* [ref,flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t count;
		uint16_t available;
		union rap_server_info *info;/* [switch_is(level)] */
	} out;

};


struct rap_WserverGetInfo {
	struct {
		uint16_t level;
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t available;
		union rap_server_info info;/* [switch_is(level)] */
	} out;

};


struct rap_NetPrintQEnum {
	struct {
		uint16_t level;
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t count;
		uint16_t available;
		union rap_printq_info *info;/* [switch_is(level)] */
	} out;

};


struct rap_NetPrintQGetInfo {
	struct {
		const char * PrintQueueName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
		uint16_t level;
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t available;
		union rap_printq_info info;/* [switch_is(level)] */
	} out;

};


struct rap_NetPrintJobPause {
	struct {
		uint16_t JobID;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetPrintJobContinue {
	struct {
		uint16_t JobID;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetPrintJobDelete {
	struct {
		uint16_t JobID;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetPrintQueuePause {
	struct {
		const char * PrintQueueName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetPrintQueueResume {
	struct {
		const char * PrintQueueName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetPrintQueuePurge {
	struct {
		const char * PrintQueueName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetPrintJobEnum {
	struct {
		const char * PrintQueueName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
		uint16_t level;
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t count;
		uint16_t available;
		union rap_printj_info *info;/* [switch_is(level)] */
	} out;

};


struct rap_NetPrintJobGetInfo {
	struct {
		uint16_t JobID;
		uint16_t level;
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t available;
		union rap_printj_info info;/* [switch_is(level)] */
	} out;

};


struct rap_NetPrintJobSetInfo {
	struct {
		uint16_t JobID;
		uint16_t level;
		uint16_t bufsize;
		enum rap_JobInfoParamNum ParamNum;
		union rap_JobInfoParam Param;/* [switch_is(ParamNum)] */
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetPrintDestEnum {
	struct {
		uint16_t level;
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t count;
		uint16_t available;
		union rap_printdest_info *info;/* [switch_is(level)] */
	} out;

};


struct rap_NetPrintDestGetInfo {
	struct {
		const char * PrintDestName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
		uint16_t level;
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t available;
		union rap_printdest_info info;/* [switch_is(level)] */
	} out;

};


struct rap_NetUserPasswordSet2 {
	struct {
		const char * UserName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
		uint8_t OldPassword[16];
		uint8_t NewPassword[16];
		uint16_t EncryptedPassword;
		uint16_t RealPasswordLength;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetOEMChangePassword {
	struct {
		const char * UserName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
		uint8_t crypt_password[516];
		uint8_t password_hash[16];
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetUserGetInfo {
	struct {
		const char * UserName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
		uint16_t level;
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t available;
		union rap_netuser_info info;/* [switch_is(level)] */
	} out;

};


struct rap_NetSessionEnum {
	struct {
		uint16_t level;
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t count;
		uint16_t available;
		union rap_session_info *info;/* [switch_is(level)] */
	} out;

};


struct rap_NetSessionGetInfo {
	struct {
		const char * SessionName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
		uint16_t level;
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		uint16_t available;
		union rap_session_info info;/* [switch_is(level)] */
	} out;

};


struct rap_NetUserAdd {
	struct {
		uint16_t level;
		uint16_t bufsize;
		uint16_t pwdlength;
		uint16_t unknown;
		union rap_netuser_info info;/* [switch_is(level)] */
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetUserDelete {
	struct {
		const char * UserName;/* [flag(LIBNDR_FLAG_STR_ASCII|LIBNDR_FLAG_STR_NULLTERM)] */
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
	} out;

};


struct rap_NetRemoteTOD {
	struct {
		uint16_t bufsize;
	} in;

	struct {
		enum rap_status status;
		uint16_t convert;
		struct rap_TimeOfDayInfo tod;
	} out;

};

#endif /* _HEADER_rap */
#endif /* _PIDL_HEADER_rap */
