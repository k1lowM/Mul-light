/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Pack.h
	概要		：	Mul light API - パッククラス
	詳細		：	複数の子オブジェクトを持つことができるボックスです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Box.h"
#include	"ObjectList.h"
#include	"Flag.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	前方宣言
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
class	Rect;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	パッククラス
*******************************************************************************/
class	Pack : public Box
{
//定数
public:
	enum	Aspect		//縦／横方向
	{
		HORIZONTAL		= 0x80000000,			//横方向のパック
		VERTICAL		= 0x80008000,			//縦方向のパック
	};
	enum	Uniformity	//均一フラグ
	{
		BALANCED		= 0x40000000,			//バランスのとれたサイズ(子オブジェクトのサイズから相対的に割り当てる)
		UNIFORMITY		= 0x40004000,			//均一のサイズ
	};

private:
	enum
	{
		NUM_LIST		= 32,

		//フラグマスク
		ASPECT_MASK		= HORIZONTAL,
		UNIFORMITY_MASK	= BALANCED,
	};

//変数
protected:
	ObjList	MA_ObjList[NUM_LIST];	//本来は動的確保すべき。
	LM		M_ObjLM;				//子オブジェクトリスト管理
	Flag16	M_Flags;				//全体フラグ

//メソッド
public:
	void	Init( Aspect Aspect = HORIZONTAL, Uniformity Uniformity = BALANCED );

	void	AddChild( Object* P_Child );					//子オブジェクト追加
	void	SetParent( void );								//親オブジェクト設定

	void	GetMinSize( ui* Pui_Width, ui* Pui_Height );	//最小サイズ取得
	Object*	GetChild( Object* P_PrevChild );				//子オブジェクトを順に取得
	Rect	GetChildArea( Object* P_Child );				//子領域取得
	ObjList*	GetHeadChildList( void );					//先頭の子オブジェクトリスト取得
	ObjList*	GetTailChildList( void );					//末尾の子オブジェクトリスト取得

	void	DrawingAll( void );								//オブジェクト描画(全ての子オブジェクトを含む)
	void	Partitioning( void );							//子オブジェクト領域分配
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
