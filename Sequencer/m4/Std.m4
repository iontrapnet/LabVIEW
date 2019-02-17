divert(-1)

define(`argn', `ifelse(`$1', 1, ``$2'', `argn(decr(`$1'), shift(shift($@)))')')
define(`arg1', `$1')
define(`Pair', `ifelse(`$1',`()',`',`arg1$1 arg1$2
Pair((shift$1),(shift$2))')')
defn(`Pair')
define(`Chapter', `define($1, `#$1
Pair'((`$'*),(shift($*))))dnl')

divert(0)dnl