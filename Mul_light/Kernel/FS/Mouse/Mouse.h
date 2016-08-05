/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/FS/Mouse/Mouse.h
	概要		：	Mouse(ファイルシステム)
	詳細		：
	責任者		：
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"FS.h"
#include	<Device/DD.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	マウスクラス
*******************************************************************************/
class	Mouse : public FS
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NOTSUPPORT,		//サポート対象外

		SUCCESS = _SUCCESS_,
	};

//変数
private:
	si		Msi_X, Msi_Y;		//マウス座標
	u4		Mu4_Button;			//31-3:予約, 2:中央, 1:右, 0:左
	u4		Mu4_ButtonOld;		//プレス/リリース検出用

//メソッド
public:
	s4		Init( u1 u1_DeviceID, ui ui_FSMemID );

	s4		CheckFS( u1 u1_DeviceID );		//ファイルシステム確認

	s4		PassDD( DD* P_DeviceData );

	s4		Rename( const char* CPc_FilePath, const char* CPc_NewFileName );				//ファイル名変更
	s4		Read( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size );		//読み取り
	s4		Write( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size );		//書き込み
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
