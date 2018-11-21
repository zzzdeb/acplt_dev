let SessionLoad = 1
if &cp | set nocp | endif
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/hiwi/ACPLT-DevKit-linux64/acplt/dev/sync/source
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +51 ~/hiwi/ACPLT-DevKit-linux64/acplt/dev/sync/source/sscBuffer.c
badd +9 ~/.vim/UltiSnips/c.snippets
badd +50 ~/hiwi/ACPLT-DevKit-linux64/acplt/dev/sync/source/dsync.c
badd +208 ~/.vim/vimrc
argglobal
silent! argdel *
edit ~/hiwi/ACPLT-DevKit-linux64/acplt/dev/sync/source/dsync.c
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 52 - ((14 * winheight(0) + 22) / 45)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
52
normal! 0
lcd ~/hiwi/ACPLT-DevKit-linux64/acplt/dev/sync/source
tabnext 1
if exists('s:wipebuf')
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=AIc
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
