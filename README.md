Hello ! Welcome inside the amazing world of bash !

This 42 project has been made in collaboration with Chloé Montaigut.


- si le path n'existe pas doit renvoyer un message d'erreur ! a mon avis comme chaque chose de l'env dailleurs
gerer des les debut les caractere speciaux


env -i recup CWD
si besoin : TERM environment variable not set.
env | grep SHLVL



GROS BUG A FIX =  -->

j'ai changé des choses dans l'expand.

==849081== Memcheck, a memory error detector
==849081== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==849081== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==849081== Command: ./minishell
==849081== 
minisacha > << $dwadaw
> ^C
minisacha > << $dwadaw
> $dwadaw
minisacha > << $dwadaw << here_doc_expand
> $dwadaw
> here_doc_expand
ouououo
==870068== 
==870068== HEAP SUMMARY:
==870068==     in use at exit: 220,387 bytes in 527 blocks
==870068==   total heap usage: 790 allocs, 263 frees, 245,918 bytes allocated
==870068== 
==870068== 5 bytes in 1 blocks are still reachable in loss record 4 of 86
==870068==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==870068==    by 0x4096B9: ft_strdup (in /home/skock/Documents/CC/Circle3/minisacha/minishell)
==870068==    by 0x4033D4: modify_main_token_lst (expander_end.c:78)
==870068==    by 0x4035BA: join_expand (expand_2.c:31)
==870068==    by 0x403875: do_expand (expand_2.c:105)
==870068==    by 0x402F05: handle_word_token (expander.c:48)
==870068==    by 0x403014: handle_token (expander.c:74)
==870068==    by 0x403134: expand_token (expander.c:106)
==870068==    by 0x402473: parsing_input (tokenizer.c:66)
==870068==    by 0x401310: handle_input_prompt (main.c:19)
==870068==    by 0x401567: prompt (main.c:83)
==870068==    by 0x40163E: main (main.c:113)
==870068== 
==870068== 6 bytes in 2 blocks