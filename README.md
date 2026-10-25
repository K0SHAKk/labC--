# Лабораторная работа 5

Головков Алексей Олегович, группа 8В42.
Тема: ООП, классы и структуры. Физический симулятор на C++17 и SFML 2.
Репозиторий: https://github.com/K0SHAKk/labC--/tree/lab-5

Реализованы Ball, перегрузки ввода Point/Color/Velocity, отключение всех коллизий
для isCollidable=false и отдельные частицы Dust со сроком жизни 0,8 с.
Шаг физики 0,001 с. Окно останавливает симуляцию точно на 10 с.
Колесо мыши меняет масштаб; крестик закрывает окно.

## Сборка

Нужны CMake 3.16+, компилятор C++17 и SFML 2.5/2.6.
Если SFML не установлен, CMake загружает и собирает SFML 2.6.2.
В Linux нужны пакеты из шага Install Linux dependencies в workflow.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure
```

Для Visual Studio исполняемый файл находится в build/Release/physics.exe,
для Ninja/MinGW — в build/physics.exe, для Linux — в build/physics.

```sh
./build/physics data/smile.txt
./build/physics data/elephant.txt
./build/physics data/smile.txt --snapshot 10 smile.png
```

В Windows используйте соответствующий путь к physics.exe. Для готовой сборки
используйте start_smile.bat и start_elephant.bat рядом с physics.exe.
В архиве с полным проектом готовое приложение находится в подкаталоге windows.
Режим --snapshot использует тот же C++ движок и SFML для сохранения PNG
на заданной секунде (0–10), без открытия основного окна.

## Тесты и автоматическая сборка

10 тестов проверяют массу, ввод, упругое столкновение равных масс,
проход сквозь шары и стенки, угловой удар, удаление частиц, обе модели,
обработку поврежденных файлов, дробные интервалы времени и совпавшие центры.
Для тестов без графических зависимостей: добавьте -DBUILD_GRAPHICS=OFF.

.github/workflows/build.yml сохраняет схему релизов исходного YAML.
В Linux сначала собирается и тестируется Release, затем создается Debug DEB.
В macOS собирается и тестируется Release, затем создается ZIP.
Каждая платформа публикует свой релиз; N — номер запуска Actions:

- Linux: тег N-Linux2, название Release N for linux2, один файл lab4-0.0.N-Linux.deb.
- macOS: тег N, название Release N (MacOS), один файл lab4-0.0.N-Darwin.zip.

Имена вложений с lab4 сохранены из исходного YAML. Внутри находится текущая
программа лабораторной работы 5. Оба релиза обычные, не черновые и не prerelease.
Дополнительные ZIP, TAR.GZ и отдельные релизы Debug/Release не публикуются.

Workflow запускается при push и pull request в main, lab-2, lab-3, lab-4,
lab-5, feature/github_actions, при публикации релиза и вручную через Actions.
При pull request сохраняются только артефакты. Для публикации используется
встроенный GITHUB_TOKEN с разрешением contents: write; отдельный секрет не нужен.
Для локальной сборки номер версии можно задать через -DPATCH_VERSION=42.

Локальное создание пакетов после сборки:

```sh
cpack --config build/CPackConfig.cmake -C Release -B build/packages
```

В ZIP приложение и каталог data находятся рядом. Пакет DEB
устанавливает их в /opt/physics-lab5; пример запуска после установки:

```sh
/opt/physics-lab5/physics /opt/physics-lab5/data/smile.txt
```

Алгоритм изменения скоростей в Physics сохранен из учебного примера:
исходные модели согласованы с ним. Это учебная модель, а не универсальный
физический движок для шаров произвольных масс.

## Формат модели

Первая строка: minX minY maxX maxY. Каждая следующая непустая строка:
x y vx vy red green blue radius true/false.
Цветовые компоненты находятся в диапазоне 0–1; радиус положителен.
Объекты с false могут начинать движение за пределами прямоугольника.
