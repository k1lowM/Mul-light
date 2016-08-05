/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	デバイス名	：	Include/Mul_light/Device/DCD.cpp
	概要		：	Mul light API - デバイス制御情報
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"
#include	"DCD.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	DCDクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	制御ID取得
	説明	：	制御番号とクラス番号をまとめた制御IDを返します。
	Include	：	DCD.h
	引数	：	void
	戻り値	：	u4		制御ID(31-16:制御番号, 15-0:クラス番号)
*******************************************************************************/
u4		DCD::GetCtrlID( void )
{
	return Mu4_CtrlID;
}


/*******************************************************************************
	概要	：	デバイスID取得
	説明	：	デバイスIDを返します。
	Include	：	DCD.h
	引数	：	void
	戻り値	：	u1		デバイスID
*******************************************************************************/
u1		DCD::GetDeviceID( void )
{
	return Mu1_DeviceID;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	各クラス：初期化関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	各クラスの初期化
	説明	：	ほぼ同じ内容なので、マクロにしました。
	Include	：	DCD.h
	引数	：	u4 CtrlID		任意の制御番号(下位16ビットは切り捨てられます)
				u1 u1_DeviceID		デバイスID
	戻り値	：	void
	例：
	void	DCD_Char::Init( u4 u4_CtrlID, u1 u1_DeviceID )
	{
		Mu4_CtrlID		= ( CtrlID & CTRLID_MASK ) | CLASTYPE_CHAR;		//任意の制御番号 | クラス番号
		Mu1_DeviceID	= u1_DeviceID;		//デバイスID
	}
*******************************************************************************/
#define	INIT_FUNC( _CLASS_, _TYPE_ )\
	void	_CLASS_::Init( u4 u4_CtrlID, u1 u1_DeviceID )\
	{\
		Mu4_CtrlID		= ( u4_CtrlID & CTRLTYPE_MASK ) | _TYPE_;\
		Mu1_DeviceID	= u1_DeviceID;\
	}

INIT_FUNC( DCD_Char, CLASS_CHAR )
INIT_FUNC( DCD_Data, CLASS_DATA )



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of Device
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
