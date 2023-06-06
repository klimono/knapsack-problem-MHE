WORKING DIRECTORY: $ ProjectFileDir $
PROGRAM ARGUMENTS: CAPACITY WANTED_VALUE POPULATION_SIZE MAX_ITERATIONS
                  
		   
		   
1. program odczytuje powyższe wartości jako parametry uruchomienia 
2. program tworzy wektor zawierający struktury przedmiotów i wgrywa do niego wartości przyjete w folderze ./dataset, lista zostaje wypisana
3. program wywołuje główną funkcję geneticAlghoritm 
4. program generuje początkową populacje wielkości POPULATION_SIZE
5. program uruchamia główną pętlę algorytmu genetycznego o warunkach MAX_ITERATIONS oraz WANTED_VALUE
	w każdej iteracji pętli program:
	- program zapamiętuje dwa najsilniejsze genotypy (elitę)
	- tworzy ruletkę do krzyżowania spośród osobników, rozmiar pola określa moc genotypu
	- losuje jedną spośród dwóch metod krzyżowania i przeprowadza ją na losowych parach genotypów
	- z prawdopodobonieństwem wynoszącym 1/4 przeprowadza jedną spośród dwóch dostępnych mutacji na każdym genotypie populacji
	- program nadpisuje dwa najsilniejsze genotypy (elitę) nad dwoma najsłabszymi genotypami nowego pokolenia
	
6. dostępne metody krzyżowania to:
	- krzyżowanie jednorodne zamieniające losowo poszczególne cechy dwóch genotypów (aż do osiągnięcia limitu CAPACITY)
	- krzyżowanie punktowe, losujące punkt względem którego krzyżowo są zamieniane cechy dwóch genotypów (aż do osiągnięcia limitu CAPACITY)
	
7. dostępne metody mutacji to:
	- mutacja korzystna generująca dla genotypu wszystkich możliwych sąsiadów z wybraniem najlepszego
	- mutacja zmieniająca genotyp na inny, losowy 