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
