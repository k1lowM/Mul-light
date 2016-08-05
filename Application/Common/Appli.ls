/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Application/Common/Appli.ls
	概要		：	アプリケーションのリンカスクリプト
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	コマンド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
OUTPUT_FORMAT( binary );
OUTPUT_ARCH( i386 );



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	セクション配置
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
SECTIONS
{
	.head :
	{
		LONG( ADDR( .align ) + 1 )
		BYTE( 0x4d ) BYTE( 0x4c ) BYTE( 0x41 ) BYTE( 0x70 )
		BYTE( 0x70 ) BYTE( 0x6c ) BYTE( 0x69 ) BYTE( 0x00 )
		LONG( 0x10000 )
		LONG( 0x10000 )
	}

	.text 0:
		AT( 0x20 )
	{
		*(.text)
	}

	.data :
		AT( LOADADDR( .text ) + SIZEOF( .text ) ) SUBALIGN( 4 )
	{
		*(.data)
	}

	.bss :
	{
		*(.bss)
	}

	.rodata :
	{
		*(.rodata*)
	}

	/DISCARD/ :
	{
		*(*)
	}

	.align ALIGN( 0x20 ) + 0x1f:
	{
		BYTE( 0 )
	}
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

