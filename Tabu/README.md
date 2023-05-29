WORKING DIRECTORY: $ ProjectFileDir $
PROGRAM ARGUMENTS: KNAPSACK_CAPACITY ALGORITHM_ITERATIONS TABU_SIZE
                  
		   
		   
1. program odczytuje powyższe wartości jako parametry uruchomienia 
2. program tworzy wektor zawierający struktury przedmiotów i wgrywa do niego wartości przyjete w folderze ./dataset, lista zostaje wypisana
3. program wywołuje główną funkcję tabuSearch podając jako parametry liste przedmiotów, wielkość plecaka, ilość iteracji oraz wielkość listy tabu
4. program generuje losowe rozwiązanie problemu które nie przekracza określonej pojemności plecaka, lista zostaje wypisana
5. program powtarza główną funkcję algorytmu tabu określoną liczbę podanych iteracji, tutaj w kazdej iteracji:
	- program generuje liste wszystkich sasiadów dla obecnego rozwiazania
	- program oblicza wartosc wszyskich sąsiadów i wybiera najlepszego SPRADZAJĄC CZY TEN NIE ZNAJDUJE SIĘ W TABU, którego rozwiązanie miesci się w plecaku
	- jeżeli takie rozwiązanie istnieje, zostaje przypisane jako aktualne oraz jest dodawane na listę rozwiązań tabu