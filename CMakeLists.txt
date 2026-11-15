cmake_minimum_required(VERSION 3.10)


# Название нашего проекта и язык программирования (C)
project(Happy-Farmer C)


# Говорим, где лежат наши заголовочные файлы (.h)
include_directories(include)


# Собираем исполняемый файл farm_game из всех исходников (.c)
add_executable(farm_game 
    src/main.c
    src/constants.c
    src/utils.c
    src/parser.c
    src/map.c
    src/player.c
    src/menu.c
)

# "Математические функции живут в отдельной библиотеке m — подключаем её" - Яндекс Алиса
target_link_libraries(farm_game PRIVATE m)
