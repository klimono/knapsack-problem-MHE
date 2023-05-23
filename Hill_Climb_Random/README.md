WORKING DIRECTORY: $ ProjectFileDir $
PROGRAM ARGUMENTS: KNAPSACK_CAPACITY ALGORITHM_ITERATIONS
                  
		   
		   
1. program odczytuje powyższe wartości jako parametry uruchomienia 
2. program tworzy wektor zawierający struktury przedmiotów i wgrywa do niego wartości przyjete w folderze ./dataset, lista zostaje wypisana
3. program wywołuje główną funkcję hillClimbingKnapsack podając jako parametry liste przedmiotów, wielkość plecaka oraz ilość iteracji
4. program generuje losowe rozwiązanie problemu które nie przekracza określonej pojemności plecaka, lista zostaje wypisana
5. program powtarza główną funkcję algorytmu wspinaczkowego określoną liczbę podanych iteracji, tutaj w kazdej iteracji:
	- program rozważa losowy przedmiot znajdujący się w plecaku i porównuje go z innym, który sie w nim nie znajduje
	- jeżeli rozważany przedmiot z poza plecaka może się w nim zmieścić bez koniecznośći zwalniania miejsca, zostaje on dodany do rozwiązania 
	- jeżeli rozważany przedmiot z poza plecaka nie mieści się w nim, ale ma:
		wartość większą lub równą rozważanego przedmiotu z plecaka i 
		po zamienieniu z rozważanym przedmiotem z plecaka rozwiązanie nie przekraczałoby pojemności  
	przedmioty zostają ze sobą zamienione.

	W każdym przypadku zmian program informuje o tym.