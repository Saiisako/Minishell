Hello ! Welcome inside the amazing world of bash !

This 42 project has been made in collaboration with Chloé Montaigut.


PARSING :
- echo $"USER" doit renvoyer USER et pas $USER
<!-- - echo $[x] doit renvoyer 0 -->
- si >>caca  soit doit faire >> caca (2 tokens) pareil pour |, <<, <, > !
a gerer des le debut et uniquement si c'est pas entre quote.
- si apres << il y a $USER -> il ne faut pas d'expander
- avec echo si les arguments sont "" "$USER" ca doit ca doit echo : _skock et pas : skock.
s il y a deux caracteres speciaux qui se suivent c'est une erreur.


EXE :
- si le path n'existe pas doit renvoyer un message d'erreur ! a mon avis comme chaque chose de l'env dailleurs
gerer des les debut les caractere speciaux
- si un pipe(special char) est le premier token, "unexpected end of line".


si lii y a une error faut pas quitter frere
