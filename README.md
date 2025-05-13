Hello ! Welcome inside the amazing world of bash !

This 42 project has been made in collaboration with Chloé Montaigut.


- si le path n'existe pas doit renvoyer un message d'erreur ! a mon avis comme chaque chose de l'env dailleurs
gerer des les debut les caractere speciaux


env -i recup CWD
si besoin : TERM environment variable not set.
env | grep SHLVL


==216265== 2 bytes in 1 blocks are still reachable in loss record 1 of 78
==216265==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==216265==    by 0x4082C9: ft_strdup (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x402FDA: special_token (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x403112: process_token (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x4027AF: parsing_input (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x401440: prompt (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x4019FE: main (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)

=216265== 11 bytes in 3 blocks are still reachable in loss record 5 of 78
==216265==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==216265==    by 0x4082C9: ft_strdup (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x403A14: modify_main_token_lst (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x40322A: join_expand (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x4034B5: do_expand (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x4035E5: handle_word_token (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x4036B5: handle_token (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x40376B: expand_token (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x402803: parsing_input (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x401440: prompt (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x4019FE: main (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)

==216265== 32 bytes in 1 blocks are still reachable in loss record 21 of 78
==216265==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==216265==    by 0x40286C: new_token (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x402982: fill_token_list (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x402FEB: special_token (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x403112: process_token (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x4027AF: parsing_input (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x401440: prompt (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==216265==    by 0x4019FE: main (in /home/skock/Documents/CC/Circle3/lazyshell/minishell

== 2,272 bytes in 71 blocks are still reachable in loss record 58 of 85
==238394==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==238394==    by 0x402265: lst_new (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==238394==    by 0x40254E: fill_env_cpy (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==238394==    by 0x4019F5: main (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==238394== 
==238394== 3,280 bytes in 71 blocks are still reachable in loss record 59 of 85
==238394==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==238394==    by 0x402426: cpy_right (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==238394==    by 0x4022B6: lst_new (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==238394==    by 0x40254E: fill_env_cpy (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)
==238394==    by 0x4019F5: main (in /home/skock/Documents/CC/Circle3/lazyshell/minishell)


lazyshell > mkdir test
lazyshell > cd test
test > mkdir test
test > cd test
test > pwd
/home/skock/Documents/CC/Circle3/lazyshell/test/test
test > rm -rf ../test
AddressSanitizer:DEADLYSIGNAL
=================================================================
==848065==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x0000004e16e3 bp 0x7ffcd038abd0 sp 0x7ffcd038abd0 T0)
==848065==The signal is caused by a READ memory access.
==848065==Hint: address points to the zero page.
    #0 0x4e16e3 in ft_strrchr (/home/skock/Documents/CC/Circle3/lazyshell/minishell+0x4e16e3)
    #1 0x4cb969 in get_last_dir /home/skock/Documents/CC/Circle3/lazyshell/main.c:199:21
    #2 0x4cb70c in prompt /home/skock/Documents/CC/Circle3/lazyshell/main.c:73:10
    #3 0x4ccbe8 in main /home/skock/Documents/CC/Circle3/lazyshell/main.c:229:3
    #4 0x7f63bc7c3d8f in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #5 0x7f63bc7c3e3f in __libc_start_main csu/../csu/libc-start.c:392:3
    #6 0x41f384 in _start (/home/skock/Documents/CC/Circle3/lazyshell/minishell+0x41f384)