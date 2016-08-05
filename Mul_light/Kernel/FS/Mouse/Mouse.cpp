/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/FS/Mouse/Mouse.cpp
	概要		：	マウス(ファイルシステム)
	詳細		：
	責任者		：
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/


/*******************************************************************************
	■デバイスデータ仕様(受け取り)
	Mu4_Data[0]	：	31-19:予約, 18:中央ボタン, 17:右ボタン, 16:左ボタン, 15-8:横方向移動量, 7-0:縦方向移動量
	Mu4_Data[1]	：	予約
	Mu4_Data[2]	：	予約
	■デバイスデータ仕様(返却)
	Mu4_Data[0]	：	31-16:X座標, 15-0:Y座標
	Mu4_Data[1]	：	31-8:予約, 7-4:予約, 2:中央ボタン, 1:右ボタン, 0:左ボタン
	Mu4_Data[2]	：	予約
*******************************************************************************/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"Mouse.h"
#include	"DM.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern DM		G_DM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	マウス(FS)クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	マウス(FS)クラスの初期化
	説明	：
	Include	：	Mouse.h
	引数	：	u1 u1_DeviceID		デバイスID
				ui ui_FSMemID		ファイルシステムクラスのメモリ領域ID
	戻り値	：	void
*******************************************************************************/
s4		Mouse::Init( u1 u1_DeviceID, ui ui_FSMemID )
{
	Msi_X	= 1024 >> 1;
	Msi_Y	= 768 >> 1;
	Mu4_Button		= 0;		//31-3:予約, 2:中央, 1:右, 0:左
	Mu4_ButtonOld	= 0;		//プレス/リリース検出用

	Mui_FSMemID		= ui_FSMemID;
	M_FSType		= FST::MOUSE;
	return SUCCESS;
}


/*******************************************************************************
	概要	：	ファイルシステム確認
	説明	：	対象のデバイスのファイルシステムがMouseか調べます。
	Include	：	Mouse.h
	引数	：	u1 u1_DeviceID		確認するデバイス番号
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Mouse::CheckFS( u1 u1_DeviceID )
{
	DevInfo	DevInfo = G_DM.GetDevInfo( u1_DeviceID );		//デバイス情報取得

	//エラー処理
	if( DevInfo.P_Driver->GetDeviceType() != DT::MOUSE )		//マウスでなければエラー
		return ERROR_NOTSUPPORT;

	return SUCCESS;
}


/*******************************************************************************
	概要	：	デバイス・データ渡し
	説明	：	ウィンドウ管理からデバイス・データを受け取り、加工して返します。
	Include	：	Mouse.h
	引数	：	DD* P_DeviceData		デバイスデータ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Mouse::PassDD( DD* P_DeviceData )
{
	//変数
	si		si_XDist = (s1)( P_DeviceData->Mu4_Data[0] >> 8 );
	si		si_YDist = (s1)( P_DeviceData->Mu4_Data[0] );
	ui		ui_ResX = G_GUI.GetResX();
	ui		ui_ResY = G_GUI.GetResY();


	Mu4_ButtonOld = Mu4_Button;

	Msi_X += si_XDist;
	Msi_Y -= si_YDist;		//Yは上下が逆。
	Mu4_Button = P_DeviceData->Mu4_Data[0] >> 16;

	if( Msi_X < 0 )
		Msi_X = 0;
	else if( Msi_X >= (si)ui_ResX )
		Msi_X = ui_ResX - 1;

	if( Msi_Y < 0 )
		Msi_Y = 0;
	else if( Msi_Y >= (si)ui_ResY )
		Msi_Y = ui_ResY - 1;

	P_DeviceData->Mu4_Data[0] = ( (u2)Msi_X << 16 ) | ( (u2)Msi_Y );
	P_DeviceData->Mu4_Data[1] = Mu4_Button;
	return SUCCESS;
}


/*******************************************************************************
	概要	：	読み取り
	説明	：	マウスのファイルシステムからデータを読み取ります。
	Include	：	Mouse.h
	引数	：	const char* CPc_FilePath		'/'から始まる絶対パス
				const char* CPc_NewFileName		新しい名前
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Mouse::Rename( const char* CPc_FilePath, const char* CPc_NewFileName )
{
	return ERROR_NOTSUPPORT;
}


/*******************************************************************************
	概要	：	読み取り
	説明	：	マウスのファイルシステムからデータを読み取ります。
	Include	：	Mouse.h
	引数	：	const char* CPc_FilePath		'/'から始まる絶対パス
				ui ui_Offset		ファイル内のオフセット
				void* Pv_Addr		格納先アドレス
				ui ui_Size			処理するサイズ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Mouse::Read( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size )
{
	return SUCCESS;
}


/*******************************************************************************
	概要	：	書き込み
	説明	：	マウスのファイルシステムにデータを書き込みます
	Include	：	Mouse.h
	引数	：	const char* CPc_FilePath		'/'から始まる絶対パス
				ui ui_Offset		ファイル内のオフセット
				void* Pv_Addr		格納先アドレス
				ui ui_Size			処理するサイズ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Mouse::Write( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size )
{
	return ERROR_NOTSUPPORT;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

