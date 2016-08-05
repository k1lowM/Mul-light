/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/DM/GDDM_DD2Msg.cpp
	概要		：	ウィンドウ管理
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"GDDM.h"
#include	"DM.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	//マウスのボタン情報
	enum
	{
		MOUSE_LEFT_BUTTON		= 0x01,			//左ボタン
		MOUSE_RIGHT_BUTTON		= 0x02,			//右ボタン
		MOUSE_CENTER_BUTTON		= 0x04,			//中央ボタン
		MOUSE_BUTTON_MASK		= 0x07,			//ボタンマスク
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern DM		G_DM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ウィンドウ管理クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	デバイスデータをメッセージに変換
	説明	：
	Include	：	GDDM.h
	引数	：	const DD& CR_DeviceData		メッセージに変換するデバイスデータクラス
				Msg* P_Message		メッセージ格納先
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		GDDM::DD2Msg( const DD& CR_DeviceData, Msg* P_Message )
{
	DD		DeviceData	= CR_DeviceData;
	DevInfo	DeviceInfo	= G_DM.GetDevInfo( DeviceData.GetDeviceID() );		//デバイス情報取得

	//マウス情報
	static ui		Sui_MouseXOld = 0, Sui_MouseYOld = 0;	//マウス座標
	static u4		Su4_MouseButtonOld = 0;					//マウスボタンのデータ


	//デバイスの種類による分岐
	switch( DeviceInfo.P_Driver->GetDeviceType() )
	{
	//キーボード
	case DT::KEYBOARD:
		if( DeviceData.Mu4_Data[0] & 0x00000100 )
			P_Message->Init( Msg::KEY_RELEASE );
		else
			P_Message->Init( Msg::KEY_PRESS );

		P_Message->Mu4_Data[0] = CR_DeviceData.Mu4_Data[0];
		break;

	//マウス
	case DT::MOUSE:
	{
		ui		ui_MouseX			= (u2)( DeviceData.Mu4_Data[0] >> 8 );
		ui		ui_MouseY			= (u2)( DeviceData.Mu4_Data[0] );
		u4		u4_MouseButton		= DeviceData.Mu4_Data[1];
		u4		u4_MouseButtonXOR	= u4_MouseButton ^ Su4_MouseButtonOld;		//変化があったビットが1になる。

		//MOUSE_MOVE(このメッセージが最多になると思われるため、最初に処理する。)
		if( ui_MouseX ^ Sui_MouseXOld || ui_MouseY ^ Sui_MouseYOld )		//座標が少しでも違う場合
			P_Message->Init( Msg::MOUSE_MOVE );
		//MOUSE_L_RELEASE/DOWN
		else if( u4_MouseButtonXOR & MOUSE_LEFT_BUTTON )
		{
			if( u4_MouseButton & MOUSE_LEFT_BUTTON )
				P_Message->Init( Msg::MOUSE_L_PRESS );
			else
				P_Message->Init( Msg::MOUSE_L_RELEASE );
		}
		//MOUSE_R_RELEASE/DOWN
		else if( u4_MouseButtonXOR & MOUSE_RIGHT_BUTTON )
		{
			if( u4_MouseButton & MOUSE_RIGHT_BUTTON )
				P_Message->Init( Msg::MOUSE_R_PRESS );
			else
				P_Message->Init( Msg::MOUSE_R_RELEASE );
		}
		//MOUSE_C_RELEASE/DOWN
		else if( u4_MouseButtonXOR & MOUSE_CENTER_BUTTON )
		{
			if( u4_MouseButton & MOUSE_CENTER_BUTTON )
				P_Message->Init( Msg::MOUSE_C_PRESS );
			else
				P_Message->Init( Msg::MOUSE_C_RELEASE );
		}
//		else
//			return ERROR_INVALID_DD;

		//マウス情報更新
		Sui_MouseXOld		= ui_MouseX;
		Sui_MouseYOld		= ui_MouseY;
		Su4_MouseButtonOld	= u4_MouseButton;

		P_Message->Mu4_Data[0] = CR_DeviceData.Mu4_Data[0];		//マウス座標
		P_Message->Mu4_Data[1] = CR_DeviceData.Mu4_Data[1];		//マウスボタン情報
		break;
	}

	//その他は無効
	default:
		return ERROR_INVALID_DT;
	}

	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

