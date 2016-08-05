/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/IM/IntEntry.S
	概要		：	割り込みエントリー
	詳細		：	割り込みルーチンの入り口となる関数を定義します。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
	.macro INT_INITIALIZE
	pushal
	pushl	%ds
	pushl	%es
	movw	%ss,	%ax
	movw	%ax,	%ds
	movw	%ax,	%es
	.endm

	.macro INT_FINALIZE
	popl	%es
	popl	%ds
	popal
	iret
	.endm



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	割り込みエントリールーチン
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
	.global	_Z10IntEntry00v
	.extern	_Z5Int00ttmmmmmmmmmtm
_Z10IntEntry00v:
	INT_INITIALIZE
	call	_Z5Int00ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry01v
	.extern	_Z5Int01ttmmmmmmmmmtm
_Z10IntEntry01v:
	INT_INITIALIZE
	call	_Z5Int01ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry02v
	.extern	_Z5Int02ttmmmmmmmmmtm
_Z10IntEntry02v:
	INT_INITIALIZE
	call	_Z5Int02ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry03v
	.extern	_Z5Int03ttmmmmmmmmmtm
_Z10IntEntry03v:
	INT_INITIALIZE
	call	_Z5Int03ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry04v
	.extern	_Z5Int04ttmmmmmmmmmtm
_Z10IntEntry04v:
	INT_INITIALIZE
	call	_Z5Int04ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry05v
	.extern	_Z5Int05ttmmmmmmmmmtm
_Z10IntEntry05v:
	INT_INITIALIZE
	call	_Z5Int05ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry06v
	.extern	_Z5Int06ttmmmmmmmmmtm
_Z10IntEntry06v:
	INT_INITIALIZE
	call	_Z5Int06ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry07v
	.extern	_Z5Int07ttmmmmmmmmmtm
_Z10IntEntry07v:
	INT_INITIALIZE
	call	_Z5Int07ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry08v
	.extern	_Z5Int08ttmmmmmmmmmtm
_Z10IntEntry08v:
	INT_INITIALIZE
	call	_Z5Int08ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry09v
	.extern	_Z5Int09ttmmmmmmmmmtm
_Z10IntEntry09v:
	INT_INITIALIZE
	call	_Z5Int09ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry0Av
	.extern	_Z5Int0Attmmmmmmmmmtm
_Z10IntEntry0Av:
	INT_INITIALIZE
	call	_Z5Int0Attmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry0Bv
	.extern	_Z5Int0Bttmmmmmmmmmtm
_Z10IntEntry0Bv:
	INT_INITIALIZE
	call	_Z5Int0Bttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry0Cv
	.extern	_Z5Int0Cttmmmmmmmmmtm
_Z10IntEntry0Cv:
	INT_INITIALIZE
	call	_Z5Int0Cttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry0Dv
	.extern	_Z5Int0Dttmmmmmmmmmtm
_Z10IntEntry0Dv:
	INT_INITIALIZE
	call	_Z5Int0Dttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry0Ev
	.extern	_Z5Int0Ettmmmmmmmmmtm
_Z10IntEntry0Ev:
	INT_INITIALIZE
	call	_Z5Int0Ettmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry0Fv
	.extern	_Z5Int0Fttmmmmmmmmmtm
_Z10IntEntry0Fv:
	INT_INITIALIZE
	call	_Z5Int0Fttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry10v
	.extern	_Z5Int10ttmmmmmmmmmtm
_Z10IntEntry10v:
	INT_INITIALIZE
	call	_Z5Int10ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry11v
	.extern	_Z5Int11ttmmmmmmmmmtm
_Z10IntEntry11v:
	INT_INITIALIZE
	call	_Z5Int11ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry12v
	.extern	_Z5Int12ttmmmmmmmmmtm
_Z10IntEntry12v:
	INT_INITIALIZE
	call	_Z5Int12ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry13v
	.extern	_Z5Int13ttmmmmmmmmmtm
_Z10IntEntry13v:
	INT_INITIALIZE
	call	_Z5Int13ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry14v
	.extern	_Z5Int14ttmmmmmmmmmtm
_Z10IntEntry14v:
	INT_INITIALIZE
	call	_Z5Int14ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry15v
	.extern	_Z5Int15ttmmmmmmmmmtm
_Z10IntEntry15v:
	INT_INITIALIZE
	call	_Z5Int15ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry16v
	.extern	_Z5Int16ttmmmmmmmmmtm
_Z10IntEntry16v:
	INT_INITIALIZE
	call	_Z5Int16ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry17v
	.extern	_Z5Int17ttmmmmmmmmmtm
_Z10IntEntry17v:
	INT_INITIALIZE
	call	_Z5Int17ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry18v
	.extern	_Z5Int18ttmmmmmmmmmtm
_Z10IntEntry18v:
	INT_INITIALIZE
	call	_Z5Int18ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry19v
	.extern	_Z5Int19ttmmmmmmmmmtm
_Z10IntEntry19v:
	INT_INITIALIZE
	call	_Z5Int19ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry1Av
	.extern	_Z5Int1Attmmmmmmmmmtm
_Z10IntEntry1Av:
	INT_INITIALIZE
	call	_Z5Int1Attmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry1Bv
	.extern	_Z5Int1Bttmmmmmmmmmtm
_Z10IntEntry1Bv:
	INT_INITIALIZE
	call	_Z5Int1Bttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry1Cv
	.extern	_Z5Int1Cttmmmmmmmmmtm
_Z10IntEntry1Cv:
	INT_INITIALIZE
	call	_Z5Int1Cttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry1Dv
	.extern	_Z5Int1Dttmmmmmmmmmtm
_Z10IntEntry1Dv:
	INT_INITIALIZE
	call	_Z5Int1Dttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry1Ev
	.extern	_Z5Int1Ettmmmmmmmmmtm
_Z10IntEntry1Ev:
	INT_INITIALIZE
	call	_Z5Int1Ettmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry1Fv
	.extern	_Z5Int1Fttmmmmmmmmmtm
_Z10IntEntry1Fv:
	INT_INITIALIZE
	call	_Z5Int1Fttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry20v
	.extern	_Z5Int20ttmmmmmmmmmtm
_Z10IntEntry20v:
	INT_INITIALIZE
	call	_Z5Int20ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry21v
	.extern	_Z5Int21ttmmmmmmmmmtm
_Z10IntEntry21v:
	INT_INITIALIZE
	call	_Z5Int21ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry22v
	.extern	_Z5Int22ttmmmmmmmmmtm
_Z10IntEntry22v:
	INT_INITIALIZE
	call	_Z5Int22ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry23v
	.extern	_Z5Int23ttmmmmmmmmmtm
_Z10IntEntry23v:
	INT_INITIALIZE
	call	_Z5Int23ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry24v
	.extern	_Z5Int24ttmmmmmmmmmtm
_Z10IntEntry24v:
	INT_INITIALIZE
	call	_Z5Int24ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry25v
	.extern	_Z5Int25ttmmmmmmmmmtm
_Z10IntEntry25v:
	INT_INITIALIZE
	call	_Z5Int25ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry26v
	.extern	_Z5Int26ttmmmmmmmmmtm
_Z10IntEntry26v:
	INT_INITIALIZE
	call	_Z5Int26ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry27v
	.extern	_Z5Int27ttmmmmmmmmmtm
_Z10IntEntry27v:
	INT_INITIALIZE
	call	_Z5Int27ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry28v
	.extern	_Z5Int28ttmmmmmmmmmtm
_Z10IntEntry28v:
	INT_INITIALIZE
	call	_Z5Int28ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry29v
	.extern	_Z5Int29ttmmmmmmmmmtm
_Z10IntEntry29v:
	INT_INITIALIZE
	call	_Z5Int29ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry2Av
	.extern	_Z5Int2Attmmmmmmmmmtm
_Z10IntEntry2Av:
	INT_INITIALIZE
	call	_Z5Int2Attmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry2Bv
	.extern	_Z5Int2Bttmmmmmmmmmtm
_Z10IntEntry2Bv:
	INT_INITIALIZE
	call	_Z5Int2Bttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry2Cv
	.extern	_Z5Int2Cttmmmmmmmmmtm
_Z10IntEntry2Cv:
	INT_INITIALIZE
	call	_Z5Int2Cttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry2Dv
	.extern	_Z5Int2Dttmmmmmmmmmtm
_Z10IntEntry2Dv:
	INT_INITIALIZE
	call	_Z5Int2Dttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry2Ev
	.extern	_Z5Int2Ettmmmmmmmmmtm
_Z10IntEntry2Ev:
	INT_INITIALIZE
	call	_Z5Int2Ettmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry2Fv
	.extern	_Z5Int2Fttmmmmmmmmmtm
_Z10IntEntry2Fv:
	INT_INITIALIZE
	call	_Z5Int2Fttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry30v
	.extern	_Z5Int30ttmmmmmmmmmtm
_Z10IntEntry30v:
	INT_INITIALIZE
	call	_Z5Int30ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry31v
	.extern	_Z5Int31ttmmmmmmmmmtm
_Z10IntEntry31v:
	INT_INITIALIZE
	call	_Z5Int31ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry32v
	.extern	_Z5Int32ttmmmmmmmmmtm
_Z10IntEntry32v:
	INT_INITIALIZE
	call	_Z5Int32ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry33v
	.extern	_Z5Int33ttmmmmmmmmmtm
_Z10IntEntry33v:
	INT_INITIALIZE
	call	_Z5Int33ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry34v
	.extern	_Z5Int34ttmmmmmmmmmtm
_Z10IntEntry34v:
	INT_INITIALIZE
	call	_Z5Int34ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry35v
	.extern	_Z5Int35ttmmmmmmmmmtm
_Z10IntEntry35v:
	INT_INITIALIZE
	call	_Z5Int35ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry36v
	.extern	_Z5Int36ttmmmmmmmmmtm
_Z10IntEntry36v:
	INT_INITIALIZE
	call	_Z5Int36ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry37v
	.extern	_Z5Int37ttmmmmmmmmmtm
_Z10IntEntry37v:
	INT_INITIALIZE
	call	_Z5Int37ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry38v
	.extern	_Z5Int38ttmmmmmmmmmtm
_Z10IntEntry38v:
	INT_INITIALIZE
	call	_Z5Int38ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry39v
	.extern	_Z5Int39ttmmmmmmmmmtm
_Z10IntEntry39v:
	INT_INITIALIZE
	call	_Z5Int39ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry3Av
	.extern	_Z5Int3Attmmmmmmmmmtm
_Z10IntEntry3Av:
	INT_INITIALIZE
	call	_Z5Int3Attmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry3Bv
	.extern	_Z5Int3Bttmmmmmmmmmtm
_Z10IntEntry3Bv:
	INT_INITIALIZE
	call	_Z5Int3Bttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry3Cv
	.extern	_Z5Int3Cttmmmmmmmmmtm
_Z10IntEntry3Cv:
	INT_INITIALIZE
	call	_Z5Int3Cttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry3Dv
	.extern	_Z5Int3Dttmmmmmmmmmtm
_Z10IntEntry3Dv:
	INT_INITIALIZE
	call	_Z5Int3Dttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry3Ev
	.extern	_Z5Int3Ettmmmmmmmmmtm
_Z10IntEntry3Ev:
	INT_INITIALIZE
	call	_Z5Int3Ettmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry3Fv
	.extern	_Z5Int3Fttmmmmmmmmmtm
_Z10IntEntry3Fv:
	INT_INITIALIZE
	call	_Z5Int3Fttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry40v
	.extern	_Z5Int40ttmmmmmmmmmtm
_Z10IntEntry40v:
	INT_INITIALIZE
	call	_Z5Int40ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry41v
	.extern	_Z5Int41ttmmmmmmmmmtm
_Z10IntEntry41v:
	INT_INITIALIZE
	call	_Z5Int41ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry42v
	.extern	_Z5Int42ttmmmmmmmmmtm
_Z10IntEntry42v:
	INT_INITIALIZE
	call	_Z5Int42ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry43v
	.extern	_Z5Int43ttmmmmmmmmmtm
_Z10IntEntry43v:
	INT_INITIALIZE
	call	_Z5Int43ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry44v
	.extern	_Z5Int44ttmmmmmmmmmtm
_Z10IntEntry44v:
	INT_INITIALIZE
	call	_Z5Int44ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry45v
	.extern	_Z5Int45ttmmmmmmmmmtm
_Z10IntEntry45v:
	INT_INITIALIZE
	call	_Z5Int45ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry46v
	.extern	_Z5Int46ttmmmmmmmmmtm
_Z10IntEntry46v:
	INT_INITIALIZE
	call	_Z5Int46ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry47v
	.extern	_Z5Int47ttmmmmmmmmmtm
_Z10IntEntry47v:
	INT_INITIALIZE
	call	_Z5Int47ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry48v
	.extern	_Z5Int48ttmmmmmmmmmtm
_Z10IntEntry48v:
	INT_INITIALIZE
	call	_Z5Int48ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry49v
	.extern	_Z5Int49ttmmmmmmmmmtm
_Z10IntEntry49v:
	INT_INITIALIZE
	call	_Z5Int49ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry4Av
	.extern	_Z5Int4Attmmmmmmmmmtm
_Z10IntEntry4Av:
	INT_INITIALIZE
	call	_Z5Int4Attmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry4Bv
	.extern	_Z5Int4Bttmmmmmmmmmtm
_Z10IntEntry4Bv:
	INT_INITIALIZE
	call	_Z5Int4Bttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry4Cv
	.extern	_Z5Int4Cttmmmmmmmmmtm
_Z10IntEntry4Cv:
	INT_INITIALIZE
	call	_Z5Int4Cttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry4Dv
	.extern	_Z5Int4Dttmmmmmmmmmtm
_Z10IntEntry4Dv:
	INT_INITIALIZE
	call	_Z5Int4Dttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry4Ev
	.extern	_Z5Int4Ettmmmmmmmmmtm
_Z10IntEntry4Ev:
	INT_INITIALIZE
	call	_Z5Int4Ettmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry4Fv
	.extern	_Z5Int4Fttmmmmmmmmmtm
_Z10IntEntry4Fv:
	INT_INITIALIZE
	call	_Z5Int4Fttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry50v
	.extern	_Z5Int50ttmmmmmmmmmtm
_Z10IntEntry50v:
	INT_INITIALIZE
	call	_Z5Int50ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry51v
	.extern	_Z5Int51ttmmmmmmmmmtm
_Z10IntEntry51v:
	INT_INITIALIZE
	call	_Z5Int51ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry52v
	.extern	_Z5Int52ttmmmmmmmmmtm
_Z10IntEntry52v:
	INT_INITIALIZE
	call	_Z5Int52ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry53v
	.extern	_Z5Int53ttmmmmmmmmmtm
_Z10IntEntry53v:
	INT_INITIALIZE
	call	_Z5Int53ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry54v
	.extern	_Z5Int54ttmmmmmmmmmtm
_Z10IntEntry54v:
	INT_INITIALIZE
	call	_Z5Int54ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry55v
	.extern	_Z5Int55ttmmmmmmmmmtm
_Z10IntEntry55v:
	INT_INITIALIZE
	call	_Z5Int55ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry56v
	.extern	_Z5Int56ttmmmmmmmmmtm
_Z10IntEntry56v:
	INT_INITIALIZE
	call	_Z5Int56ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry57v
	.extern	_Z5Int57ttmmmmmmmmmtm
_Z10IntEntry57v:
	INT_INITIALIZE
	call	_Z5Int57ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry58v
	.extern	_Z5Int58ttmmmmmmmmmtm
_Z10IntEntry58v:
	INT_INITIALIZE
	call	_Z5Int58ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry59v
	.extern	_Z5Int59ttmmmmmmmmmtm
_Z10IntEntry59v:
	INT_INITIALIZE
	call	_Z5Int59ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry5Av
	.extern	_Z5Int5Attmmmmmmmmmtm
_Z10IntEntry5Av:
	INT_INITIALIZE
	call	_Z5Int5Attmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry5Bv
	.extern	_Z5Int5Bttmmmmmmmmmtm
_Z10IntEntry5Bv:
	INT_INITIALIZE
	call	_Z5Int5Bttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry5Cv
	.extern	_Z5Int5Cttmmmmmmmmmtm
_Z10IntEntry5Cv:
	INT_INITIALIZE
	call	_Z5Int5Cttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry5Dv
	.extern	_Z5Int5Dttmmmmmmmmmtm
_Z10IntEntry5Dv:
	INT_INITIALIZE
	call	_Z5Int5Dttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry5Ev
	.extern	_Z5Int5Ettmmmmmmmmmtm
_Z10IntEntry5Ev:
	INT_INITIALIZE
	call	_Z5Int5Ettmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry5Fv
	.extern	_Z5Int5Fttmmmmmmmmmtm
_Z10IntEntry5Fv:
	INT_INITIALIZE
	call	_Z5Int5Fttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry60v
	.extern	_Z5Int60ttmmmmmmmmmtm
_Z10IntEntry60v:
	INT_INITIALIZE
	call	_Z5Int60ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry61v
	.extern	_Z5Int61ttmmmmmmmmmtm
_Z10IntEntry61v:
	INT_INITIALIZE
	call	_Z5Int61ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry62v
	.extern	_Z5Int62ttmmmmmmmmmtm
_Z10IntEntry62v:
	INT_INITIALIZE
	call	_Z5Int62ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry63v
	.extern	_Z5Int63ttmmmmmmmmmtm
_Z10IntEntry63v:
	INT_INITIALIZE
	call	_Z5Int63ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry64v
	.extern	_Z5Int64ttmmmmmmmmmtm
_Z10IntEntry64v:
	INT_INITIALIZE
	call	_Z5Int64ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry65v
	.extern	_Z5Int65ttmmmmmmmmmtm
_Z10IntEntry65v:
	INT_INITIALIZE
	call	_Z5Int65ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry66v
	.extern	_Z5Int66ttmmmmmmmmmtm
_Z10IntEntry66v:
	INT_INITIALIZE
	call	_Z5Int66ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry67v
	.extern	_Z5Int67ttmmmmmmmmmtm
_Z10IntEntry67v:
	INT_INITIALIZE
	call	_Z5Int67ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry68v
	.extern	_Z5Int68ttmmmmmmmmmtm
_Z10IntEntry68v:
	INT_INITIALIZE
	call	_Z5Int68ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry69v
	.extern	_Z5Int69ttmmmmmmmmmtm
_Z10IntEntry69v:
	INT_INITIALIZE
	call	_Z5Int69ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry6Av
	.extern	_Z5Int6Attmmmmmmmmmtm
_Z10IntEntry6Av:
	INT_INITIALIZE
	call	_Z5Int6Attmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry6Bv
	.extern	_Z5Int6Bttmmmmmmmmmtm
_Z10IntEntry6Bv:
	INT_INITIALIZE
	call	_Z5Int6Bttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry6Cv
	.extern	_Z5Int6Cttmmmmmmmmmtm
_Z10IntEntry6Cv:
	INT_INITIALIZE
	call	_Z5Int6Cttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry6Dv
	.extern	_Z5Int6Dttmmmmmmmmmtm
_Z10IntEntry6Dv:
	INT_INITIALIZE
	call	_Z5Int6Dttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry6Ev
	.extern	_Z5Int6Ettmmmmmmmmmtm
_Z10IntEntry6Ev:
	INT_INITIALIZE
	call	_Z5Int6Ettmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry6Fv
	.extern	_Z5Int6Fttmmmmmmmmmtm
_Z10IntEntry6Fv:
	INT_INITIALIZE
	call	_Z5Int6Fttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry70v
	.extern	_Z5Int70ttmmmmmmmmmtm
_Z10IntEntry70v:
	INT_INITIALIZE
	call	_Z5Int70ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry71v
	.extern	_Z5Int71ttmmmmmmmmmtm
_Z10IntEntry71v:
	INT_INITIALIZE
	call	_Z5Int71ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry72v
	.extern	_Z5Int72ttmmmmmmmmmtm
_Z10IntEntry72v:
	INT_INITIALIZE
	call	_Z5Int72ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry73v
	.extern	_Z5Int73ttmmmmmmmmmtm
_Z10IntEntry73v:
	INT_INITIALIZE
	call	_Z5Int73ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry74v
	.extern	_Z5Int74ttmmmmmmmmmtm
_Z10IntEntry74v:
	INT_INITIALIZE
	call	_Z5Int74ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry75v
	.extern	_Z5Int75ttmmmmmmmmmtm
_Z10IntEntry75v:
	INT_INITIALIZE
	call	_Z5Int75ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry76v
	.extern	_Z5Int76ttmmmmmmmmmtm
_Z10IntEntry76v:
	INT_INITIALIZE
	call	_Z5Int76ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry77v
	.extern	_Z5Int77ttmmmmmmmmmtm
_Z10IntEntry77v:
	INT_INITIALIZE
	call	_Z5Int77ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry78v
	.extern	_Z5Int78ttmmmmmmmmmtm
_Z10IntEntry78v:
	INT_INITIALIZE
	call	_Z5Int78ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry79v
	.extern	_Z5Int79ttmmmmmmmmmtm
_Z10IntEntry79v:
	INT_INITIALIZE
	call	_Z5Int79ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry7Av
	.extern	_Z5Int7Attmmmmmmmmmtm
_Z10IntEntry7Av:
	INT_INITIALIZE
	call	_Z5Int7Attmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry7Bv
	.extern	_Z5Int7Bttmmmmmmmmmtm
_Z10IntEntry7Bv:
	INT_INITIALIZE
	call	_Z5Int7Bttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry7Cv
	.extern	_Z5Int7Cttmmmmmmmmmtm
_Z10IntEntry7Cv:
	INT_INITIALIZE
	call	_Z5Int7Cttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry7Dv
	.extern	_Z5Int7Dttmmmmmmmmmtm
_Z10IntEntry7Dv:
	INT_INITIALIZE
	call	_Z5Int7Dttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry7Ev
	.extern	_Z5Int7Ettmmmmmmmmmtm
_Z10IntEntry7Ev:
	INT_INITIALIZE
	call	_Z5Int7Ettmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry7Fv
	.extern	_Z5Int7Fttmmmmmmmmmtm
_Z10IntEntry7Fv:
	INT_INITIALIZE
	call	_Z5Int7Fttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry80v
	.extern	_Z5Int80ttmmmmmmmmmtm
_Z10IntEntry80v:
	INT_INITIALIZE
	call	_Z5Int80ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry81v
	.extern	_Z5Int81ttmmmmmmmmmtm
_Z10IntEntry81v:
	INT_INITIALIZE
	call	_Z5Int81ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry82v
	.extern	_Z5Int82ttmmmmmmmmmtm
_Z10IntEntry82v:
	INT_INITIALIZE
	call	_Z5Int82ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry83v
	.extern	_Z5Int83ttmmmmmmmmmtm
_Z10IntEntry83v:
	INT_INITIALIZE
	call	_Z5Int83ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry84v
	.extern	_Z5Int84ttmmmmmmmmmtm
_Z10IntEntry84v:
	INT_INITIALIZE
	call	_Z5Int84ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry85v
	.extern	_Z5Int85ttmmmmmmmmmtm
_Z10IntEntry85v:
	INT_INITIALIZE
	call	_Z5Int85ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry86v
	.extern	_Z5Int86ttmmmmmmmmmtm
_Z10IntEntry86v:
	INT_INITIALIZE
	call	_Z5Int86ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry87v
	.extern	_Z5Int87ttmmmmmmmmmtm
_Z10IntEntry87v:
	INT_INITIALIZE
	call	_Z5Int87ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry88v
	.extern	_Z5Int88ttmmmmmmmmmtm
_Z10IntEntry88v:
	INT_INITIALIZE
	call	_Z5Int88ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry89v
	.extern	_Z5Int89ttmmmmmmmmmtm
_Z10IntEntry89v:
	INT_INITIALIZE
	call	_Z5Int89ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry8Av
	.extern	_Z5Int8Attmmmmmmmmmtm
_Z10IntEntry8Av:
	INT_INITIALIZE
	call	_Z5Int8Attmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry8Bv
	.extern	_Z5Int8Bttmmmmmmmmmtm
_Z10IntEntry8Bv:
	INT_INITIALIZE
	call	_Z5Int8Bttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry8Cv
	.extern	_Z5Int8Cttmmmmmmmmmtm
_Z10IntEntry8Cv:
	INT_INITIALIZE
	call	_Z5Int8Cttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry8Dv
	.extern	_Z5Int8Dttmmmmmmmmmtm
_Z10IntEntry8Dv:
	INT_INITIALIZE
	call	_Z5Int8Dttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry8Ev
	.extern	_Z5Int8Ettmmmmmmmmmtm
_Z10IntEntry8Ev:
	INT_INITIALIZE
	call	_Z5Int8Ettmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry8Fv
	.extern	_Z5Int8Fttmmmmmmmmmtm
_Z10IntEntry8Fv:
	INT_INITIALIZE
	call	_Z5Int8Fttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry90v
	.extern	_Z5Int90ttmmmmmmmmmtm
_Z10IntEntry90v:
	INT_INITIALIZE
	call	_Z5Int90ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry91v
	.extern	_Z5Int91ttmmmmmmmmmtm
_Z10IntEntry91v:
	INT_INITIALIZE
	call	_Z5Int91ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry92v
	.extern	_Z5Int92ttmmmmmmmmmtm
_Z10IntEntry92v:
	INT_INITIALIZE
	call	_Z5Int92ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry93v
	.extern	_Z5Int93ttmmmmmmmmmtm
_Z10IntEntry93v:
	INT_INITIALIZE
	call	_Z5Int93ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry94v
	.extern	_Z5Int94ttmmmmmmmmmtm
_Z10IntEntry94v:
	INT_INITIALIZE
	call	_Z5Int94ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry95v
	.extern	_Z5Int95ttmmmmmmmmmtm
_Z10IntEntry95v:
	INT_INITIALIZE
	call	_Z5Int95ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry96v
	.extern	_Z5Int96ttmmmmmmmmmtm
_Z10IntEntry96v:
	INT_INITIALIZE
	call	_Z5Int96ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry97v
	.extern	_Z5Int97ttmmmmmmmmmtm
_Z10IntEntry97v:
	INT_INITIALIZE
	call	_Z5Int97ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry98v
	.extern	_Z5Int98ttmmmmmmmmmtm
_Z10IntEntry98v:
	INT_INITIALIZE
	call	_Z5Int98ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry99v
	.extern	_Z5Int99ttmmmmmmmmmtm
_Z10IntEntry99v:
	INT_INITIALIZE
	call	_Z5Int99ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry9Av
	.extern	_Z5Int9Attmmmmmmmmmtm
_Z10IntEntry9Av:
	INT_INITIALIZE
	call	_Z5Int9Attmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry9Bv
	.extern	_Z5Int9Bttmmmmmmmmmtm
_Z10IntEntry9Bv:
	INT_INITIALIZE
	call	_Z5Int9Bttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry9Cv
	.extern	_Z5Int9Cttmmmmmmmmmtm
_Z10IntEntry9Cv:
	INT_INITIALIZE
	call	_Z5Int9Cttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry9Dv
	.extern	_Z5Int9Dttmmmmmmmmmtm
_Z10IntEntry9Dv:
	INT_INITIALIZE
	call	_Z5Int9Dttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry9Ev
	.extern	_Z5Int9Ettmmmmmmmmmtm
_Z10IntEntry9Ev:
	INT_INITIALIZE
	call	_Z5Int9Ettmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntry9Fv
	.extern	_Z5Int9Fttmmmmmmmmmtm
_Z10IntEntry9Fv:
	INT_INITIALIZE
	call	_Z5Int9Fttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryA0v
	.extern	_Z5IntA0ttmmmmmmmmmtm
_Z10IntEntryA0v:
	INT_INITIALIZE
	call	_Z5IntA0ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryA1v
	.extern	_Z5IntA1ttmmmmmmmmmtm
_Z10IntEntryA1v:
	INT_INITIALIZE
	call	_Z5IntA1ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryA2v
	.extern	_Z5IntA2ttmmmmmmmmmtm
_Z10IntEntryA2v:
	INT_INITIALIZE
	call	_Z5IntA2ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryA3v
	.extern	_Z5IntA3ttmmmmmmmmmtm
_Z10IntEntryA3v:
	INT_INITIALIZE
	call	_Z5IntA3ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryA4v
	.extern	_Z5IntA4ttmmmmmmmmmtm
_Z10IntEntryA4v:
	INT_INITIALIZE
	call	_Z5IntA4ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryA5v
	.extern	_Z5IntA5ttmmmmmmmmmtm
_Z10IntEntryA5v:
	INT_INITIALIZE
	call	_Z5IntA5ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryA6v
	.extern	_Z5IntA6ttmmmmmmmmmtm
_Z10IntEntryA6v:
	INT_INITIALIZE
	call	_Z5IntA6ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryA7v
	.extern	_Z5IntA7ttmmmmmmmmmtm
_Z10IntEntryA7v:
	INT_INITIALIZE
	call	_Z5IntA7ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryA8v
	.extern	_Z5IntA8ttmmmmmmmmmtm
_Z10IntEntryA8v:
	INT_INITIALIZE
	call	_Z5IntA8ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryA9v
	.extern	_Z5IntA9ttmmmmmmmmmtm
_Z10IntEntryA9v:
	INT_INITIALIZE
	call	_Z5IntA9ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryAAv
	.extern	_Z5IntAAttmmmmmmmmmtm
_Z10IntEntryAAv:
	INT_INITIALIZE
	call	_Z5IntAAttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryABv
	.extern	_Z5IntABttmmmmmmmmmtm
_Z10IntEntryABv:
	INT_INITIALIZE
	call	_Z5IntABttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryACv
	.extern	_Z5IntACttmmmmmmmmmtm
_Z10IntEntryACv:
	INT_INITIALIZE
	call	_Z5IntACttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryADv
	.extern	_Z5IntADttmmmmmmmmmtm
_Z10IntEntryADv:
	INT_INITIALIZE
	call	_Z5IntADttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryAEv
	.extern	_Z5IntAEttmmmmmmmmmtm
_Z10IntEntryAEv:
	INT_INITIALIZE
	call	_Z5IntAEttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryAFv
	.extern	_Z5IntAFttmmmmmmmmmtm
_Z10IntEntryAFv:
	INT_INITIALIZE
	call	_Z5IntAFttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryB0v
	.extern	_Z5IntB0ttmmmmmmmmmtm
_Z10IntEntryB0v:
	INT_INITIALIZE
	call	_Z5IntB0ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryB1v
	.extern	_Z5IntB1ttmmmmmmmmmtm
_Z10IntEntryB1v:
	INT_INITIALIZE
	call	_Z5IntB1ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryB2v
	.extern	_Z5IntB2ttmmmmmmmmmtm
_Z10IntEntryB2v:
	INT_INITIALIZE
	call	_Z5IntB2ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryB3v
	.extern	_Z5IntB3ttmmmmmmmmmtm
_Z10IntEntryB3v:
	INT_INITIALIZE
	call	_Z5IntB3ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryB4v
	.extern	_Z5IntB4ttmmmmmmmmmtm
_Z10IntEntryB4v:
	INT_INITIALIZE
	call	_Z5IntB4ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryB5v
	.extern	_Z5IntB5ttmmmmmmmmmtm
_Z10IntEntryB5v:
	INT_INITIALIZE
	call	_Z5IntB5ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryB6v
	.extern	_Z5IntB6ttmmmmmmmmmtm
_Z10IntEntryB6v:
	INT_INITIALIZE
	call	_Z5IntB6ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryB7v
	.extern	_Z5IntB7ttmmmmmmmmmtm
_Z10IntEntryB7v:
	INT_INITIALIZE
	call	_Z5IntB7ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryB8v
	.extern	_Z5IntB8ttmmmmmmmmmtm
_Z10IntEntryB8v:
	INT_INITIALIZE
	call	_Z5IntB8ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryB9v
	.extern	_Z5IntB9ttmmmmmmmmmtm
_Z10IntEntryB9v:
	INT_INITIALIZE
	call	_Z5IntB9ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryBAv
	.extern	_Z5IntBAttmmmmmmmmmtm
_Z10IntEntryBAv:
	INT_INITIALIZE
	call	_Z5IntBAttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryBBv
	.extern	_Z5IntBBttmmmmmmmmmtm
_Z10IntEntryBBv:
	INT_INITIALIZE
	call	_Z5IntBBttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryBCv
	.extern	_Z5IntBCttmmmmmmmmmtm
_Z10IntEntryBCv:
	INT_INITIALIZE
	call	_Z5IntBCttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryBDv
	.extern	_Z5IntBDttmmmmmmmmmtm
_Z10IntEntryBDv:
	INT_INITIALIZE
	call	_Z5IntBDttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryBEv
	.extern	_Z5IntBEttmmmmmmmmmtm
_Z10IntEntryBEv:
	INT_INITIALIZE
	call	_Z5IntBEttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryBFv
	.extern	_Z5IntBFttmmmmmmmmmtm
_Z10IntEntryBFv:
	INT_INITIALIZE
	call	_Z5IntBFttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryC0v
	.extern	_Z5IntC0ttmmmmmmmmmtm
_Z10IntEntryC0v:
	INT_INITIALIZE
	call	_Z5IntC0ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryC1v
	.extern	_Z5IntC1ttmmmmmmmmmtm
_Z10IntEntryC1v:
	INT_INITIALIZE
	call	_Z5IntC1ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryC2v
	.extern	_Z5IntC2ttmmmmmmmmmtm
_Z10IntEntryC2v:
	INT_INITIALIZE
	call	_Z5IntC2ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryC3v
	.extern	_Z5IntC3ttmmmmmmmmmtm
_Z10IntEntryC3v:
	INT_INITIALIZE
	call	_Z5IntC3ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryC4v
	.extern	_Z5IntC4ttmmmmmmmmmtm
_Z10IntEntryC4v:
	INT_INITIALIZE
	call	_Z5IntC4ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryC5v
	.extern	_Z5IntC5ttmmmmmmmmmtm
_Z10IntEntryC5v:
	INT_INITIALIZE
	call	_Z5IntC5ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryC6v
	.extern	_Z5IntC6ttmmmmmmmmmtm
_Z10IntEntryC6v:
	INT_INITIALIZE
	call	_Z5IntC6ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryC7v
	.extern	_Z5IntC7ttmmmmmmmmmtm
_Z10IntEntryC7v:
	INT_INITIALIZE
	call	_Z5IntC7ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryC8v
	.extern	_Z5IntC8ttmmmmmmmmmtm
_Z10IntEntryC8v:
	INT_INITIALIZE
	call	_Z5IntC8ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryC9v
	.extern	_Z5IntC9ttmmmmmmmmmtm
_Z10IntEntryC9v:
	INT_INITIALIZE
	call	_Z5IntC9ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryCAv
	.extern	_Z5IntCAttmmmmmmmmmtm
_Z10IntEntryCAv:
	INT_INITIALIZE
	call	_Z5IntCAttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryCBv
	.extern	_Z5IntCBttmmmmmmmmmtm
_Z10IntEntryCBv:
	INT_INITIALIZE
	call	_Z5IntCBttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryCCv
	.extern	_Z5IntCCttmmmmmmmmmtm
_Z10IntEntryCCv:
	INT_INITIALIZE
	call	_Z5IntCCttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryCDv
	.extern	_Z5IntCDttmmmmmmmmmtm
_Z10IntEntryCDv:
	INT_INITIALIZE
	call	_Z5IntCDttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryCEv
	.extern	_Z5IntCEttmmmmmmmmmtm
_Z10IntEntryCEv:
	INT_INITIALIZE
	call	_Z5IntCEttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryCFv
	.extern	_Z5IntCFttmmmmmmmmmtm
_Z10IntEntryCFv:
	INT_INITIALIZE
	call	_Z5IntCFttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryD0v
	.extern	_Z5IntD0ttmmmmmmmmmtm
_Z10IntEntryD0v:
	INT_INITIALIZE
	call	_Z5IntD0ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryD1v
	.extern	_Z5IntD1ttmmmmmmmmmtm
_Z10IntEntryD1v:
	INT_INITIALIZE
	call	_Z5IntD1ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryD2v
	.extern	_Z5IntD2ttmmmmmmmmmtm
_Z10IntEntryD2v:
	INT_INITIALIZE
	call	_Z5IntD2ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryD3v
	.extern	_Z5IntD3ttmmmmmmmmmtm
_Z10IntEntryD3v:
	INT_INITIALIZE
	call	_Z5IntD3ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryD4v
	.extern	_Z5IntD4ttmmmmmmmmmtm
_Z10IntEntryD4v:
	INT_INITIALIZE
	call	_Z5IntD4ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryD5v
	.extern	_Z5IntD5ttmmmmmmmmmtm
_Z10IntEntryD5v:
	INT_INITIALIZE
	call	_Z5IntD5ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryD6v
	.extern	_Z5IntD6ttmmmmmmmmmtm
_Z10IntEntryD6v:
	INT_INITIALIZE
	call	_Z5IntD6ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryD7v
	.extern	_Z5IntD7ttmmmmmmmmmtm
_Z10IntEntryD7v:
	INT_INITIALIZE
	call	_Z5IntD7ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryD8v
	.extern	_Z5IntD8ttmmmmmmmmmtm
_Z10IntEntryD8v:
	INT_INITIALIZE
	call	_Z5IntD8ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryD9v
	.extern	_Z5IntD9ttmmmmmmmmmtm
_Z10IntEntryD9v:
	INT_INITIALIZE
	call	_Z5IntD9ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryDAv
	.extern	_Z5IntDAttmmmmmmmmmtm
_Z10IntEntryDAv:
	INT_INITIALIZE
	call	_Z5IntDAttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryDBv
	.extern	_Z5IntDBttmmmmmmmmmtm
_Z10IntEntryDBv:
	INT_INITIALIZE
	call	_Z5IntDBttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryDCv
	.extern	_Z5IntDCttmmmmmmmmmtm
_Z10IntEntryDCv:
	INT_INITIALIZE
	call	_Z5IntDCttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryDDv
	.extern	_Z5IntDDttmmmmmmmmmtm
_Z10IntEntryDDv:
	INT_INITIALIZE
	call	_Z5IntDDttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryDEv
	.extern	_Z5IntDEttmmmmmmmmmtm
_Z10IntEntryDEv:
	INT_INITIALIZE
	call	_Z5IntDEttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryDFv
	.extern	_Z5IntDFttmmmmmmmmmtm
_Z10IntEntryDFv:
	INT_INITIALIZE
	call	_Z5IntDFttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryE0v
	.extern	_Z5IntE0ttmmmmmmmmmtm
_Z10IntEntryE0v:
	INT_INITIALIZE
	call	_Z5IntE0ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryE1v
	.extern	_Z5IntE1ttmmmmmmmmmtm
_Z10IntEntryE1v:
	INT_INITIALIZE
	call	_Z5IntE1ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryE2v
	.extern	_Z5IntE2ttmmmmmmmmmtm
_Z10IntEntryE2v:
	INT_INITIALIZE
	call	_Z5IntE2ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryE3v
	.extern	_Z5IntE3ttmmmmmmmmmtm
_Z10IntEntryE3v:
	INT_INITIALIZE
	call	_Z5IntE3ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryE4v
	.extern	_Z5IntE4ttmmmmmmmmmtm
_Z10IntEntryE4v:
	INT_INITIALIZE
	call	_Z5IntE4ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryE5v
	.extern	_Z5IntE5ttmmmmmmmmmtm
_Z10IntEntryE5v:
	INT_INITIALIZE
	call	_Z5IntE5ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryE6v
	.extern	_Z5IntE6ttmmmmmmmmmtm
_Z10IntEntryE6v:
	INT_INITIALIZE
	call	_Z5IntE6ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryE7v
	.extern	_Z5IntE7ttmmmmmmmmmtm
_Z10IntEntryE7v:
	INT_INITIALIZE
	call	_Z5IntE7ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryE8v
	.extern	_Z5IntE8ttmmmmmmmmmtm
_Z10IntEntryE8v:
	INT_INITIALIZE
	call	_Z5IntE8ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryE9v
	.extern	_Z5IntE9ttmmmmmmmmmtm
_Z10IntEntryE9v:
	INT_INITIALIZE
	call	_Z5IntE9ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryEAv
	.extern	_Z5IntEAttmmmmmmmmmtm
_Z10IntEntryEAv:
	INT_INITIALIZE
	call	_Z5IntEAttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryEBv
	.extern	_Z5IntEBttmmmmmmmmmtm
_Z10IntEntryEBv:
	INT_INITIALIZE
	call	_Z5IntEBttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryECv
	.extern	_Z5IntECttmmmmmmmmmtm
_Z10IntEntryECv:
	INT_INITIALIZE
	call	_Z5IntECttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryEDv
	.extern	_Z5IntEDttmmmmmmmmmtm
_Z10IntEntryEDv:
	INT_INITIALIZE
	call	_Z5IntEDttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryEEv
	.extern	_Z5IntEEttmmmmmmmmmtm
_Z10IntEntryEEv:
	INT_INITIALIZE
	call	_Z5IntEEttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryEFv
	.extern	_Z5IntEFttmmmmmmmmmtm
_Z10IntEntryEFv:
	INT_INITIALIZE
	call	_Z5IntEFttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryF0v
	.extern	_Z5IntF0ttmmmmmmmmmtm
_Z10IntEntryF0v:
	INT_INITIALIZE
	call	_Z5IntF0ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryF1v
	.extern	_Z5IntF1ttmmmmmmmmmtm
_Z10IntEntryF1v:
	INT_INITIALIZE
	call	_Z5IntF1ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryF2v
	.extern	_Z5IntF2ttmmmmmmmmmtm
_Z10IntEntryF2v:
	INT_INITIALIZE
	call	_Z5IntF2ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryF3v
	.extern	_Z5IntF3ttmmmmmmmmmtm
_Z10IntEntryF3v:
	INT_INITIALIZE
	call	_Z5IntF3ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryF4v
	.extern	_Z5IntF4ttmmmmmmmmmtm
_Z10IntEntryF4v:
	INT_INITIALIZE
	call	_Z5IntF4ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryF5v
	.extern	_Z5IntF5ttmmmmmmmmmtm
_Z10IntEntryF5v:
	INT_INITIALIZE
	call	_Z5IntF5ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryF6v
	.extern	_Z5IntF6ttmmmmmmmmmtm
_Z10IntEntryF6v:
	INT_INITIALIZE
	call	_Z5IntF6ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryF7v
	.extern	_Z5IntF7ttmmmmmmmmmtm
_Z10IntEntryF7v:
	INT_INITIALIZE
	call	_Z5IntF7ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryF8v
	.extern	_Z5IntF8ttmmmmmmmmmtm
_Z10IntEntryF8v:
	INT_INITIALIZE
	call	_Z5IntF8ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryF9v
	.extern	_Z5IntF9ttmmmmmmmmmtm
_Z10IntEntryF9v:
	INT_INITIALIZE
	call	_Z5IntF9ttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryFAv
	.extern	_Z5IntFAttmmmmmmmmmtm
_Z10IntEntryFAv:
	INT_INITIALIZE
	call	_Z5IntFAttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryFBv
	.extern	_Z5IntFBttmmmmmmmmmtm
_Z10IntEntryFBv:
	INT_INITIALIZE
	call	_Z5IntFBttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryFCv
	.extern	_Z5IntFCttmmmmmmmmmtm
_Z10IntEntryFCv:
	INT_INITIALIZE
	call	_Z5IntFCttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryFDv
	.extern	_Z5IntFDttmmmmmmmmmtm
_Z10IntEntryFDv:
	INT_INITIALIZE
	call	_Z5IntFDttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryFEv
	.extern	_Z5IntFEttmmmmmmmmmtm
_Z10IntEntryFEv:
	INT_INITIALIZE
	call	_Z5IntFEttmmmmmmmmmtm
	INT_FINALIZE

	.global	_Z10IntEntryFFv
	.extern	_Z5IntFFttmmmmmmmmmtm
_Z10IntEntryFFv:
	INT_INITIALIZE
	call	_Z5IntFFttmmmmmmmmmtm
	INT_FINALIZE



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
