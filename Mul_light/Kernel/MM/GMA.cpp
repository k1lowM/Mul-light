/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/MM/GMA.cpp
	概要		：	全域メモリ割り当て
	詳細		：	Global Memory Allocation
					メモリ全体の割り当てをします。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"GMA.h"

#include	"GUI.h"
#include	<stdio.h>
#include	<Math.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
namespace
{
	enum
	{
		MEMORYMAP_BASE		= WORK_BASE + 0x100 + sizeof (MemoryMap),	//メモリマップのベースアドレス
		FREESPACE_BASE		= 0x00800000,			//空き領域のベースアドレス
		MEMORY_LIMIT		= 0xffffffff,			//メモリの最大値(4GiB)
	};
}

#define	NUM_MEMORYMAP	( *(u2*)( WORK_BASE + 0x100 ) )				//メモリマップ数



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	GMAクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	GMAクラスの初期化
	説明	：	ブート時に取得したメモリデータから、メモリマップを作成します。
	Include	：	GMA.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	GMA::Init( void )
{
	//メモリマップタイプ
	enum
	{
		MEMORY_TYPE_FREE = 1,		//空き領域
		MEMORY_TYPE_RESERVED,		//予約領域。
		MEMORY_TYPE_ACPI,			//ACPI領域。
		MEMORY_TYPE_NVS,			//ACPI NVS領域。
		MEMORY_TYPE_UNUSABLE,		//使えない。
	};

	//変数
	ui			ui_SrcCount;
	ui			ui_DestCount = 0;
	const u4	Cu4_NumMemoryMap = NUM_MEMORYMAP;
	MemoryMap*	P_MemoryMap	= (MemoryMap*)MEMORYMAP_BASE;

	//メモリマップ取得
	for( ui_SrcCount = 0; ui_SrcCount < Cu4_NumMemoryMap; ui_SrcCount++ )
	{
		//4GiBのリミットを越えたら終了。
		if( P_MemoryMap[ui_SrcCount].u8_Base > MEMORY_LIMIT )
			break;

		//空き領域でないなら次へ。
		if( P_MemoryMap[ui_SrcCount].u8_Type != MEMORY_TYPE_FREE )
			continue;
		//ベースアドレス+サイズがFREESPACE_BASE未満なら次へ。
		if( P_MemoryMap[ui_SrcCount].u8_Base + P_MemoryMap[ui_SrcCount].u8_Size < FREESPACE_BASE )
			continue;
		//ページ単位にアライメントした結果、空き領域が1ページに満たない場合も次へ
		if( SizeAlign( (u4)P_MemoryMap[ui_SrcCount].u8_Base, (u4)P_MemoryMap[ui_SrcCount].u8_Size ) < PAGE )
			continue;

		//先頭のメモリマップ
		if( ui_DestCount == 0 )
		{
			//空き領域の設定
			M_LM.Init( &M_MAL[ui_DestCount] );
			M_MAL[ui_DestCount].Mu4_Base	= BaseAlign( ( FREESPACE_BASE < P_MemoryMap[ui_SrcCount].u8_Base )? P_MemoryMap[ui_SrcCount].u8_Base : FREESPACE_BASE );
			M_MAL[ui_DestCount].Mu4_Size	= SizeAlign( M_MAL[ui_DestCount].Mu4_Base, (u4)P_MemoryMap[ui_SrcCount].u8_Base + (u4)P_MemoryMap[ui_SrcCount].u8_Size - M_MAL[ui_DestCount].Mu4_Base );
			M_MAL[ui_DestCount].M_Flag16.Init( MAL::USAGE_FREE );		//空き領域に設定
//			M_MAL[ui_DestCount].Init( , , MAL::USAGE_FREE );
			ui_DestCount++;
		}
		//先頭以外
		else
		{
			//使用不可領域の設定
			M_LM.JoinTail( &M_MAL[ui_DestCount] );
			M_MAL[ui_DestCount].Mu4_Base	= M_MAL[ui_DestCount - 1].Mu4_Base + M_MAL[ui_DestCount - 1].Mu4_Size;
			M_MAL[ui_DestCount].Mu4_Size	= (u4)P_MemoryMap[ui_SrcCount].u8_Base - M_MAL[ui_DestCount].Mu4_Base;
			M_MAL[ui_DestCount].M_Flag16.Init( MAL::USAGE_RESERVED );		//予約領域に設定
			ui_DestCount++;

			//空き領域の設定
			M_LM.JoinTail( &M_MAL[ui_DestCount] );
			M_MAL[ui_DestCount].Mu4_Base	= BaseAlign( (u4)P_MemoryMap[ui_SrcCount].u8_Base );
			M_MAL[ui_DestCount].Mu4_Size	= SizeAlign( (u4)P_MemoryMap[ui_SrcCount].u8_Base, (u4)P_MemoryMap[ui_SrcCount].u8_Size );
			M_MAL[ui_DestCount].M_Flag16.Init( MAL::USAGE_FREE );		//空き領域に設定
			//サイズを4GiBのラインまでに制限
			if( P_MemoryMap[ui_SrcCount].u8_Base + P_MemoryMap[ui_SrcCount].u8_Size > (u8)( MEMORY_LIMIT ) )
				M_MAL[ui_DestCount].Mu4_Size = -M_MAL[ui_DestCount].Mu4_Base;		//0x1 0000 0000 == 0
			ui_DestCount++;
		}
	}

	ShowMemoryMap();

	DP( "GMA::Init()\t\t\t\t\t\t\t\t\t[  OK  ]" );
}


/*******************************************************************************
	概要	：	メモリ領域確保
	説明	：	指定のサイズの空き領域を検索して、ベースアドレスを返します。
				引数のサイズはページ単位に切り上げられます。
	Include	：	GMA.h
	引数	：	ui ui_ByteSize		取得するメモリのサイズ(Byte単位)
	戻り値	：	ui		確保したメモリ領域のID((ui)-1ならエラー)
*******************************************************************************/
ui		GMA::Create( ui ui_ByteSize )
{
	return MA::Create( INT_CEIL_BIN( ui_ByteSize, PAGE - 1 ) );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	GMAクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ベースを4KiB境界に揃える
	説明	：	ベースを4KiBの境界に、切り上げで揃えます。
				空き領域用です。
	Include	：	GMA.h
	引数	：	u4 u4_Base	揃える対象のベースアドレス
	戻り値	：	u4		アライン後のベースアドレス
*******************************************************************************/
u4		GMA::BaseAlign( u4 u4_Base )
{
	return INT_CEIL_BIN( u4_Base, PAGE - 1 );
}


/*******************************************************************************
	概要	：	サイズを4KiB境界に揃える
	説明	：	ベースを4KiB境界に切り上げて、サイズをその分引いた後、
				サイズを4KiB境界に切り捨てて揃えます。
				空き領域用です。
	Include	：	GMA.h
	引数	：	u4 u4_Base	アライン前のベースアドレス
				u4 u4_Size	揃える対象のサイズ
	戻り値	：	u4		アライン後のサイズ
*******************************************************************************/
u4		GMA::SizeAlign( u4 u4_Base, u4 u4_Size )
{
	//サイズが1Pageに満たない場合はエラー。
	if( u4_Size < PAGE )
		return 0;

	//4KiB境界に切り上げたベースと元のベースの差分をサイズから引く。
	u4_Size -= BaseAlign( u4_Base ) - u4_Base;
	//サイズを4KiB境界に切り捨てて返す。
	return INT_FLOOR_BIN( u4_Size, PAGE - 1 );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

