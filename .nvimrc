set tabstop=2
set softtabstop=0
set shiftwidth=2
set noexpandtab

set foldmethod=indent
set foldlevelstart=99
                    
set colorcolumn=110
highlight ColorColumn ctermbg=darkgray

augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END

set noautochdir 

let &path.="*/include, */source, CTree/include, CTree/source"

"autocmd VimEnter * NERDTree

"let g:ycm_global_ycm_extra_conf = ".ycm_extra_conf.py"
let g:ycm_confirm_extra_conf = 0
" let g:ycm_autoclose_preview_window_after_insertion = 1
let g:ycm_auto_trigger = 1

let 

let g:UltiSnipsSnippetDirectories = ['/home/zzz/hiwi/ACPLT-DevKit-linux64/acplt/dev/UltiSnips', '/home/zzz/.vim/bundle/vim-snippets/UltiSnips']

set wildignore+=*.o,*.a,*.so,*.un~,*/source/sourcetemplates,.*/build,draft,*/CTree1
let g:CommandTWildIgnore = &wildignore . ", */CTree1, */build/*"

" let g:CCSpellCheckEnable = 1
" set makeprg 
" augroup END

" :nnoremap <f2> :cd ~/hiwi/ACPLT-DevKit-linux64/acplt/dev <bar> mksession!<CR>
" :inoremap ' "
" :inoremap " '

"if exists("did_load_filetypes")
"  finish
" endif
" augroup filetypedetect
"   au! BufNewFile,BufRead *.c setf c

autocmd FileType c.doxygen :nnoremap <buffer> <C-S> :ClangFormat <CR>:<C-u>Update<CR>
autocmd FileType c.doxygen :inoremap <buffer> <C-S> <ESC>:ClangFormat <CR>:Update<CR>
nnoremap <C-S> :<C-u>Update<CR>
inoremap <C-S> <ESC>:Update<CR>


set makeprg=make\ -C\ %:h/../build/linux\ -j\ debug


nmap <c-b> :make<cr> 
au QuickFixCmdPost make exec 'cw' 

""nnoremap <Leader>b :!make -C %:h/../build/linux -j debug<cr>

" function! g:UltiSnips_Complete()
"     call UltiSnips#ExpandSnippet()
"     if g:ulti_expand_res == 0
"         if pumvisible()
"             return "\<C-n>"
"         else
"             call UltiSnips#JumpForwards()
"             if g:ulti_jump_forwards_res == 0
"                return "\<TAB>"
"             endif
"         endif
"     endif
"     return ""
" endfunction

" au BufEnter * exec "inoremap <silent> " . g:UltiSnipsExpandTrigger . " <C-R>=g:UltiSnips_Complete()<cr>"
let g:UltiSnipsListSnippets="<c-e>"
" this mapping Enter key to <C-y> to chose the current highlight item 
" and close the selection list, same as other IDEs.
" CONFLICT with some plugins like tpope/Endwise
inoremap <expr> <CR> pumvisible() ? "\<C-y>" : "\<C-g>u\<CR>"

augroup ovm 
    au BufWrite *.ovm :sil !rm %:h:h/source/sourcetemplates/*
    au BufWritePost *.ovm !acplt_builder -l %:h:h:t

    " au BufWritePost *.ovm :ped term://acplt_builder -l %:h:h:t 2>&1 \| grep -i Error
augroup END

nmap <localleader>s :%s//

nmap <localleader>ev :vsplit .nvimrc<cr>
nmap <localleader>eom :execute ":vsplit ".expand("%:h:h")."/model/".expand("%:h:h").".ovm \| let @/='CLASS\[^S\]*".expand('%:t:r')."\[^S\]*:'"<cr>nz<cr>
let @/='CLASS\[^S\]*\[^S\]*\:'"<cr>

nmap <localleader>eof :execute ":vsplit ".expand("%:h:h")."/model/".expand("%:h:h").".ovf"<cr>
nmap <localleader>eot :execute ":vsplit ".expand("%:h:h")."/model/".expand("%:h:h").".ovt"<cr>
nmap <localleader>em :execute ":vsplit ".expand("%:h:h")."/build/linux/Makefile"<cr>
nmap <localleader>eh :execute ":vsplit ".expand("%:h:h")."/include/".expand("%:h:h")."_helper.h"<cr>

nmap <localleader>bd :!tclsh $ACPLT_HOME/system/systools/build_database_omni.tcl<cr>
nmap <localleader>bg :!tclsh $ACPLT_HOME/system/systools/build_database_omni.tcl GSE<cr>
nmap <localleader>t :!cd $ACPLT_HOME/dev; ctags -Rnu<cr>
nmap <localleader>r :vsplit term://$ACPLT_HOME/system/sysbin/ov_runtimeserver\ -c\ $ACPLT_HOME/servers/MANAGER/ov_server.conf
nnoremap <localleader>r :vsplit term://$ACPLT_HOME/system/sysbin/ov_runtimeserver -c $ACPLT_HOME/servers/MANAGER/ov_server.conf >/dev/null 2>&1\| sed "s,\[ACPLT/OV Error].*,$(tput setaf 1)&$(tput sgr0),; s,\[ACPLT/OV Warning].*,$(tput setaf 3)&$(tput sgr0),; s,TEST(.*PASS,$(tput setaf 2)&$(tputsgr0),"<cr>

function! OvUp()
    new
    sb
    terminal tclsh $ACPLT_HOME/system/systools/build_database_omni.tcl
    terminal tclsh $ACPLT_HOME/system/systools/build_database_omni.tcl MANAGERx

    sleep 500m

  	terminal$ACPLT_HOME/system/sysbin/ov_runtimeserver -c $ACPLT_HOME/servers/MANAGER/ov_server.conf >/dev/null 2>&1 | sed "s,\[ACPLT/OV Error].*,$(tput setaf 1)&$(tput sgr0),; s,\[ACPLT/OV Warning].*,$(tput setaf 3)&$(tput sgr0),; s,TEST(.*PASS,$(tput setaf 2)&$(tput sgr0),"
    file SERVER1
   vnew
    :terminal $ACPLT_HOME/system/sysbin/ov_runtimeserver -c $ACPLT_HOME/servers/MANAGERx/ov_server.conf >/dev/null 2>&1 | sed "s,\[ACPLT/OV Error].*,$(tput setaf 1)&$(tput sgr0),; s,\[ACPLT/OV Warning].*,$(tput setaf 3)&$(tput sgr0),; s,TEST(.*PASS,$(tput setaf 2)&$(tput sgr0),"
    file SERVER2
endfunction

nnoremap <localleader>run :call OvUp()<cr>

nnoremap <localleader>r i$ACPLT_HOME/system/sysbin/ov_runtimeserver -c $ACPLT_HOME/servers/MANAGER/ov_server.conf >/dev/null 2>&1\| sed "s,\[ACPLT/OV Error].*,$(tput setaf 1)&$(tput sgr0),; s,\[ACPLT/OV Warning].*,$(tput setaf 3)&$(tput sgr0),; s,TEST(.*PASS,$(tput setaf 2)&$(tputsgr0),"<cr><esc>

nnoremap <localleader>ra :!curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/dsyncTest/case_self2dsync/obj.actimode&newvalue=3"<cr>
nnoremap <localleader>rs :!curl "http://localhost:7509/createObject?format=plain&path=/TechUnits/dsyncTest&factory=/acplt/syncTest/dsyncTest"<cr>

