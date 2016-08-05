/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Single.cpp
	概要		：	Mul light API - シングルクラス
	詳細		：	一つの子オブジェクトを持つことができるボックスです。
					抽象的なクラスで、これ自体にあまり意味はありません。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"

#include	"Single.h"
#include	"Pack.h"
#include	"Content.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	シングルクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	子オブジェクト設定
	説明	：
	Include	：	Single.h
	引数	：	Object* P_Child		子オブジェクトのポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Single::SetChild( Object* P_Child )
{
	//エラー処理
	if( P_Child == NULL )
		return ERROR_NULLPOINTER;		//NULLポインタエラー
	if( !P_Child->CheckInheritance( OBJECT ) )
		return ERROR_NOTOBJECT;			//オブジェクトでない。

	MP_Child = P_Child;				//子オブジェクトを設定
	P_Child->SetParent( this );		//子の親を設定
	return SUCCESS;
}


/*******************************************************************************
	概要	：	子オブジェクト取得
	説明	：
	Include	：	Single.h
	引数	：	void
	戻り値	：	Object*		オブジェクトのポインタ
*******************************************************************************/
Object*	Single::GetChild( void ){ return MP_Child; }


/*******************************************************************************
	概要	：	最小サイズ取得
	説明	：	このオブジェクトの最低限必要とするサイズを返します。
				最低限必要なサイズは、子オブジェクトの内容のサイズを
				　一番下の子まで再帰で調べて算出します。
				子にパックが含まれていた場合、パックの最小サイズを調べます。
				この関数はパッククラスのGetMinSize()と連携して動作します。
	Include	：	Single.h
	引数	：	ui* Pui_Width		最小幅	を返すポインタ
				ui* Pui_Height			高さ
	戻り値	：	void
*******************************************************************************/
void	Single::GetMinSize( ui* Pui_Width, ui* Pui_Height )
{
	//子がNULLなら終了。(再帰終了)
	if( MP_Child == NULL )
	{
		*Pui_Width	= 0;
		*Pui_Height	= 0;
		return;
	}

	//子の最小サイズを取得
	MP_Child->Object::GetMinSize( Pui_Width, Pui_Height );

	//パディングを加算
	*Pui_Width	+= Box::M_Padding.u1_Left + Box::M_Padding.u1_Right;
	*Pui_Height	+= Box::M_Padding.u1_Top + Box::M_Padding.u1_Bottom;
}


/*******************************************************************************
	概要	：	子領域取得
	説明	：	引数で指定した子オブジェクトの領域を返します。
	Include	：	Single.h
	引数	：	Object* P_Child		不使用(仮想関数を使うことを想定して・・・)
	戻り値	：	Rect		子領域
*******************************************************************************/
Rect	Single::GetChildArea( Object* P_Child )
{
	Rect	ChArea;

	//ウィンドウの場合
	if( CheckInheritance( WINDOW ) )
		ChArea.SetRect( 0, 0, Object::M_Area.GetWidth(), Object::M_Area.GetHeight() );
	//ウィンドウ以外
	else
		ChArea = Object::M_Area;

	//パディング分領域縮小
	ChArea.Contract(
			Box::M_Padding.u1_Top,
			Box::M_Padding.u1_Left,
			Box::M_Padding.u1_Right,
			Box::M_Padding.u1_Bottom );
	return ChArea;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	シングルクラス：プロテクティッドメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	シングルクラスの初期化
	説明	：
	Include	：	Single.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Single::Init( void )
{
	Box::Init();
	Object::SetObjectType( Object::SINGLE );

	MP_Child	= NULL;		//子オブジェクト
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

