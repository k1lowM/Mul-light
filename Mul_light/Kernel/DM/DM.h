/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/DM.h
	概要		：	デバイス管理クラスヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Driver.h"
#include	"PIC/PIC.h"
#include	"IM/IM.h"

#include	"GDDM.h"
#include	"GDDQ.h"

#include	"FS.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern	GDDQ	G_GDDQ;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	デバイス情報
*******************************************************************************/
struct	DevInfo
{
public:
	Driver*	P_Driver;	//ドライバクラスポインタ
	FS*		P_FS;		//ファイルシステムクラスのポインタ
};


/*******************************************************************************
	デバイス管理クラス
*******************************************************************************/
class	DM
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_TYPENON,			//デバイスタイプが無効
		ERROR_NULLPOINTER,		//NULLポインタ
		ERROR_INVALIDTYPE,		//デバイスタイプ無効
		ERROR_DITFULL,			//デバイス情報テーブル空き無し
		ERROR_OVERSIZEID,		//IDが大きすぎる
		ERROR_DEVNOTHING,		//デバイスが登録されていない

		SUCCESS = _SUCCESS_,
	};

	//システムデバイスの予約番号
	enum	SystemDevice
	{
		SD_PIC,
		SD_PIT,
		SD_RTC,
		SD_PCI,

		NUM_SD
	};

private:
	enum
	{
		NUM_DEVICE		= 256,
	};

//変数
private:
	DevInfo	M_DevInfo[NUM_DEVICE];	//デバイス情報

//メソッド
public:
	void	SysInit( void );
	void	Init( void );

	s4		Ctrl( DCD* P_DCD );

	//デバイス登録／更新／抹消
	s4		RegiDevice( u1* Pu1_DeviceID, Driver* P_Driver );		//登録
	s4		DevInfoUpdate( u1 u1_DeviceID, Driver* P_Driver );		//更新
	s4		DevInfoUpdate( u1 u1_DeviceID, FS* P_FS );				//更新
	s4		EraseDevice( u1 u1_DeviceID );							//抹消

	DevInfo	GetDevInfo( u1 u1_DeviceID );		//デバイス情報取得

private:
	void	RegiSysDev( u1 u1_DeviceID, Driver* P_Driver );		//システムデバイス登録
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
