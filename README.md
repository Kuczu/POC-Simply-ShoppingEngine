# POC-Simply-ShoppingEngine

Bardzo prosty program do zarządzania listą zakupów. Napisany jako zadanie z kursu programowania z zakresu struktur i obsługi plików.

Program pozwala na zapis do pliku i wczytanie z już istniejącego listy zakupów. Program pozwala na utworzenie kategorii i przypisanie do niej przedmiotów, przedmiotom pozwala nadać cenę oraz jednostkę. Prócz wypisania listy przedmiotów z podziałem na kategorie pozawala wyświetlić podsumowanie kategorii.

Kod należy skompilować z flagą -std=c++11 (wymaga c++ w wersji 11+).

```
g++ -std=c++11 -o shoppingEng shoppingEng.cpp
```

Aby uruchomić program należy jako argument podać plik "z bazą".

```
hoppingEng.exe test.txt
```
