/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/ATA/ATA_CmdProt.cpp
	概要		：
	詳細		：
	責任者		：	牧内 恵一朗
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"DM.h"
#include	"PCI.h"
#include	"TM.h"
#include	"ATA.h"
#include	"BMIDE.h"
#include	<Math.h>
#include	"RDTM.h"

//デバッグ用
//#include	<stdio.h>
//#include	"../../GUI/GUI.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern TM		G_TM;
extern PIC		G_PIC;
extern RDTM		G_RDTM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ATAクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	デバイスセレクション プロトコル
	説明	：
	Include	：
	引数	：	P_AtaCmd	コマンド発行用構造体
	戻り値	：	エラー値
*******************************************************************************/
s4		ATA::DeviceSelect( u1 u1_DeviceHead )
{
	u4		u4_Count;
	u1		u1_InByte;

	//30秒待ち
	for( u4_Count = 0; u4_Count < 30000; u4_Count++ )
	{
		u1_InByte = IO::In1( PORT_STATUS );

		//バスがアイドル状態か確認(1回目)
		if( ((u1_InByte & STATUSBIT_BSY) == 0) && ( (u1_InByte & STATUSBIT_DRQ) == 0) )
				break;

		G_RDTM.WaitMSec( 1 );	//1秒のウェイト
	}
	if( u4_Count == 300000 )
		return ERROR_TIMEOUT;	//タイムアウトエラー

	IO::Out1( PORT_DEVHEAD, u1_DeviceHead );		//Device Headレジスタにディバイス番号をセット

	Wait();

	//30秒待ち
	for( u4_Count = 0; u4_Count < 300000; u4_Count++ )
	{
		u1_InByte = IO::In1( PORT_STATUS );

		//バスがアイドル状態か確認(2回目)
		if( ( (u1_InByte & STATUSBIT_BSY) == 0) && ( (u1_InByte & STATUSBIT_DRQ) == 0) )
				break;

		G_RDTM.WaitMSec( 1 );	//1秒のウェイト
	}
	if( u4_Count == 300000 )
		return ERROR_TIMEOUT;	//タイムアウトエラー

	return	SUCCESS;
}


/*******************************************************************************
	概要	：	Non Data Command プロトコル
	説明	：
	Include	：
	引数	：	P_AtaCmd	コマンド発行用構造体
	戻り値	：	エラー値
*******************************************************************************/
s4		ATA::NonData_CMD( ATA_Cmd *P_AtaCmd )
{
	u1		u1_InByte;
	u4		u4_Count;
	s4		s4_Return;

	//デバイスセレクション
	s4_Return = DeviceSelect( P_AtaCmd->u1_DevHed );
	if( s4_Return < 0)
		return ERROR_DEVSELECT;	//セレクションエラー

//	IO::Out1( PORT_DEVHEAD, P_AtaCmd->u1_DevHed );

	//コマンド入力
	CommandOUT( P_AtaCmd );
	Wait();		//ウェイト

	for( u4_Count = 0; u4_Count < TIMEOUT; u4_Count++ )	//タイムアウトまでループ
	{
		u1_InByte = IO::In1( PORT_STATUS );			//Statusレジスタ
		if( (u1_InByte & STATUSBIT_BSY) == 0 )
			break;		//コマンド実行終了
	}
	if( u4_Count == TIMEOUT )
		return ERROR_TIMEOUT;			//タイムアウトエラー

//	IO::In1( PORT_ATA_ASR );							//Alternate Statusレジスタ

	u1_InByte = IO::In1( PORT_STATUS );				//Statusレジスタ
	if( u1_InByte & STATUSBIT_ERR )					//エラー終了
	{
		u1_InByte = IO::In1( PORT_ERROR );			//戻り値の下位8ビットをErrorレジスタの値とする
		return ERRORREGI_MASK | u1_InByte;;			//エラー終了 != 0
	}
	else												//コマンド正常実行終了時
		return	SUCCESS;
}


/*******************************************************************************
	概要	：	PIO Data In Command プロトコル
	説明	：
	Include	：
	引数	：	P_AtaCmd	コマンド発行用構造体
	戻り値	：	エラー値
*******************************************************************************/
s4		ATA::PIO_DataIn_CMD( ATA_Cmd *P_AtaCmd, u2 u2_Block, void *Pv_Buff )
{
	s4		s4_Return;
	u2*		Pu2_DataRaed;
	ui		ui_BlockCnt, ui_Cunt;
	u1		u1_InByte;

	Pu2_DataRaed = (u2 *)Pv_Buff;

	//デバイスセレクション
	s4_Return = DeviceSelect( P_AtaCmd->u1_DevHed);
	if( s4_Return < 0)
		return ERROR_DEVSELECT;	//セレクションエラー

#ifdef DP_DEBUG
	DP(" *PIO_DataIn_CMD* DeviceSelect");
#endif

	//コマンド送信
	CommandOUT( P_AtaCmd );
	Wait( );

#ifdef DP_DEBUG
	DP(" *PIO_DataIn_CMD* CommandOUT");
#endif

	//オールタネート・ステータス・レジスタ空読み
	IO::In1( PORT_STATUS );

	//読み出しブロック数ループ
	for( ui_BlockCnt = 0; ui_BlockCnt < u2_Block; ui_BlockCnt++ )
	{
		//タイムアウトまでコマンド実行
		for( ui_Cunt = 0; ui_Cunt < TIMEOUT; ui_Cunt++ )
		{
			u1_InByte = IO::In1( PORT_STATUS );
			if( (u1_InByte & STATUSBIT_BSY) == 0 )
				break;
		}

#ifdef DP_DEBUG
		DP(" *PIO_DataIn_CMD* CMDP!!!!");
#endif
		if( ui_Cunt == TIMEOUT )
			return ERROR_TIMEOUT;						//タイムアウトエラー

		u1_InByte = IO::In1( PORT_STATUS );
		if( (u1_InByte & STATUSBIT_ERR) != 0 )
			break;		//コマンド実行エラー

		if( (u1_InByte & STATUSBIT_DRQ) == 0 )
			return ERROR_CMD;	//コマンド未実行エラー(データなし)
		else
		{
			BlockDataRead( 256, Pu2_DataRaed );	//ブロック読み出し(256ワード)
#ifdef DP_DEBUG
			DP(" *PIO_DataIn_CMD* Pu2_DataRaed");
#endif
		}

		Pu2_DataRaed = Pu2_DataRaed + 256;			//データバッファポインタ
	}
#ifdef DP_DEBUG
	DP(" *PIO_DataIn_CMD* DataRaed_END!!!");
#endif

	//オールタネート・ステータス・レジスタ空読み
	IO::In1( PORT_STATUS );

	u1_InByte = IO::In1( PORT_STATUS );
	if( u1_InByte & STATUSBIT_ERR )
	{
		u1_InByte = IO::In1( PORT_STATUS );		//戻り値の下位8bitをErrorレジスタの値とする
		return ERRORREGI_MASK | u1_InByte;		//エラー終了
	}
	else
		return	SUCCESS;						//正常終了
}


/*******************************************************************************
	概要	：PACKET PIO Data Command プロトコル
	説明	：
	Include	：	ATA.h
	引数	：
	戻り値	：
*******************************************************************************/
s4		ATA::PacketPIO_CMD( ATAPI_Cmd *P_AtapiCmd, u2 u2_Limit, void const* PCv_Buff )
{
	u1*		Pu1_Buffer = (u1*)PCv_Buff;
	u4		u4_Count;
	s4		s4_Return ;
	u2		u2_DataSize;
	u1		u1_InSTR,u1_InIRR;

	if( DeviceSelect(P_AtapiCmd->u1_DevHed) < 0 )
		return	ERROR_DEVSELECT;		//セレクションエラ-

#ifdef DP_DEBUG
	DP( "PORT_STATUS 1:%#x",IO::In1( PORT_STATUS ) );
#endif

	//PACKETコマンド発行
	P_AtapiCmd->u1_SecCnt = 0;
	P_AtapiCmd->u1_CylLo = (u1)u2_Limit;
	P_AtapiCmd->u1_CylHi = (u1)(u2_Limit >> 8);
	P_AtapiCmd->u1_Command = 0xa0;
	CommandOUT( (ATA_Cmd *)P_AtapiCmd );
	Wait();								//ウェイト

	//デバイスのコマンドパケット受信開始待ち
	s4_Return = PackeCMDRecvWait();
	if( s4_Return < 0 )
		return s4_Return;

#ifdef DP_DEBUG
	DP( "PORT_STATUS 2:%#x",IO::In1( PORT_STATUS ) );
#endif

	//コマンドパケット送信
	BlockDataWrite( 6, &P_AtapiCmd->u1_CmdPacket );	//6ワード書き込み

#ifdef DP_DEBUG
	DP( "PORT_STATUS 3:%#x",IO::In1( PORT_STATUS ) );
#endif

	//パケットコマンド実行終了待ち
	for( u4_Count = 0; u4_Count < 30000; u4_Count++)	//タイムアウトまでループ
	{
		u1_InSTR = IO::In1( PORT_STATUS );			//ステータスリード
		u1_InIRR = IO::In1( PORT_INTREASON );		//割り込み要因リード

		if( ( u1_InSTR & STATUSBIT_BSY ) )			//コマンド実行中
		{
			G_RDTM.WaitMSec( 1 );		//1ms待機
			continue;
		}

		//エラーまたはイベントがあればエラー終了。
		if( ( u1_InSTR & STATUSBIT_CHK ) )	//Statusレジスタ CHK=1
		{
			u1_InSTR = IO::In1( PORT_ERROR );
			return	ERRORREGI_MASK | u1_InSTR;	//戻り値はErrorレジスタの値とする
		}

		//デバイス→ホストデータ転送
		if( ( u1_InIRR & INTERRUPBIT_IO )
		 && !( u1_InIRR & INTERRUPBIT_CD )
		 && ( u1_InSTR & STATUSBIT_DRQ ) )
		{
			u2_DataSize = ( IO::In1( PORT_BYTECNT_HI ) << 8 ) | IO::In1( PORT_BYTECNT_LO );	//読み出しバイト数
			BlockDataRead( INT_CEIL_BIN( u2_DataSize, 1 ) >> 1, Pu1_Buffer );	//Dataレジスタ読み出し
			Pu1_Buffer += u2_DataSize;
			u4_Count = 0;
			continue;
		}

		//ホスト→デバイスデータ転送
		if( !( u1_InIRR & INTERRUPBIT_IO )
		 && !( u1_InIRR & INTERRUPBIT_CD )
		 && ( u1_InSTR & STATUSBIT_DRQ ) )
		{
			u2_DataSize = ( IO::In1( PORT_BYTECNT_HI ) << 8 ) | IO::In1( PORT_BYTECNT_LO );	//書き込みバイト数

			if( u2_DataSize > u2_Limit )
				return	ERROR_PACKETCMD;	//実際に転送したバイト数がセクタサイズより大きい場合

			BlockDataWrite( INT_CEIL_BIN( u2_DataSize, 1 ) >> 1, Pu1_Buffer );//Dataレジスタ書き込み
			Pu1_Buffer += u2_DataSize;
			u4_Count = 0;
			continue;
		}

		//パケットコマンド実行終了
//		if( ( u1_InIRR & INTERRUPBIT_IO )
//		 && ( u1_InIRR & INTERRUPBIT_CD )
//		 && !( u1_InSTR & STATUSBIT_DRQ ) )
		if( ( u1_InIRR & ( INTERRUPBIT_IO | INTERRUPBIT_CD) )
		 && !( u1_InSTR & STATUSBIT_DRQ ) )
		{
			IO::In1( PORT_STATUS );			//Statusレジスタ空読み
			return (s4)( (ui)Pu1_Buffer - (ui)PCv_Buff );
		}
	}

#ifdef DP_DEBUG
	DP( "PORT_ERROR:%#x",IO::In1( PORT_ERROR ) );
	DP( "PORT_STATUS 4:%#x",IO::In1( PORT_STATUS ) );
#endif

	return ERROR_TIMEOUT;	//タイムアウトエラー
}



/*******************************************************************************
	概要	：	コマンドパケット受信準備完了
	説明	：	デバイスのコマンドパケット受信開始待ちをする関数です。
	Include	：	ATA.h
	引数	：	void
	戻り値	：	Errorレジスタの値
*******************************************************************************/
s4		ATA::PackeCMDRecvWait(void)
{
	u4		u4_Count;
	u1		u1_InSTR = 0,u1_InIRR = 0;

	for( u4_Count = 0; u4_Count < TIMEOUT; u4_Count++ )		//タイムアウトまでループ
	{
		u1_InSTR = IO::In1( PORT_STATUS );			//Statusレジスタ読み込み

		if( (u1_InSTR & STATUSBIT_BSY) == 0 )		//コマンド実行終了
		{
			if( (u1_InSTR & STATUSBIT_CHK) != 0 )	//Statusレジスタ CHK=1
			{
				u1_InSTR = IO::In1( PORT_ERROR );
				return ERRORREGI_MASK | u1_InSTR;	//戻り値はErrorレジスタの値とする
			}
			u1_InIRR = IO::In1( PORT_INTREASON );	//Interrup Reasonレジスタ読み込み

			//受信準備完了
			if( ( (u1_InSTR & STATUSBIT_DRQ) != 0 )			//Statusレジスタ DRQ=1
				&& ( (u1_InIRR & INTERRUPBIT_IO ) == 0 )	//Interrup Reasonレジスタ I/O=0
				&& ( (u1_InIRR & INTERRUPBIT_CD ) != 0 ) )	//Interrup Reasonレジスタ C/D=1
				return SUCCESS;
		}
	}

	return ERROR_TIMEOUT;		//タイムアウトエラー
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

