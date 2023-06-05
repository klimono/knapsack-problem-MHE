WORKING DIRECTORY: $ ProjectFileDir $
PROGRAM ARGUMENTS: KNAPSACK_CAPACITY ALGORITHM_ITERATIONS 
                  
		   
		   
1. program odczytuje powyższe wartości jako parametry uruchomienia 
2. program tworzy wektor zawierający struktury przedmiotów i wgrywa do niego wartości przyjete w folderze ./dataset, lista zostaje wypisana
3. program wywołuje główną funkcję sim_annealing podając jako parametry liste przedmiotów, wielkość plecaka, ilość iteracji oraz funkcje temperatury
4. program generuje losowe rozwiązanie problemu które nie przekracza określonej pojemności plecaka, lista zostaje wypisana
5. program powtarza główną funkcję algorytmu tabu określoną liczbę podanych iteracji, tutaj w kazdej iteracji:
	- program generuje sąsiednie rozwiązanie
	- program przyjmuje je jako aktualne jeżeli jest lepsze
	- jezeli jest gorsze, program zgodnie z algorytmem SA może je przyjąć jeżeli jest nieznacznie gorsze
	- program zapisuje najlepsze dotychczasowe rozwiązanie