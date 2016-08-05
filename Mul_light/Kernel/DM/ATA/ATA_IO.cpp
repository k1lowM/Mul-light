/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/ATA/ATA_IO.cpp
	概要		：	ATA_IOレイヤ
	詳細		：	ATA/ATAPIのレジスタアクセスを管理するレイヤです。
	責任者		：	牧内 恵一朗
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■as
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"../../Common.h"
#include	"../DM.h"
#include	"ATA.h"
#include	"RDTM.h"


//デバッグ用
//#include	<stdio.h>
//#include	"../../GUI/GUI.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern RDTM		G_RDTM;


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ATA_IOレイヤ
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	データレジスタ ブロックデータ読み込み
	説明	：	データレジスタにブロックデータを読み込みます
	Include	：	ATA.h
	引数	：	u2_Len	読み込みブロック数
				Pv_Buff	読み込みバッファ
	戻り値	：	-
*******************************************************************************/
void	ATA::BlockDataRead( u2 u2_Len, void* Pv_Buff )
{
	u2		u2_Count;
	u2*		Pu2_Raed;

	Pu2_Raed = (u2*)Pv_Buff;

//	DP(" *BlockDataRead* ");

	if( Pu2_Raed == 0 )	//データの空読み
	{
		for( u2_Count = 0; u2_Count < u2_Len; u2_Count++ )
		{
			IO::In2( PORT_DATA );
		}
	}
	else	//データバッファから読み出し
	{
		for( u2_Count = 0; u2_Count < u2_Len; u2_Count++ )
		{
			*Pu2_Raed = IO::In2( PORT_DATA );
			Pu2_Raed++;
		}
	}
}


/*******************************************************************************
	概要	：	データレジスタ ブロックデータ書き込み
	説明	：	データレジスタにブロックデータを書き込みます
	Include	：	ATA.h
	引数	：	u2_Len	書き込むブロック数
				Pv_Buff	書き込むバッファ
	戻り値	：	-
*******************************************************************************/
void 	ATA::BlockDataWrite( u2 u2_Len, void* Pv_Buff )
{
	u2		u2_Count;
	u2*		Pu2_Write;

	Pu2_Write = (u2 *)Pv_Buff;

	if(  Pu2_Write == 0 )	//ダミーデータ( 00h )の書き込み
	{
		for( u2_Count = 0; u2_Count < u2_Len; u2_Count++)
		{
			IO::Out2( PORT_DATA, 0 );
			Pu2_Write++;
		}
	}
	else	//データバッファへ書き込み
	{
		for( u2_Count = 0; u2_Count < u2_Len; u2_Count++ )
		{
			IO::Out2( PORT_DATA, *Pu2_Write );
			Pu2_Write++;
		}
	}
}


/*******************************************************************************
	概要	：	ウェイト
	説明	：	適当な時間ウェイトします。
	Include	：	ATA.h
	引数	：	-
	戻り値	：	-
*******************************************************************************/
void	ATA::Wait( void )
{
//	DateTime*	P_DateTime;

//	u4		u4_count;
//	for( u4_count = 0 ; u4_count < 400000; u4_count++ );
	G_RDTM.WaitMSec( 5 );		//5ms待機
}


/*******************************************************************************
	概要	：
	説明	：	BSYビットがクリアされるまでウェイト
	Include	：	ATA.h
	引数	：	-
	戻り値	：	-
*******************************************************************************/
s4		ATA::Wait_BsyCHK(void)
{
	u1		u1_Chr;
	u4		u4_i;

	for( u4_i = 0; u4_i < TIMEOUT; u4_i++ )
	{
		u1_Chr = IO::In1( PORT_STATUS );
		if( !( u1_Chr & 0x80) )
			return SUCCESS;	//BSYビットが0になったらreturn
	}
	return	ERROR_TIMEOUT;	//タイムアウト
}

/*******************************************************************************
	概要	：
	説明	：	DRDYビットが立つまでウェイト
	Include	：	ATA.h
	引数	：	-
	戻り値	：	-
*******************************************************************************/
void	ATA::Wait_DevReadySET(void)
{
	u1		u1_Chr;

	while(1)
	{
		u1_Chr = IO::In1( PORT_STATUS );
		if( u1_Chr & 0x40 )
			break;	//DRDYビットが1になったらreturn
	}
}


/*******************************************************************************
	概要	：	ソフトウェアリセット＆割り込み許可
	説明	：	ATAデバイスのソフトウェアリセットと割り込み許可を実行します。
	Include	：	ATA.h
	引数	：	-
	戻り値	：	-
*******************************************************************************/
void	ATA::ATAReset( void )
{
	// ソフトウェアリセット
	IO::Out1( PORT_DEVCTRL, 0x6 );	//ソフトウェア・リセット & 割り込み禁止
	Wait();						//ウェイト
	IO::Out1( PORT_DEVCTRL, 0x2 );	//ソフトリセット解除 & 割り込み禁止
	Wait();						//ウェイト

	//割り込み許可
//	IO::Out1( PORT_DEVCTRL, 0 );
}


/*******************************************************************************
	概要	：	シグネチャ取得
	説明	：	ATAデバイスのシグネチャ取得します。
	Include	：	ATA.h
	引数	：	u4_Device	マスタ / スレーブ
	戻り値	：	s4			エラー値
*******************************************************************************/
s4		ATA::Signature( u4 u4_Device, u1* Pu1_CylLo, u1* Pu1_CylHi )
{
	u4		u4_Count;

	for( u4_Count = 0; u4_Count < RETRY_MAX; u4_Count++ )
	{
		*Pu1_CylLo = *Pu1_CylHi = DEVICE_NON;			//ディバイス未接続フラグ
		IO::Out1( PORT_DEVHEAD, u4_Device << 4 );	//デバイス選択
		Wait( );									//ウエィト
		if( IO::In1( PORT_STATUS ) == 0xff )
			return	ERROR;							//Statusレジスタが0xffだったら未接続
		if( Wait_BsyCHK( ) < 0 )
			return ERROR;									//BYSビットクリア待ち(ビジー状態判定)
		if( (IO::In1( PORT_ERROR ) & 0x7f) != 1 )
			return	ERROR;							//ヤンキー(不良)です。
		if( (IO::In1( PORT_DEVHEAD ) & 0x10) == 0 )	//デバイスの接続確認
		{
			*Pu1_CylHi = IO::In1( PORT_CYL_LO );		//シリンダLow
			*Pu1_CylLo = IO::In1( PORT_CYL_HI );		//シリンダHi
			break;
		}
	}

	return	SUCCESS;
}


/*******************************************************************************
	概要	：	レジスタに値を代入
	説明	：	レジスタに値を代入します。
	Include	：	ATA.h
	引数	：	P_AtaCmd	コマンド発行用構造体
	戻り値	：	void
*******************************************************************************/
void	ATA::CommandOUT( ATA_Cmd *P_AtaCmd )
{
//	IO::Out1( PORT_DEVCTRL, 0 );						//割り込み許可
	IO::Out1( PORT_FEATURES, P_AtaCmd->u1_Feature );	//フィーチャー
	IO::Out1( PORT_SCT_COUNT, P_AtaCmd->u1_SecCnt );	//セクタカウント
	IO::Out1( PORT_SCT_NUMBER, P_AtaCmd->u1_SecNo );	//セクタナンバ
	IO::Out1( PORT_CYL_LO, P_AtaCmd->u1_CylLo );		//シリンダLo
	IO::Out1( PORT_CYL_HI, P_AtaCmd->u1_CylHi );		//シリンダHi

	if( P_AtaCmd->u4_Flag & STATUSBIT_DRDY )
		Wait_DevReadySET();								//DRDYビットウェイト
	IO::Out1( PORT_COMMAND, P_AtaCmd->u1_Command );		//コマンド
}


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

