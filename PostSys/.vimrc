set tabstop=4
set softtabstop=4
set shiftwidth=4
set noexpandtab
                    
set colorcolumn=110
highlight ColorColumn ctermbg=darkgray



augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END

let &path.="build/linux, ../ov/include, ../ov/model, ../ov/include/libov"


autocmd VimEnter * NERDTree

let g:ycm_global_ycm_extra_conf = ".ycm_extra_conf.py"
