/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/FS/Keyboard/Keyboard.cpp
	概要		：	キーボード(ファイルシステム)
	詳細		：
	責任者		：
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/


/*******************************************************************************
	■デバイスデータ仕様(受け取り)
	Mu4_Data[0]	：	キーデータ
	Mu4_Data[1]	：	予約
	Mu4_Data[2]	：	予約
	■デバイスデータ仕様(返却)
	Mu4_Data[0]	：	31-9:予約, 8:ON/OFFフラグ, 7-0:キーコード
	Mu4_Data[1]	：	予約
	Mu4_Data[2]	：	予約
*******************************************************************************/


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"Keyboard.h"
#include	"DM.h"
#include	<KeyCode.h>
#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	//キーコード変換配列
	u1	GAu1_KeyCode[128] =
	{
		0x00,	KC::ESC,	KC::KEY_1,	KC::KEY_2,	KC::KEY_3,	KC::KEY_4,	KC::KEY_5,	KC::KEY_6,	KC::KEY_7,	KC::KEY_8,	KC::KEY_9,	KC::KEY_0,	KC::HYPHEN,	KC::HAT,	KC::BS,	KC::TAB,
		KC::Q,	KC::W,	KC::E,	KC::R,	KC::T,	KC::Y,	KC::U,	KC::I,								KC::O,	KC::P,	KC::ATMARK,	KC::LEFT_SB,	KC::ENTER,	KC::LCTRL,	KC::A,	KC::S,
		KC::D,	KC::F,	KC::G,	KC::H,	KC::J,	KC::K,	KC::L,	KC::SEMICOLON,						KC::COLON,	KC::KANJI,	KC::LSHIFT,	KC::RIGHT_SB,	KC::Z,	KC::X,	KC::C,	KC::V,
		KC::B,	KC::N,	KC::M,	KC::COMMA,	KC::PERIOD,	KC::SLASH,	KC::RSHIFT,	KC::TEN_ASTERISK,	KC::LALT,	KC::SPACE,	KC::CAPSLOCK,	KC::F1,	KC::F2,	KC::F3,	KC::F4,	KC::F5,
		KC::F6,	KC::F7,	KC::F8,	KC::F9,	KC::F10,	KC::NUMLOCK,	KC::SCROLLLOCK,	KC::TEN_7,		KC::TEN_8,	KC::TEN_9,	KC::TEN_HYPHEN,	KC::TEN_4,	KC::TEN_5,	KC::TEN_6,	KC::TEN_PLUS,	KC::TEN_1,
		KC::TEN_2,	KC::TEN_3,	KC::TEN_0,	KC::TEN_PERIOD,	0x00,	0x00,	0x00,	KC::F11,		KC::F12,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,		0x00,		0x00,	0x00,	0x00,	0x00,	0x00,						0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		KC::KANA,	0x00,		0x00,		0x00,	0x00,	0x00,	0x00,	0x00,					0x00,	KC::CONVERT,	0x00,	KC::NONCONVERT,	0x00,	KC::BACKSLASH,	0x00,	0x00,
	};

	//E0拡張キーコード変換配列
	u1	GAu1_E0KeyCode[128] =
	{
		0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,								0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,								0x00,	0x00,	0x00,	0x00,	KC::TEN_ENTER,	KC::RCTRL,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,								0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,	KC::TEN_SLASH,	0x00,								KC::PRINTSCREEN,	KC::RALT,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	KC::BREAK,	KC::HOME,						KC::UP,	KC::PAGEUP,	0x00,	KC::LEFT,	0x00,	KC::RIGHT,	0x00,	KC::END,
		KC::DOWN,	KC::PAGEDOWN,	KC::INSERT,	KC::DELETE,	0x00,	0x00,	0x00,	0x00,			0x00,	0x00,	0x00,	KC::LWIN,	KC::RWIN,	KC::MENU,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,								0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
		0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,								0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00
	};
}

extern DM		G_DM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	キーボード(FS)クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	キーボード(FS)クラスの初期化
	説明	：
	Include	：	Keyboard.h
	引数	：	u1 u1_DeviceID		デバイスID
				ui ui_FSMemID		ファイルシステムクラスのメモリ領域ID
	戻り値	：	void
*******************************************************************************/
s4		Keyboard::Init( u1 u1_DeviceID, ui ui_FSMemID )
{
	memset( MAu4_KeyState, 0, sizeof MAu4_KeyState );
	Mui_FSMemID		= ui_FSMemID;
	M_FSType		= FST::KEYBOARD;
	return SUCCESS;
}


/*******************************************************************************
	概要	：	ファイルシステム確認
	説明	：	対象のデバイスのファイルシステムがKeyboardか調べます。
	Include	：	Keyboard.h
	引数	：	u1 u1_DeviceID		確認するデバイス番号
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Keyboard::CheckFS( u1 u1_DeviceID )
{
	DevInfo	DevInfo = G_DM.GetDevInfo( u1_DeviceID );		//デバイス情報取得

	//エラー処理
	if( DevInfo.P_Driver->GetDeviceType() != DT::KEYBOARD )		//キーボードでなければエラー
		return ERROR_NOTSUPPORT;

	return SUCCESS;
}


/*******************************************************************************
	概要	：	デバイス・データ渡し
	説明	：	ウィンドウ管理からデバイス・データを受け取り、加工して返します。
	Include	：	Keyboard.h
	引数	：	DD* P_DeviceData		デバイスデータ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Keyboard::PassDD( DD* P_DeviceData )
{
	ui		ui_KeyData = P_DeviceData->Mu4_Data[0];
	ui		ui_KeyCode;
	bool	b_Press;		//true:Press, false:Release

	//ON/OFF判定
	b_Press = ( ( ui_KeyData & 0x80 ) == 0 );

	//E1拡張
	if( ( ui_KeyData & 0xff0000 ) == 0xe1 )
	{
		ui_KeyData &= 0x7f7f;
		if( ui_KeyData == 0x1d45 )
			SetKeyState( ui_KeyCode = KC::PAUSE, b_Press );
	}
	//E0拡張
	else if( ( ui_KeyData & 0xff00 ) == 0xe0 )
		SetKeyState( ui_KeyCode = GAu1_E0KeyCode[ui_KeyData & 0x7f], b_Press );
	//通常
	else if( ui_KeyData < 0x100 )
		SetKeyState( ui_KeyCode = GAu1_KeyCode[ui_KeyData & 0x7f], b_Press );
	//エラー
	else
		return ERROR_INVALIDKEYCODE;

	P_DeviceData->Mu4_Data[0] = ( b_Press << 8 ) | ( ui_KeyCode & 0xff );

	return SUCCESS;
}


/*******************************************************************************
	概要	：	読み取り
	説明	：	キーボードのファイルシステムからデータを読み取ります。
	Include	：	Keyboard.h
	引数	：	const char* CPc_FilePath		'/'から始まる絶対パス
				const char* CPc_NewFileName		新しい名前
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Keyboard::Rename( const char* CPc_FilePath, const char* CPc_NewFileName )
{
	return ERROR_NOTSUPPORT;
}


/*******************************************************************************
	概要	：	読み取り
	説明	：	キーボードのファイルシステムからデータを読み取ります。
	Include	：	Keyboard.h
	引数	：	const char* CPc_FilePath		'/'から始まる絶対パス
				ui ui_Offset		ファイル内のオフセット
				void* Pv_Addr		格納先アドレス
				ui ui_Size			処理するサイズ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Keyboard::Read( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size )
{
	//未実装
	return 0;
}


/*******************************************************************************
	概要	：	書き込み
	説明	：	キーボードのファイルシステムにデータを書き込みます
	Include	：	Keyboard.h
	引数	：	const char* CPc_FilePath		'/'から始まる絶対パス
				ui ui_Offset		ファイル内のオフセット
				void* Pv_Addr		格納先アドレス
				ui ui_Size			処理するサイズ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Keyboard::Write( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size )
{
	return ERROR_NOTSUPPORT;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	キーボード(FS)クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	キー押下状態のセット
	説明	：
	Include	：	Keyboard.h
	引数	：	u1 u1_KeyCode		変更するキーコード
				bool b_Press		Press/Releaseフラグ
	戻り値	：	void
*******************************************************************************/
void	Keyboard::SetKeyState( u1 u1_KeyCode, bool b_Press )
{
	ui		ui_ArrayNo	= u1_KeyCode >> 5;		//配列番号
	u4		u4_Mask		= 1 << ( u1_KeyCode & 0x1f );

	MAu4_KeyOld[ui_ArrayNo] = MAu4_KeyState[ui_ArrayNo];

	if( b_Press )	//Press
		MAu4_KeyState[ui_ArrayNo] |= u4_Mask;
	else			//Release
		MAu4_KeyState[ui_ArrayNo] &= ~u4_Mask;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

