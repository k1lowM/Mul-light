/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/ATA/ATA_CmdPublish.cpp
	概要		：
	詳細		：
	責任者		：	牧内 恵一朗
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"../../Common.h"
#include	"../DM.h"
#include	"../PCI/PCI.h"
#include	"../../TM/TM.h"
#include	"ATA.h"
#include	"BMIDE.h"

//デバッグ用
#ifdef DP_DEBUG
	#include	<stdio.h>
	#include	"../../GUI/GUI.h"
#endif

#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ATAクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	IDENTIFY DEVICE/IDENTIFY PACKET DEVICEコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択 0 or 1
				s4_Flg			ATA or ATAPIフラグ
				Pu2_IDdata		IDENTIFY情報格納バッファ
	戻り値	：	0>				コマンド実行前/中エラー
				0				正常終了
				0<				コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::IdentifyDevice( u4 u4_Device, s4 s4_Flg, u2 *Pu2_IDdata )
{
	ATA_Cmd	AtaCmd = { 0 };	//構造体のゼロ埋め
	s4		s4_Return;

	// デバイス/ヘッドレジスタ
	AtaCmd.u1_DevHed =  ( u4_Device << 4 );

	//ATAディバイス
	if( s4_Flg == DEVICE_ATA )
	{
		if( (M_DevInfo[u4_Device].u4_DeviceType & DEVICE_MASK) == DEVICE_NON )	//未接続
			AtaCmd.u4_Flag = 0;		// ディバイス判定のためチェック不要
		else
			AtaCmd.u4_Flag = 0x40;	// DRDYビットチェック

		AtaCmd.u1_Command = 0xec;	//IDENTIFY DEVICEコマンド発行
#ifdef DP_DEBUG
		DP(" *IdentifyDevice* DEVICE_ATA");
#endif
	}
	//ATAPIデバイス
	else
	{
		AtaCmd.u4_Flag = 0;			//DRDYビットチェック不要
		AtaCmd.u1_Command = 0xa1;	//IDENTIFY PACKET DEVICEコマンド発行
#ifdef DP_DEBUG
		DP(" *IdentifyDevice* DEVICE_ATAPI");
#endif
	}

	//１ブロック(256ワード)リード
	s4_Return = PIO_DataIn_CMD( &AtaCmd, 1, (void *) Pu2_IDdata );

#ifdef DP_DEBUG
	DP(" *IdentifyDevice* PIO_DataIn_CMD");
#endif

	if( s4_Return == ERROR_CMD )
		return	ERROR_NODEVICE;				//ディバイス未接続

	//エラー
	if( s4_Return < 0 )
	{
		//コマンドエラー
		if( ( s4_Return & (ERRORREGI_MASK | 4) ) != 0 )
			return	ERROR_ABORT;			//アボートエラー
		else
			return	ERROR;					//アボート以外のエラー
	}


	return SUCCESS;										//正常終了
}

/*******************************************************************************
	概要	：	INITALIZE DEVICE PARAMATERSコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択 0 or 1
				u1_head			ヘッド数
				u1_MaxSector	セクタ数
	戻り値	：	0>				コマンド実行前/中エラー
				0				正常終了
				0<				コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::InitDevParamaters( u4 u4_Device, u1 u1_Head, u1 u1_MaxSector )
{
	ATA_Cmd	AtaCmd = { 0 };

	AtaCmd.u1_Feature = 0xff;							//(フィーチャー)
	AtaCmd.u1_SecCnt = u1_MaxSector;					//最大セクタ数
	AtaCmd.u1_DevHed =  u4_Device << 4 | ( u1_Head & 0xf );	//デバイス/ヘッド数
	AtaCmd.u1_Command = 0x91;							//INITALIZE DEVICE PARAMATERSコマンド

	return NonData_CMD( &AtaCmd );				//正常終了時=0 エラー終了時!=0
}

/*******************************************************************************
	概要	：	IDLE/IDLE IMMEDIATEコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択 0 or 1
				s4_flg			ATA or ATAPIフラグ
	戻り値	：	0>				コマンド実行前/中エラー
				0				正常終了
				0<				コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::IdleDevice( u4 u4_Device ,u4 DevFlg )
{
	ATA_Cmd	AtaCmd = { 0 };

	AtaCmd.u1_Feature = 0xff;			//(フィーチャー)
	AtaCmd.u1_DevHed =  u4_Device << 4;	//デバイス/ヘッド数
	AtaCmd.u4_Flag = 0x40;				//DRDYビットチェック

	if( DevFlg & DEVICE_ATA )
		AtaCmd.u1_Command = 0xe3;			//ATAデバイス用 IDLEコマンド
	else
		AtaCmd.u1_Command = 0xe1;			//ATAPIデバイス用 IDLEコマンド

	return	NonData_CMD( &AtaCmd );	//正常終了時=0 エラー終了時!=0
}


/*******************************************************************************
	概要	：	SET FEATURESコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択 0 or 1
				u1_subcom		サブコマンド
				u1_mode			転送モード値ほか
	戻り値	：	0>				コマンド実行前/中エラー
				0				正常終了
				0<				コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::SetFeatures( u4 u4_Device, u1 u1_subcom, u1 u1_mode )
{
	ATA_Cmd	AtaCmd = { 0 };

	AtaCmd.u1_Feature = u1_subcom;			//SET FEATURES sub command
	AtaCmd.u1_SecCnt = u1_mode;				//データ転送モード値ほか
	AtaCmd.u1_DevHed =  u4_Device << 4;		//デバイス/ヘッド数
	AtaCmd.u1_Command = 0xef;				//SET FEATURESコマンド
	AtaCmd.u4_Flag = 0x40;					//DRDYビットチェック

	return NonData_CMD( &AtaCmd );
}


/*******************************************************************************
	概要	：	GET MEDIA STATUSコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択 0 or 1
	戻り値	：	0>				コマンド実行前/中エラー
				0				正常終了
				0<				コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::GetMediaStatus( u4 u4_Device )
{
	ATA_Cmd	AtaCmd = { 0 };

	AtaCmd.u1_DevHed =  u4_Device << 4;	//デバイス
	AtaCmd.u1_Command = 0xda;				//GET MEDIA STATUSコマンド
	AtaCmd.u4_Flag = 0x40;				//DRDYビットチェック

	return NonData_CMD( &AtaCmd );	//正常終了時=0 エラー終了時!=0
}


/*******************************************************************************
	概要	：	CHECK POWER MODEコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択 0 or 1
	戻り値	：	0>				コマンド実行前/中エラー
				0				正常終了
				0<				コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::ChkPowerMode( u4 u4_Device )
{
	ATA_Cmd	AtaCmd = { 0 };

	AtaCmd.u1_DevHed =  u4_Device << 4;	//デバイス
	AtaCmd.u1_Command = 0xe5;				//GET MEDIA STATUSコマンド

	return NonData_CMD( &AtaCmd );	//正常終了時=0 エラー終了時!=0
}


/*******************************************************************************
	概要	：	DEVICE RESETコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択 0 or 1
	戻り値	：	0>				コマンド実行前/中エラー
				0				正常終了
				0<				コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::DeviceReset( u4 u4_Device )
{
	ATA_Cmd	AtaCmd = { 0 };

	AtaCmd.u1_DevHed =  u4_Device << 4;	//デバイス
	AtaCmd.u1_Command = 0x08;				//GET MEDIA STATUSコマンド

	return NonData_CMD( &AtaCmd );	//正常終了時=0 エラー終了時!=0
}


/*******************************************************************************
	概要	：	FLUSH CHACEコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択 0 or 1
	戻り値	：	0>				コマンド実行前/中エラー
				0				正常終了
				0<				コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::FlushChack( u4 u4_Device )
{
	ATA_Cmd	AtaCmd = { 0 };

	AtaCmd.u1_DevHed =  u4_Device << 4;	//デバイス
	AtaCmd.u1_Command = 0xe7;				//GET MEDIA STATUSコマンド

	return NonData_CMD( &AtaCmd );	//正常終了時=0 エラー終了時!=0
}


/*******************************************************************************
	概要	：	SLEEPコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択 0 or 1
	戻り値	：	0>				コマンド実行前/中エラー
				0				正常終了
				0<				コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::Sleep( u4 u4_Device )
{
	ATA_Cmd	AtaCmd = { 0 };

	AtaCmd.u1_DevHed =  u4_Device << 4;	//デバイス
	AtaCmd.u1_Command = 0xe6;				//GET MEDIA STATUSコマンド

	return NonData_CMD( &AtaCmd );	//正常終了時=0 エラー終了時!=0
}


/*******************************************************************************
	概要	：	EXECUTE DEVICE DIAGNOSTICコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択 0 or 1
	戻り値	：	0>				コマンド実行前/中エラー
				0				正常終了
				0<				コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::ExeDevDiagnostic( void )
{
	ATA_Cmd	AtaCmd = { 0 };

	AtaCmd.u1_Command = 0x90;				//GET MEDIA STATUSコマンド

	return NonData_CMD( &AtaCmd );	//正常終了時=0 エラー終了時!=0
}


/*******************************************************************************
	概要	：	STANDBY IMMEDIATEコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択 0 or 1
	戻り値	：	0>				コマンド実行前/中エラー
				0				正常終了
				0<				コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::StandbyImmediate( u4 u4_Device )
{
	ATA_Cmd	AtaCmd = { 0 };

	AtaCmd.u1_Command = 0xe0;				//GET MEDIA STATUSコマンド

	return NonData_CMD( &AtaCmd );	//正常終了時=0 エラー終了時!=0
}


/*****************************************************************************
	概要	:	START/STOP UNIT(START)パケットコマンド発行
	説明	:
	Include	:	ATA.h
	引数	:	u4_Device	デバイス選択 0 or 1
				u1_Data		スタート/イジェクト/トレイクローズ
	戻り値	:	0>			コマンド実行前/中エラー
				0			正常終了
				0<			コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*******************************************************************************/
s4		ATA::PacketStartUnit(u4 u4_Device, u1 u1_Data)
{
	ATAPI_Cmd	AtapiCmd;

	//ゼロ埋め
	memset( AtapiCmd.u1_CmdPacket, 0, 12 );

	AtapiCmd.u1_Feature = 0;				//Non-overlap/NonDMA
	AtapiCmd.u1_DevHed = u4_Device << 4 ;	//デバイス
	AtapiCmd.u1_CmdPacket[0] = 0x1b;		//START UNIT
	AtapiCmd.u1_CmdPacket[4] = u1_Data;		//設定値

	return	PacketPIO_CMD( &AtapiCmd, 0, NULL );		//正常終了時=0 エラー終了時!=0
}


/*********************************************************************
	概要	:	REQUEST SENSEコマンド発行
	説明	:
	Include	:	ATA.h
	引数	:	u4_Device	デバイス選択 0 or 1
				u1_Data		スタート/イジェクト/トレイクローズ
	戻り値	:	0>     : コマンド実行前/中エラー
				0      : 正常終了
				0<     : コマンド実行後エラー(下位8ビット Errorレジスタの内容)
*********************************************************************/
s4		ATA::PacketRequestSense(u4 u4_Device, void* Pv_Buff)
{
	s4			s4_Return;
	ATAPI_Cmd	AtapiCmd;

	//ゼロ埋め
	memset( AtapiCmd.u1_CmdPacket, 0, 12 );

	AtapiCmd.u1_Feature = 0;			//Non-overlap/NonDMA
	AtapiCmd.u1_DevHed = u4_Device << 4 ;	//デバイス
	AtapiCmd.u1_CmdPacket[0] = 3;		//REQUEST SENSE
	AtapiCmd.u1_CmdPacket[4] = 18;	//18バイト

	s4_Return = PacketPIO_CMD( &AtapiCmd, 18, Pv_Buff );
	if( s4_Return == 18 )
		return SUCCESS;				//正常終了時=0
	else
		return s4_Return;					//エラー終了時!=0
}


/*******************************************************************************
	概要	：	READ (10)パケットコマンド発行
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device		デバイス選択 0 or 1
				ui_LBA			LBA
				*Pv_buff		データ格納バッファポインタ
				ui_NumSector	セクタ数
	戻り値	：	s4		エラー情報(正数:読み取ったセクタ数)
*******************************************************************************/
s4		ATA::PacketRead10( u4 u4_Device, ui ui_LBA, void *Pv_buff, ui ui_NumSector )
{
	ATAPI_Cmd	AtapiCmd;
//	s4			s4_Return;

	//ゼロ埋め
	memset( AtapiCmd.u1_CmdPacket, 0, 12 );

	AtapiCmd.u1_DevHed = u4_Device << 4 ;	//デバイス
	AtapiCmd.u1_CmdPacket[0] = 0x28;		//READ(10)
	AtapiCmd.u1_CmdPacket[2] = 0xff & (ui_LBA >> 24);	//論理セクタ
	AtapiCmd.u1_CmdPacket[3] = 0xff & (ui_LBA >> 16);
	AtapiCmd.u1_CmdPacket[4] = 0xff & (ui_LBA >> 8);
	AtapiCmd.u1_CmdPacket[5] = 0xff & ui_LBA;
	AtapiCmd.u1_CmdPacket[7] = 0xff & (ui_NumSector >> 8);	//読み出しセクタ数
	AtapiCmd.u1_CmdPacket[8] = 0xff & ui_NumSector;


	AtapiCmd.u1_Feature = 0;	//Non-overlap/NonDMA
	return PacketPIO_CMD( &AtapiCmd, M_DevInfo[u4_Device].u4_DevSecSize, Pv_buff );
}


/*******************************************************************************
	概要	:	READ CAPACITYパケットコマンド発行
	説明	:
	Include	:	ATA.h
	引数	:	u4_Device	デバイス選択 0 or 1
				*Pv_Buff	データ格納バッファポインタ
	戻り値	:
*******************************************************************************/
s4		ATA::PackeReadCapacity(u4 u4_Device, void *PV_Buff)
{
	ATAPI_Cmd	AtapiCmd;
	s4			s4_Return;

	//ゼロ埋め
	memset( AtapiCmd.u1_CmdPacket, 0, 12 );

	AtapiCmd.u1_Feature = 0;				//Non-overlap/non DMA
	AtapiCmd.u1_DevHed = u4_Device << 4;	//デバイス
	AtapiCmd.u1_CmdPacket[0] = 0x25;		//READ CAPACITY

	s4_Return = PacketPIO_CMD( &AtapiCmd, 8, PV_Buff );

	if(s4_Return < 0)
		return s4_Return;

	if( s4_Return == 8 )
		return SUCCESS;	//正常終了時=0

	return ERROR;		//エラー終了時!=0
}


/*******************************************************************************
	概要	:	TEST UNITコマンドパケット発行
	説明	:
	Include	:	ATA.h
	引数	:	u4_Device	デバイス選択 0 or 1
	戻り値	:
*******************************************************************************/
s4		ATA::PacketTestUnit( u4 u4_Device )
{
	ATAPI_Cmd	AtapiCmd;

	//ゼロ埋め
	memset( AtapiCmd.u1_CmdPacket, 0, 12 );

	AtapiCmd.u1_Feature = 0;				//Non-overlap/non DMA
	AtapiCmd.u1_DevHed = u4_Device << 4;	//デバイス

	return	PacketPIO_CMD( &AtapiCmd, 0, NULL );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
