/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/ATA/ATA_Init.cpp
	概要		：	ATAクラス
	詳細		：	ATA/ATAPIデバイス制御を管理するクラスです。
	責任者		：	牧内 恵一朗
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■as
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




/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
enum
{
	DEVBIT			=  4,
	IORDY_SUP		= 0x800,

	//SET FEATURESコマンド用サブコマンド
	SET_TRANSFER	= 0x03,		//Set Transfer Mode
	ENA_MEDIASTATUS	= 0x95,		//Enable Media Status Notification
	DIS_MEDIASTATUS	= 0x31,		//Disable Media Status Notification

	PIO_FLOWCTRL	= 0x08,		//PIOフロー制御モード
//	TMN_MASK		= 0x07,		//転送モード番号マスク
	UDMA_TM			= 0x40,		//UDMA転送モード

	//GetMediaStatusエラービット
	BIT_NM			= 2,		//メディアがない
	BIT_MCR			= 8,		//メディアチェンジが要求された
	BIT_MC			= 0x20,		//メディアがチェンジされた
	BIT_WP			= 0x40,		//メディアがライトプロテクト状態である
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	マクロ関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#define	SENSE_INFO( buf )		( ( (buf[2] & 0x0f) << 16 ) | ( buf[12] << 8) | (buf[13]) )
#define	BIG2LITTLE( buf, i )	( ((buf)[(i)] << 24) | ((buf)[(i) + 1] << 16) | ((buf)[(i) + 2] << 8) | (buf)[(i) + 3] )

/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ATAクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ATAクラスの初期化サブ関数
	説明	：	ATAデバイスの接続確認(シグネチャ発行)
	Include	：	ATA.h
	引数	：	u4_Device	ターゲットデバイス
	戻り値	：	SUCCESS		成功
				ERROR		失敗
*******************************************************************************/
s4		ATA::Init_DeviceCheck( u4 u4_Device ,u2* Pu2_IDdata )
{
	u4		u4_Interim = 0;
	u1		u1_CylLo, u1_CyHi;

	//シグネチャ取得
	if( Signature( u4_Device, &u1_CylLo, &u1_CyHi ) < 0 )
	{
		M_DevInfo[u4_Device].u4_DeviceType = DEVICE_NON;	//ディバイスは未接続
		return	ERROR;
	}
#ifdef DP_DEBUG
	DP(" *Init_DeviceCheck* Signature");
#endif
	//暫定接続状況
	if( (u1_CylLo == 0) && (u1_CyHi ==0) )				//ディバイスはATAの可能性あり
		u4_Interim = DEVICE_ATA;
	else if( (u1_CyHi == 0x14) && ( u1_CylLo== 0xeb) )	//ディバイスはATAPIの可能性あり
		u4_Interim = DEVICE_ATAPI;

	//Identify発行
	return	Init_DeviceCheck_Sub( u4_Device, u4_Interim ,Pu2_IDdata);
}


/*******************************************************************************
	概要	：	ATAクラスの初期化サブ関数
	説明	：	ATAデバイスの接続確認(Identify発行)
	Include	：	ATA.h
	引数	：	u4_Device	ターゲットデバイス
				u4_Interim	暫定デバイスタイプ
				Pu2_IDdata	Identify Deviceコマンドで取得したデータ
	戻り値	：	SUCCESS		成功
				ERROR		失敗
*******************************************************************************/
s4		ATA::Init_DeviceCheck_Sub( u4 u4_Device, u4 u4_Interim, u2 *Pu2_IDdata )
{
	s4		s4_Count, s4_Return;

	//BSYビットクリア待ち
	if( Wait_BsyCHK( ) < 0 )
	{
		M_DevInfo[u4_Device].u4_DeviceType = DEVICE_UNKOWN;		//不明なデバイス
		return	ERROR;
	}

#ifdef DP_DEBUG
	DP(" *Init_DeviceCheck* Wait_BsyCHK");
#endif

	for( s4_Count = 0; s4_Count < RETRY_MAX; s4_Count++ )
	{
		Wait( );
		s4_Return = IdentifyDevice( u4_Device, u4_Interim, Pu2_IDdata );
		Wait( );
		if( s4_Return != IdentifyDevice( u4_Device, u4_Interim, Pu2_IDdata ) )
			continue;

#ifdef DP_DEBUG
		DP(" *Init_DeviceCheck* IdentifyDevice");
#endif

		//IdentifyDeviceを2回実行して戻り値が一致
		if( s4_Return == SUCCESS )								//エラーなし＆IDENTIFYデータ正常取得
		{
			M_DevInfo[u4_Device].u4_DeviceType = u4_Interim;	//デバイス確定！
			return	SUCCESS;
		}
		else if( s4_Return == ERROR_NODEVICE || s4_Return == ERROR_ABORT )				//コマンド未実行終了またはアボートエラー
		{
			M_DevInfo[u4_Device].u4_DeviceType = DEVICE_NON;	//デバイスは未接続でした・・・
			return	SUCCESS;
		}
		//それ以外のエラーはリトライ
	}
	M_DevInfo[u4_Device].u4_DeviceType = DEVICE_UNKOWN;		//タイムアウトで不明なデバイス
	return	ERROR;
}


/*******************************************************************************
	概要	：	ATAクラスの初期化サブ関数
	説明	：	ATAデバイスの転送モード判定
	Include	：	ATA.h
	引数	：	u4_Device	ターゲットデバイス
				Pu2_IDdata	Identify Deviceコマンドで取得したデータ
	戻り値	：	SUCCESS		成功
				ERROR		失敗
*******************************************************************************/
s4		ATA::Init_DeviceMode( u4 u4_Device ,const u2* Pu2_IDdata )
{
	s4		s4_TM = M_DevInfo[u4_Device].s4_TransferMode;

	if( (M_DevInfo[u4_Device].u4_DeviceType & DEVICE_ATA) ||
		(M_DevInfo[u4_Device].u4_DeviceType & DEVICE_ATAPI) )
	{
		//UDAMモードサポートチェック
		if( Pu2_IDdata[53] & 4 )	//ワード53 ビット3=1 ワード88有効
		{
			s4_TM = UDMA_MODE_0;		//基本はUltra DMAモード0
			if( Pu2_IDdata[88] & 0x2 )		s4_TM = UDMA_MODE_1;	//Ultra DMAモードモード1
			if( Pu2_IDdata[88] & 0x4 )		s4_TM = UDMA_MODE_2;	//Ultra DMAモードモード2
			if( Pu2_IDdata[88] & 0x8 )		s4_TM = UDMA_MODE_3;	//Ultra DMAモードモード3
			if( Pu2_IDdata[88] & 0x10 )		s4_TM = UDMA_MODE_4;	//Ultra DMAモードモード4
		}

		//PIOモードサポートチェック
		if( Pu2_IDdata[53] & 2 )	//ワード53 ビット1=1 ワード64〜70有効
		{
			M_DevInfo[u4_Device].s4_TransferMode &= UDMA_MODE_MSK;		//基本はPIOモード0
			if( Pu2_IDdata[64] & 1 )		s4_TM = (s4_TM & UDMA_MODE_MSK) | PIO_MODE_3;	//PIOモード3
			if( Pu2_IDdata[88] & 2 )		s4_TM = (s4_TM & UDMA_MODE_MSK) | PIO_MODE_4;	//PIOモード4
		}
		else	//対応モードが不明な場合
		{
			s4_TM = UNKOWN_MODE;	//転送モード非対応
			return	ERROR;
		}
	}
	else//デバイス未接続 or 非ATA/ATAPIデバイス
	{
		s4_TM = NON_MODE;	//未接続や非ATA/ATAPIデバイスなら無視
		return	ERROR;
	}

	M_DevInfo[u4_Device].s4_TransferMode = s4_TM;

	return	Init_DeviceMode_Sub( u4_Device, Pu2_IDdata );
}


/*******************************************************************************
	概要	：	ATAクラスの初期化サブ関数
	説明	：	ATAデバイスの転送モード判定サブ
	Include	：	ATA.h
	引数	：	u4_Device	ターゲットデバイス
				Pu2_IDdata	Identify Deviceコマンドで取得したデータ
	戻り値	：	SUCCESS		成功
				ERROR		失敗
*******************************************************************************/
s4		ATA::Init_DeviceMode_Sub( u4 u4_Device ,const u2* Pu2_IDdata )
{
	s4		s4_Return;

	//ケーブル判定
//	if( G_BMIDE.GetCableType(Mu1_BMOffset) == BMIDE::CABLE80 )
	{
		//UltraDMA転送設定
		if( M_DevInfo[u4_Device].s4_TransferMode & UDMA_MODE_MSK )
		{
			// UltraDMA転送
			s4_Return = SetFeatures( u4_Device, SET_TRANSFER, M_DevInfo[u4_Device].s4_TransferMode >> 4 );
			if( s4_Return < 0 )	//UltraDMA転送設定エラー
				M_DevInfo[u4_Device].s4_TransferMode &= PIO_MODE_MSK;
		}
	}

	//PIO転送モード設定
	if( (Pu2_IDdata[49] & IORDY_SUP) == 0 )		//IORDYサポート
	{
		//PIOフロー制御モード&PIO転送モードを設定
		s4_Return = SetFeatures( u4_Device, SET_TRANSFER, PIO_FLOWCTRL | (M_DevInfo[u4_Device].s4_TransferMode & PIO_MODE_MSK) );
		if( s4_Return < 0 )									//フローコントロール付きPIOモード 設定エラー
		{
			SetFeatures( u4_Device, SET_TRANSFER, 0 );				//PIOデフォルト転送モード(エラー無視)
			M_DevInfo[u4_Device].s4_TransferMode &= UDMA_MODE_MSK;		//PIO転送モードリセット
		}
	}
	else	//PIOフローコントロール未サポート
	{
		SetFeatures( u4_Device, SET_TRANSFER, 0 );				//PIOデフォルト転送モード(エラー無視)
		M_DevInfo[u4_Device].s4_TransferMode &= UDMA_MODE_MSK;		//PIO転送モードリセット
	}

	//Removable Media Status Notification サポートチェック
	if( ( (Pu2_IDdata[83] & 0x10) == 0x10) || ((Pu2_IDdata[127] & 3) == 1 ) )
	{
		s4_Return = SetFeatures( u4_Device, ENA_MEDIASTATUS, 0 );	//Enable Media Status Notification

		if( s4_Return & 2 )
			M_DevInfo[u4_Device].u4_DeviceType |= SUP_LOCK_UNLOCK;	// ロック/アンロック機能あり
		if( s4_Return & 3 )
			M_DevInfo[u4_Device].u4_DeviceType |= SUP_AUTOEJECTt;	// パワーイジェクト機能あり
	}

	//デバイスタイプ設定
	M_DevInfo[u4_Device].u4_DeviceType |= ( Pu2_IDdata[0] >> 8 ) & 0x1f ;

	// リムーバブルデバイス
	if( Pu2_IDdata[0] & 0x80 )
		M_DevInfo[u4_Device].u4_DeviceType |= SUP_REMOVABLE;

	//GET MEDIA STATUSコマンド対応確認
	if(
		( (Pu2_IDdata[82] != 0xffff) && ( (Pu2_IDdata[82] & 0x0004) == 0x0004 ) )	//Removable Media feature set
		||		//←とりあえず「どれか成立すれば」で判定
		( (Pu2_IDdata[83] != 0xffff) && ( (Pu2_IDdata[83] & 0x0010) == 0x0010 ) )	//Removable Media Status Notification feature set
		||		//←とりあえず「どれか成立すれば」で判定
		( ( (Pu2_IDdata[127] & 3) == 1 ) )											//Removable Media Status Notification feature set
	)
	{	//GET MEDIA STATUSコマンドに対応しているかもしれないので、実際にコマンドを発行してみる
		s4_Return = GetMediaStatus( u4_Device );
		if( !(s4_Return & 4) )
			M_DevInfo[u4_Device].u4_DeviceType |= SUP_GMS_CMD;	//アボードが発生しなければGET MEDIA STATUSコマンド対応
	}

	return	SUCCESS;
}
/*******************************************************************************
	概要	：	ATAクラスの初期化サブ関数
	説明	：	ATAデバイスをアイドル状態へ移行
	Include	：	ATA.h
	引数	：	void
	戻り値	：	SUCCESS		成功
				ERROR		失敗
*******************************************************************************/
s4		ATA::Init_DeviceIdle( u4 u4_Device ,const u2* Pu2_IDdata )
{
	s4		s4_Return;

	//ATA
	if( DeviceSelect( u4_Device << DEVBIT ) != SUCCESS )
		return	ERROR_DEVSELECT;		//デバイス選択エラー

	if( M_DevInfo[u4_Device].u4_DeviceType & DEVICE_ATA )
	{
		s4_Return = InitDevParamaters( u4_Device, Pu2_IDdata[3] - 1, Pu2_IDdata[6] );
		if( s4_Return != 0 )
			return	IDP_ERR;		//INITALIZE DEVICE PARAMATERSコマンドエラー
		s4_Return = IdleDevice( u4_Device , DEVICE_ATA );	//IDLEコマンド
		DP("ATA_IDLE");
		if( s4_Return != 0 )
			return	IDLECMD_ERR;
	}

	//ATAPI
	if( (M_DevInfo[u4_Device].u4_DeviceType & DEVICE_MASK) == DEVICE_ATAPI )
	{
//		ATAPIなら IDLE IMMEDIATEコマンドサポートが必須になっているはずだが
		if( Pu2_IDdata[82] & 8 )	// Power Management feature setサポート
		{
			s4_Return = IdleDevice( u4_Device , DEVICE_ATAPI );
			if( s4_Return < 0 )
				return	IDLECMD_ERR;

			DP("ATAPI_IDLE 2");
		}
	}

	return	SUCCESS;
}


/*******************************************************************************
	概要	：	メディア状態チェック
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス
	戻り値	：	メディア状態
*******************************************************************************/
s4		ATA::MediaStatusCHK( u4 u4_Device )
{
	//非リムーバブルメディア
	if( !(M_DevInfo[u4_Device].u4_DeviceType & SUP_REMOVABLE))
		return	MEDIA_RADDY;											//常時レディ状態

	//CD-ROMデバイスの場合
	if( (M_DevInfo[u4_Device].u4_DeviceType & DEV_MASK) == DEV_CD_ROM )
	{
		//ビット27〜8にSenseKeyやASC/ASCQの状態を格納
		s4		s4_DeviceReady = ATAPI_DeviceReady( u4_Device );

		if( s4_DeviceReady < 0 )
			return	ERROR_MEDIA;

		if( s4_DeviceReady == 0 )								//レディ状態
			return	MEDIA_RADDY;

		if( (s4_DeviceReady & 0x000fff00) == 0x00062800 )		//ディスクが交換された
			return	MEDIA_WP | MEDIA_CHG;

		if( (s4_DeviceReady & 0x000f0000) == 0x00020000 )		//Notレディ状態
			return	MEDIA_WP | MEDIA_NOT_READY;
	}

	//CD-ROM以外のデバイスの場合
	else if( M_DevInfo[u4_Device].u4_DeviceType & SUP_GMS_CMD )		//GET MEDIA STATUSコマンド対応
	{
		s4		s4_GETMeSt = GetMediaStatus( u4_Device );	//GET MEDIA STATUSコマンド

		if( s4_GETMeSt == SUCCESS )
			return	MEDIA_RADDY;			//レディ状態

		//エラー状態
		if( s4_GETMeSt & ERRORBIT_ABRT )
			return	ERROR_MEDIA;			//何らかのエラー

		if( s4_GETMeSt & BIT_WP )
			return	MEDIA_WP;				//メディアがライトプロテクト状態である
		if( s4_GETMeSt & BIT_NM )
			return	MEDIA_NO_DISK;			//メディアがない
		if( s4_GETMeSt & BIT_MC )
			return	MEDIA_CHG;				//メディアがチェンジされた
		if( s4_GETMeSt & BIT_MCR )
			return	MEDIA_CHG_REQ;			//メディアチェンジが要求された
	}

	return	ERROR_MEDIA;
}

/*******************************************************************************
	概要	：	ATAPIデバイス(CD-ROMドライブ)レディチェック
	説明	：	ATAデバイスをアイドル状態へ移行
	Include	：	ATA.h
	引数	：	u4_Device	デバイス選択
	戻り値	：	bit19 - 16	SenseKey
				bit15 - 8	ASC
				bit7  - 0	ASCQ
*******************************************************************************/
s4	ATA::ATAPI_DeviceReady( u4 u4_Device )
{
	s4		s4_Return;
	u1		u1_SenseBuff[18];

	//デバイスはATAPI限定
	if( !(M_DevInfo[u4_Device].u4_DeviceType & DEVICE_ATAPI) )
		return	ERROR_NOATAPI;

	//Request Senseが失敗したときはリトライ
	for(u4 u4_cont = 0; u4_cont < RETRY_MAX; u4_cont++ )
	{
		PacketTestUnit( u4_Device );	//ATAPI TEST UNIT
		s4_Return = PacketRequestSense( u4_Device, u1_SenseBuff );
		if(s4_Return == SUCCESS)
			return	SENSE_INFO( u1_SenseBuff );	//正常終了

		Wait( );	//ウェイト
	}

	return ERROR;
}


/*******************************************************************************
	概要	：	デバイス/メディアのセクタあたりのバイト数と最大LBAの取得
	説明	：
	Include	：	ATA.h
	引数	：	u4_Device	デバイス
				*Pu4_LBA	最大LBA値格納ポインタ
				*Pu4_Size	セクタサイズ値格納ポインタ
	戻り値	：
*******************************************************************************/
s4		ATA::GetMediaInfo(u4 u4_Device, u4* Pu4_LBA, u4* Pu4_Size)
{
	s4		s4_Return;
	u4		u4_Cnut;
	u1		u1_Buf[8];

	//デバイスはATAPI限定
	if( !(M_DevInfo[u4_Device].u4_DeviceType & DEVICE_ATAPI) )
		return	ERROR_NOATAPI;

	//READ CAPACITYコマンドパケット発行
	for( u4_Cnut = 0; u4_Cnut < RETRY_MAX; u4_Cnut++ )
	{
		s4_Return = PackeReadCapacity( u4_Device, u1_Buf );
		if( s4_Return > 0 )
			break;				//正常取得
		else
			return	s4_Return;	//エラー終了(メディアが入っていないなど)
	}

	*Pu4_LBA = BIG2LITTLE( u1_Buf, 0 );		//最大LBA;
	*Pu4_Size = BIG2LITTLE( u1_Buf, 4 );	//ブロックサイズ;

	return	SUCCESS;	//正常終了
}


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

