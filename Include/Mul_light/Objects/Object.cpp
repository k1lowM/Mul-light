/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/Objects/Object.cpp
	概要		：	Mul light API - オブジェクトクラス
	詳細		：	オブジェクトのベースクラスです。
					抽象的なクラスで、これ自体にあまり意味はありません。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"

#include	"Object.h"
#include	"Box.h"
#include	"Pack.h"
#include	"Window.h"
#include	"Content.h"
#include	"Text.h"

#include	"MLStdLib.h"
#include	"Rect.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
//パーセント単位のサイズをピクセル単位のサイズに変換(100％が65536として計算。)
#define	SIZE_PER2PX( _PERCENT_, _MAX_PX_ )		( ( ( _PERCENT_ ) * ( _MAX_PX_ ) ) >> 16 )



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	オブジェクトクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	オフセットの設定
	説明	：	オブジェクトのオフセット値を設定します。
				オブジェクト領域は、描画時に更新します。
	Include	：	Object.h
	引数	：	[si si_OffsetX = 0]		ピクセル単位のオフセット値
				[si si_OffsetY = 0]
				[HBP HBP = H_CENTER]	オフセットの基点
				[VBP VBP = V_CENTER]
	戻り値	：	void
*******************************************************************************/
void	Object::SetOffset( si si_OffsetX, si si_OffsetY, HBP HBP, VBP VBP )
{
	Msi_OffsetX	= si_OffsetX;
	Msi_OffsetY	= si_OffsetY;
	M_Flags.SetFlags( HBP | VBP );
}


/*******************************************************************************
	概要	：	サイズの設定
	説明	：	オブジェクトのサイズを設定します。
				オブジェクト領域は、描画時に更新します。
	Include	：	Object.h
	引数	：	[ui ui_Width = 0]			幅
				[ui ui_Height = 0]			高さ
				[WType WType = WIDTH_MAX]	幅の種類、単位
				[HType HType = HEIGHT_MAX]	高さの種類、単位
	戻り値	：	void
*******************************************************************************/
void	Object::SetSize( ui ui_Width, ui ui_Height, WType WType, HType HType )
{
	Mui_Width	= ui_Width;
	Mui_Height	= ui_Height;
	M_Flags.SetFlags( WType | HType );
}


/*******************************************************************************
	概要	：	親オブジェクト設定
	説明	：	全ての子オブジェクトの親オブジェクトを再帰的に設定します。
				親オブジェクトの所属ウィンドウが設定されていれば、所属ウィンドウも設定します。
				この関数は、全てのオブジェクトを一つに繋げる役割があります。

				シングルやパックやコンテントとかに記述したほうがいいかも。ダウンキャストは効率的じゃない。
				てか、ポインタではメンバに自由にアクセスできません。。。
				ここは、オブジェクトタイプによって分岐するだけがいいかも。いや、結局ダウンキャストがいるのか。。
				そうすれば、他で分岐がいらんし、メンバにも自由にアクセスできるし。
	Include	：	Object.h
	引数	：	Box* P_Parent		親オブジェクトのポインタ
	戻り値	：	void
*******************************************************************************/
void	Object::SetParent( Box* P_Parent )
{
	Object*	P_Child;

	//エラー処理
	if( P_Parent == NULL )
		return;							//NULLポインタエラー
	if( CheckInheritance( WINDOW ) )	//このオブジェクトがウィンドウを継承していればエラー。
		return;							//	ウィンドウの親はNULL

	MP_Parent = P_Parent;		//このオブジェクトの親オブジェクトを設定

	//親オブジェクトの所属するウィンドウのポインタがNULLなら終了(再帰終了)
	//	ついでに、メンバの所属ウィンドウを設定
	if( ( MP_Window = P_Parent->GetWindow() ) == NULL )
		return;

	//子オブジェクトの親と所属ウィンドウを設定(再帰的に、末端まで設定する)
	//このオブジェクトがシングルの場合
	if( CheckInheritance( SINGLE ) )
	{
		Single*	P_this = (Single*)this;		//シングルに強引にダウンキャストしたthisポインタ

		//子オブジェクトがNULLなら終了
		if( ( P_Child = P_this->GetChild() ) == NULL )
			return;

		P_Child->SetParent( P_this );		//子オブジェクトの親を設定(再帰)
		return;
	}

	//このオブジェクトがパックの場合
	if( CheckInheritance( PACK ) )
	{
		Pack*	P_this = (Pack*)this;		//パックに強引にダウンキャストしたthisポインタ
		P_this->Pack::SetParent();			//パックで複数の子オブジェクトの親を設定
		return;
	}

	//シングルでもパックでもなければ、再帰終了
	return;
}


/*******************************************************************************
	概要	：	各種ゲッター
	説明	：	メンバを返すだけなので、説明省略。
	Include	：	Object.h
*******************************************************************************/
Object::ObjType	Object::GetObjectType( void )	{ return M_ObjectType; }		//オブジェクトタイプの取得
Window*			Object::GetWindow( void )		{ return MP_Window; }			//所属するウィンドウのポインタ取得
Box*			Object::GetParent( void )		{ return MP_Parent; }			//親オブジェクトのポインタ取得
si				Object::GetOffsetX( void )		{ return Msi_OffsetX; }			//オフセットXの取得
si				Object::GetOffsetY( void )		{ return Msi_OffsetY; }			//オフセットYの取得
ui				Object::GetWidth( void )		{ return Mui_Width; }			//幅の取得
ui				Object::GetHeight( void )		{ return Mui_Height; }			//高さの取得
Rect			Object::GetArea( void )			{ return M_Area; }				//オブジェクト領域の取得
u4				Object::GetWType( void )		{ return M_Flags.GetFlags( WIDTH_MASK ); }		//幅タイプ取得
u4				Object::GetHType( void )		{ return M_Flags.GetFlags( HEIGHT_MASK ); }		//高さタイプ取得


/*******************************************************************************
	概要	：	最小サイズ取得
	説明	：	このオブジェクトの最低限必要とするサイズを返します。
	Include	：	Object.h
	引数	：	ui* Pui_Width		最小幅	を返すポインタ
				ui* Pui_Height			高さ
	戻り値	：	void
*******************************************************************************/
void	Object::GetMinSize( ui* Pui_Width, ui* Pui_Height )
{
	//クラスのタイプで分岐。
	if( CheckInheritance( SINGLE ) )		//シングル
		( (Single*)this )->Single::GetMinSize( Pui_Width, Pui_Height );
	else if( CheckInheritance( PACK ) )		//パック
		( (Pack*)this )->Pack::GetMinSize( Pui_Width, Pui_Height );
	else if( CheckInheritance( CONTENT ) )	//コンテント
	{
		*Pui_Width	= ( (Content*)this )->Content::GetWidth();
		*Pui_Height	= ( (Content*)this )->Content::GetHeight();
	}
}


/*******************************************************************************
	概要	：	有効領域取得
	説明	：	このオブジェクトの有効な領域を返します。
	Include	：	Object.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
Rect	Object::GetValidArea( void )
{
	Box*	P_Parent;
	Rect	ParentArea;
	Rect	ValidArea = M_Area;

	//親を全てたどり、有効領域を求める
	for( P_Parent = MP_Parent; P_Parent != NULL; P_Parent = P_Parent->GetParent() )
	{
		ParentArea = P_Parent->Box::GetChildArea( this );
		ValidArea.And( ParentArea );
	}
	return ValidArea;
}


/*******************************************************************************
	概要	：	オブジェクト描画
	説明	：	各オブジェクトによってオプションが違うので、仮想関数化して処理を分散させたい。。
	Include	：	Object.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Object::Drawing( void )
{
	//下の階層から。
	if( CheckInheritance( TEXT ) )
		( (Text*)this )->Text::Drawing();

	//その他。完全に仮想関数と同じ機能をifで実装してる。。
	else
	{
		UpdateArea();		//オブジェクト領域更新

		//システムコール
		SCD::DrawObject	SCD_DrawObject;
		SCD_DrawObject.MP_Object		= this;
		SCD_DrawObject.M_ChArea			= GetValidArea();
		SCD_DrawObject.MAu4_Option[0]	= 0;
		SCD_DrawObject.MAu4_Option[1]	= 0;
		SC::SystemCall( SC::DRAW_OBJECT, &SCD_DrawObject );
	}
}


/*******************************************************************************
	概要	：	オブジェクト描画(子オブジェクト含む)
	説明	：	仮想関数が使えれば、Box,Packなどにこの関数を持たせ、実行させる。
				シングルやパックやコンテントとかに記述したほうがいいかも。ダウンキャストは効率的じゃない。
				てか、ポインタではメンバに自由にアクセスできません。。。
				ここは、オブジェクトタイプによって分岐するだけがいいかも。いや、結局ダウンキャストがいるのか。。
				そうすれば、他で分岐がいらんし、メンバにも自由にアクセスできるし。
	Include	：	Object.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Object::DrawingAll( void )
{
	Object*	P_Child;

	//このオブジェクトを描画
	Drawing();

	//子オブジェクトを描画(再帰的に、末端まで描画する)
	//このオブジェクトがシングルの場合
	if( CheckInheritance( SINGLE ) )
	{
		Single*	P_this = (Single*)this;		//シングルに強引にダウンキャストしたthisポインタ

		//子オブジェクトがNULLなら終了
		if( ( P_Child = P_this->GetChild() ) == NULL )
			return;

		P_Child->DrawingAll();		//子オブジェクトの親を設定(再帰)
		return;
	}

	//このオブジェクトがパックの場合
	if( CheckInheritance( PACK ) )
	{
		Pack*	P_this = (Pack*)this;		//パックに強引にダウンキャストしたthisポインタ
		P_this->Pack::DrawingAll();			//パックで複数の子オブジェクトの親を設定
		return;
	}

	//シングルでもパックでもなければ、再帰終了
	return;
}


/*******************************************************************************
	概要	：	継承チェック
	説明	：	引数で指定したオブジェクトタイプのクラスを、
				　このクラスのオブジェクトが継承しているか調べます。
	Include	：	Object.h
	引数	：	ObjType CheckType		継承関係を調べるオブジェクトタイプ
	戻り値	：	bool		true:継承している, false:継承していない
*******************************************************************************/
bool	Object::CheckInheritance( ObjType CheckType )
{
	u4		u4_CheckType = CheckType;
	u4		u4_Mask;

	//マスクを4ビットづつずらしてマスクを作成	(少しオーバーヘッドだけど、最大でも6,7回のループ。)
	for( u4_Mask = 0x0fffffff; u4_CheckType & u4_Mask; u4_Mask >>= 4 )
	{}

	return u4_CheckType == ( M_ObjectType & ~u4_Mask );		//比較結果を返す
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	オブジェクトクラス：プロテクティッドメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	オブジェクトクラスの初期化
	説明	：
	Include	：	Object.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Object::Init( void )
{
	M_ObjectType		= OBJECT;	//オブジェクトタイプ
	MP_Window			= NULL;		//所属するウィンドウ
	MP_Parent			= NULL;		//親オブジェクト

	Msi_OffsetX			= 0;		//オフセットX
	Msi_OffsetY			= 0;		//オフセットY
	Mui_Width			= 0;		//幅
	Mui_Height			= 0;		//高さ

	M_Area.Init();					//オブジェクト領域初期化
	M_Flags.Init();					//各種フラグ初期化
}


/*******************************************************************************
	概要	：	各種セッター
	説明	：	メンバを設定するだけなので、説明省略
	Include	：	Object.h
*******************************************************************************/
void	Object::SetObjectType( ObjType ObjectType ){ M_ObjectType = ObjectType; }	//オブジェクトタイプ設定


/*******************************************************************************
	概要	：	オブジェクト領域更新
	説明	：
	Include	：	Object.h
	引数	：	void
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Object::UpdateArea( void )
{
	Object*	P_Parent;				//親オブジェクト
	Rect	SelfArea;				//親の子領域
	ui		ui_PxWidth;				//ピクセル単位の幅
	ui		ui_PxHeight;			//ピクセル単位の高さ
	u4		u4_WType = 0;			//幅タイプ
	u4		u4_HType = 0;			//高さタイプ
	s4		s4_Return;


	//親オブジェクトがNULL(ウィンドウか、エラー)
	if( MP_Parent == NULL )
	{
		//ウィンドウならウィンドウの絶対座標を更新する。
		if( CheckInheritance( WINDOW ) )
			return ( (Window*)this )->Window::UpdateArea();
		return ERROR_PARENT_IS_NULL;		//親無し
	}

	//親の領域を更新、親の子領域(自領域)を取得
	if( ( s4_Return = MP_Parent->UpdateArea() ) < 0 )	//親の領域を更新(再帰)
		return s4_Return;
	SelfArea = MP_Parent->Box::GetChildArea( this );	//親の子領域(自領域)を取得

	//自身がMINでないとき、親にMINがあるか
	for( P_Parent = this; P_Parent != NULL; P_Parent = P_Parent->GetParent() )
	{
		//MINがあれば、自身はMINに。
		if( u4_WType != WIDTH_MIN )
			u4_WType = P_Parent->GetWType();
		//MINがあれば、自身はMINに。
		if( u4_HType != HEIGHT_MIN );
			u4_HType = P_Parent->GetHType();
		//どちらもMINになれば終了
		if( ( u4_WType | u4_HType ) == ( WIDTH_MIN | HEIGHT_MIN ) )
			break;
	}

	//どちらかがMINなら子のサイズを取得
	if( u4_WType == WIDTH_MIN || u4_HType == HEIGHT_MIN )
		GetMinSize( &ui_PxWidth, &ui_PxHeight );

	//親にMINがない
	if( u4_WType != WIDTH_MIN )
	{
		//幅タイプによる分岐
		switch( M_Flags.GetFlags( WIDTH_MASK ) )
		{
		case WIDTH_MAX:		//==親の子領域
			ui_PxWidth		= SelfArea.GetWidth();
			break;
		case WIDTH_PERCENT:	//親の子領域を100%(65536)として計算
			ui_PxWidth		= SIZE_PER2PX( Mui_Width, SelfArea.GetWidth() );
			break;
		case WIDTH_PIXEL:	//==Mui_Width
			ui_PxWidth		= Mui_Width;
			break;
		}
	}
	if( u4_HType != HEIGHT_MIN )
	{
		//高さタイプによる分岐
		switch( M_Flags.GetFlags( HEIGHT_MASK ) )
		{
		case HEIGHT_MAX:
			ui_PxHeight		= SelfArea.GetHeight();
			break;
		case HEIGHT_PERCENT:
			ui_PxHeight		= SIZE_PER2PX( Mui_Height, SelfArea.GetHeight() );
			break;
		case HEIGHT_PIXEL:
			ui_PxHeight		= Mui_Height;
			break;
		}
	}


	//ようやくサイズが決まった・・・
	//ここから、オフセットを処理して、加算。
//M_Area = SelfArea;


	//水平基点による分岐
	switch( M_Flags.GetFlags( HBP_MASK ) )
	{
	case H_CENTER:
		M_Area.Msi_Left		= Msi_OffsetX + ( ( SelfArea.Msi_Left + SelfArea.Msi_Right ) >> 1 ) - ( ui_PxWidth >> 1 );
		M_Area.Msi_Right	= Msi_OffsetX + ( ( SelfArea.Msi_Left + SelfArea.Msi_Right ) >> 1 ) + ( ui_PxWidth >> 1 );
		break;
	case H_LEFT:
		M_Area.Msi_Left		= Msi_OffsetX + SelfArea.Msi_Left;
		M_Area.Msi_Right	= Msi_OffsetX + SelfArea.Msi_Left + ui_PxWidth;
		break;
	case H_RIGHT:
		M_Area.Msi_Left		= Msi_OffsetX + SelfArea.Msi_Right - ui_PxWidth;
		M_Area.Msi_Right	= Msi_OffsetX + SelfArea.Msi_Right;
		break;
	}
	//垂直基点による分岐
	switch( M_Flags.GetFlags( VBP_MASK ) )
	{
	case V_CENTER:
		M_Area.Msi_Top		= Msi_OffsetY + ( ( SelfArea.Msi_Top + SelfArea.Msi_Bottom ) >> 1 ) - ( ui_PxHeight >> 1 );
		M_Area.Msi_Bottom	= Msi_OffsetY + ( ( SelfArea.Msi_Top + SelfArea.Msi_Bottom ) >> 1 ) + ( ui_PxHeight >> 1 );
		break;
	case V_TOP:
		M_Area.Msi_Top		= Msi_OffsetY + SelfArea.Msi_Top;
		M_Area.Msi_Bottom	= Msi_OffsetY + SelfArea.Msi_Top + ui_PxHeight;
		break;
	case V_BOTTOM:
		M_Area.Msi_Top		= Msi_OffsetY + SelfArea.Msi_Bottom - ui_PxHeight;
		M_Area.Msi_Bottom	= Msi_OffsetY + SelfArea.Msi_Bottom;
		break;
	}

	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

