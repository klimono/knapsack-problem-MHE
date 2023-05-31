WORKING DIRECTORY: $ ProjectFileDir $
PROGRAM ARGUMENTS: KNAPSACK_CAPACITY ITERATIONS
                  
		   
		   
1. program odczytuje capacity i iterations jako parametry uruchomienia 
2. program tworzy wektor zawierający struktury przedmiotów i wgrywa do niego wartości przyjete w folderze ./dataset, lista zostaje wypisana
3. program wywołuje główną funkcję hillClimbingKnapsack podając jako parametry liste przedmiotów oraz wielkość plecaka
4. program tworzy początkowe rozwiązanie losowe (które jest prawidłowe wzgledem pojemności plecaka)
5. program uruchamia główną pętle programu (określoną przez ITERATIONS) w której:
	- generuje pełne sąsiedztwo względem punktu roboczego
	- wybiera z całego sąsiedztwa najlepsze rozwiązanie
	- jezeli te jest lepsze od aktualnego rozwiązania, te przyjmuje jego wartość

