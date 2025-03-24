Hello ! Welcome inside the amazing world of bash !

This 42 project has been made in collaboration with Chloé Montaigut.



- faire une fonction qui met is_next_space à false si c'est le dernier node obligatoirement ?
- echo $"USER" doit renvoyer USER et pas $USER
- echo ${USER} doit renvoyer skock
- echo $[x] doit renvoyer 0

echo "Bonjour $USER" 'comment' "$HOME"'/'""$SHELL""
ls -la | grep "test" > output.txt
echo "$PATH" | tr ':' '\n' | grep bin >> "$USER"_bins.txt
cat < "$HOME/.bashrc" | head -n 5
echo "imbrication "'de guillemets'" différents" | wc -w
echo "$NON_EXISTANT" "'''"'"'"$USER avec guillemets consécutifs"
ls "$HOME"/"Documents"/'Projets'/"$USER"
echo test"$SHELL"'$USER'"$USER" "test  test" '' "$?"
<<EOF cat | sort | uniq > "sorted_$USER.txt"