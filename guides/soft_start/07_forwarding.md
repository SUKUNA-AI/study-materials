# Перед relay: имя параметра и исходная категория

Первые 7 минут двадцатиминутной задачи U08.D3. После U03.T17 и U06.T8;
на зелёной ветке FORK-CPP шаблоны изучаются позже, но всё равно до U08.
Здесь нет реализации `relay`: сначала проверь типы отдельных выражений.

```cpp
#include <type_traits>
#include <utility>
int main() {
    int number = 6;
    auto&& left = std::forward<int&>(number);
    auto&& right = std::forward<int>(number);
    static_assert(std::is_same_v<decltype(left), int&>);
    static_assert(std::is_same_v<decltype(right), int&&>);
    static_assert(std::is_same_v<decltype((right)), int&>);
}
```

В шаблоне с выводимым параметром `T&&` аргумент-lvalue даёт `T = int&`,
а аргумент-rvalue — `T = int`. Сворачивание ссылок оставляет `&`, если оно есть;
только `&&` вместе с `&&` даёт `&&`. Имя `right` в выражении всё равно lvalue,
хотя объявленный тип — `int&&`. `std::forward<T>` восстанавливает категорию,
закодированную в `T`; безусловный `std::move` этой информации не использует.

Прогноз R6: для `const int c` и `auto&& x = std::forward<const int&>(c)`
каков тип `x`? Снимается ли `const`?

Дальше самостоятельно выбери выражение для вызова готовой `category`.
Справка по карте: Грегуар, гл. 12, PDF 817–824 — вывод параметров шаблона функции.
Конкретное правило forwarding проверено по [forward](https://eel.is/c++draft/forward),
а не приписано этим страницам книги. Все показанные конструкции доступны в C++23.
