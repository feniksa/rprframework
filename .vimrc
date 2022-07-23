nnoremap <F8> :make<CR>
set makeprg=ninja\ -C\ build

nnoremap <F9> :Termdebug -cd=build/src --args ./rpr<CR>
set path+=src
set path+=tests

