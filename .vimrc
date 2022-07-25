nnoremap <F8> :make<CR>
set makeprg=ninja\ -C\ build

nnoremap <F9> :Termdebug -cd=build/src --args ./rpr<CR>
nnoremap <C-F9> :Termdebug -cd=build/tests --args ./tests_main --gtest_filter=TestNorthstar.*<CR>
set path+=src
set path+=math
set path+=tests

