/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Object.h
	概要		：	Mul light API - オブジェクトクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*******************************************************************************
	■オブジェクト階層図
	　Object オブジェクト
		├Box			：ボックス
		│	├Single		：シングルボックス
		│	│	├Caption		：キャプションボックス
		│	│	│	├Window		：ウィンドウ
		│	│	│	└Frame			：フレーム
		│	│	├Button		：ボタン
		│	│	│	├CheckBox		：チェックボックス
		│	│	│	└RadioButton	：ラジオボタン
		│	│	└Item			：アイテム
		│	│		└MenuItem		：メニューアイテム
		│	└Pack			：パックボックス
		│		├Menu			：メニュー
		│		└Adjustable	：調整可能ボックス
		└Content		：コンテント
			├Edit			：テキスト編集
			│	├OneLine		：一行入力
			│	└TextBox		：テキストボックス
			└Text			：テキスト
*******************************************************************************/


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Flag.h"
#include	"SC.h"
#include	"Rect.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス前方宣言
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
class	Window;
class	Box;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	オブジェクトクラス
*******************************************************************************/
class	Object
{
//定数
public:
	//オブジェクト・タイプ
	enum	ObjType
	{
		OBJECT			= 0x10000000,		//オブジェクト
			BOX				= OBJECT	| 0x01000000,		//ボックス
			CONTENT			= OBJECT	| 0x02000000,		//コンテント
				SINGLE			= BOX		| 0x00100000,		//シングルボックス
				PACK			= BOX		| 0x00200000,		//パックボックス
				EDIT			= CONTENT	| 0x00100000,		//テキスト編集
				TEXT			= CONTENT	| 0x002000000,		//テキスト
					CAPTION			= SINGLE	| 0x00010000,		//キャプションボックス
					BUTTON			= SINGLE	| 0x00020000,		//ボタン
					ITEM			= SINGLE	| 0x00030000,		//アイテム
					MENU			= PACK		| 0x00010000,		//メニュー
					ADJUSTABLE		= PACK		| 0x00020000,		//アジャスタブルボックス
					ONELINE			= EDIT		| 0x000100000,		//一行テキスト編集
					TEXT_BOX		= EDIT		| 0x000200000,		//テキストボックス
						WINDOW			= CAPTION	| 0x00001000,		//ウィンドウ
						FRAME			= CAPTION	| 0x00002000,		//フレーム
						CHECK_BOX		= BUTTON	| 0x00001000,		//チェックボックス
						RADIO_BUTTON	= BUTTON	| 0x00002000,		//ラジオボタン
						MENU_ITEM		= ITEM		| 0x00001000,		//メニューアイテム
	};

	//フラグ
	//----------------------------------------------------------
		//基点(Base Point)
		enum	HBP
		{
			H_CENTER		= 0x00030000,
			H_LEFT			= 0x00030002,
			H_RIGHT			= 0x00030003,
		};
		enum	VBP
		{
			V_CENTER		= 0x000c0000,
			V_TOP			= 0x000c0008,
			V_BOTTOM		= 0x000c000c,
		};

		//幅、高さの種類、単位
		enum	WType
		{
			WIDTH_MAX		= 0x00f00000,		//サイズ最大(親の子領域に合わせる)
			WIDTH_MIN		= 0x00f00010,		//サイズ最小(最下層の子要素に合わせる)
			WIDTH_PERCENT	= 0x00f00020,		//単位：パーセント(x/65536 65536:領域の最大値。65537以上でも問題ない)
			WIDTH_PIXEL		= 0x00f00030,		//単位：ピクセル
		};
		enum	HType
		{
			HEIGHT_MAX		= 0x0f000000,
			HEIGHT_MIN		= 0x0f000100,
			HEIGHT_PERCENT	= 0x0f000200,
			HEIGHT_PIXEL	= 0x0f000300,
		};

		//
		enum
		{
			NOUPDATE_AREA	= 0x10000000,		//領域更新なし
			UPDATE_AREA		= 0x10001000,		//領域更新あり
		};

		//フラグマスク
		enum
		{
			HBP_MASK		= H_CENTER,
			VBP_MASK		= V_CENTER,
			BP_MASK			= HBP_MASK | VBP_MASK,
			WIDTH_MASK		= WIDTH_MAX,
			HEIGHT_MASK		= HEIGHT_MAX,
			SIZE_MASK		= WIDTH_MASK | HEIGHT_MASK,
		};
	//----------------------------------------------------------

	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_PARENT_IS_NULL,		//親がNULL
		OBJECT_ERROR_MAX,

		SUCCESS = _SUCCESS_,
		OBJECT_SUCCESS_MAX,
	};

//変数
protected:
	ObjType	M_ObjectType;		//オブジェクトの種類
	Window*	MP_Window;			//所属するウィンドウ
	Box*	MP_Parent;			//親オブジェクト

	si		Msi_OffsetX;		//HBPを基点とするオフセット
	si		Msi_OffsetY;		//VBPを基点とするオフセット
	ui		Mui_Width;			//幅( WTypeがMAX,MINの時は無効 )
	ui		Mui_Height;			//高さ( HTyepがMAX,MINの時は無効 )

	Rect	M_Area;				//オブジェクト領域(ウィンドウバッファの左上を基点とする座標)
	Flag16	M_Flags;			//各種フラグ

//メソッド
public:
	//セッター
	void	SetOffset( si si_OffsetX = 0, si si_OffsetY = 0, HBP HBP = H_CENTER, VBP VBP = V_CENTER );
	void	SetSize( ui ui_Width = 0, ui ui_Height = 0, WType WType = WIDTH_MAX, HType HType = HEIGHT_MAX );
	void	SetParent( Box* P_Parent );

	//ゲッター
	ObjType	GetObjectType( void );
	Window*	GetWindow( void );
	Box*	GetParent( void );
	si		GetOffsetX( void );
	si		GetOffsetY( void );
	ui		GetWidth( void );
	ui		GetHeight( void );
	Rect	GetArea( void );
	u4		GetWType( void );
	u4		GetHType( void );
	void	GetMinSize( ui* Pui_Width, ui* Pui_Height );		//最小サイズ取得
	Rect	GetValidArea( void );		//有効領域取得

	//その他
	void	Drawing( void );			//オブジェクト描画
	void	DrawingAll( void );			//オブジェクト描画(全ての子オブジェクトを含む)

	bool	CheckInheritance( ObjType CheckType );		//継承チェック

protected:
	void	Init( void );

	//セッター
	void	SetObjectType( ObjType ObjectType );

	s4		UpdateArea( void );		//オブジェクト領域更新
};


/*******************************************************************************
	オブジェクト描画
*******************************************************************************/
namespace	SCD
{
	class	DrawObject : public SCDBase
	{
	public:
		Object*	MP_Object;		//対象オブジェクトポインタ(ローカル)
		Rect	M_ChArea;		//対象オブジェクトの親の子領域
		u4		MAu4_Option[2];	//オプションデータ
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
