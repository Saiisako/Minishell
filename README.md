Hello ! Welcome inside the amazing world of bash !

This 42 project has been made in collaboration with Chloé Montaigut.


PARSING :
- echo $"USER" doit renvoyer USER et pas $USER

EXE :
- si le path n'existe pas doit renvoyer un message d'erreur ! a mon avis comme chaque chose de l'env dailleurs
gerer des les debut les caractere speciaux


env -i recup CWD
si besoin : TERM environment variable not set.
env | grep SHLVL

lazyshell > $EMPTY echo hi
=================================================================
==433057==ERROR: AddressSanitizer: heap-use-after-free on address 0x603000001498 at pc 0x0000004d40d8 bp 0x7ffe23d95970 sp 0x7ffe23d95968
READ of size 8 at 0x603000001498 thread T0
    #0 0x4d40d7 in expand_token /home/skock/Documents/CC/Circle3/lazyshell/parsing/expander.c:149:15
    #1 0x4d050f in parsing_input /home/skock/Documents/CC/Circle3/lazyshell/parsing/tokenizer.c:168:2
    #2 0x4cb6ca in prompt /home/skock/Documents/CC/Circle3/lazyshell/main.c:66:8
    #3 0x4cc888 in main /home/skock/Documents