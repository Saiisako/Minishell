Hello ! Welcome inside the amazing world of bash !

This 42 project has been made in collaboration with Chloé Montaigut.



- faire une fonction qui met is_next_space à false si c'est le dernier node obligatoirement ?
- echo $"USER" doit renvoyer USER et pas $USER
- echo $[x] doit renvoyer 0
- si >>caca  soit doit faire >> caca (2 tokens) pareil pour |, <<, <, > !
a gerer des le debut et uniquement si c'est pas entre quote.
- echo "" caca caca   si "" merge alros els "" disparaissent sinon ca reste une chaine vide !

si il peux y avoir un merge je dois supprimer la premiere " ou ' jusqua a la prochaine !
si un pipe(special char) est le dernier token, "unexpected end of line".
si un pipe(special char) est le premier token, "unexpected end of line".
avec echo si les arguments sont "" "$USER" ca doit ca doit echo : _skock et pas : skock.