# Допустить только целые слова

*Реализуй (Т10) · задача дня U04, запас (Вт) · судья, 16 тестов · уровень: самостоятельно*

## Коротко
Объяви concept `Word<T>` — целые типы, кроме `bool`, — и ограничь им шаблон `storage_bytes`.
Вызов для запрещённого типа не должен быть доступен.

## Зачем
Ограничение (concept) отсекает неверный тип элемента буфера при компиляции, до запуска ядра.

## Вход и выход
```cpp
template<class T> concept Word = /* … */;
template<Word T> std::size_t storage_bytes(std::span<const T> values);  // size() * sizeof(T)
```
- `Word<T>` истинен ровно для `std::integral<T>`, кроме `bool` с любыми `const` и `volatile`.
- Ограничение — в объявлении шаблона. `static_assert` в теле не подходит: проверка `requires` его не видит.
- Число элементов — от 0 до 1024.

| тип | `n` | результат |
|---|---|---|
| `std::int32_t` | 3 | `12` |
| `double` | 2 | вызов недоступен |

## Проверь, что понял
Сколько типов из списка `std::int8_t`, `bool`, `const bool`, `double`, `std::byte`, `std::uint64_t` допускает `Word`?

## Шаги
1. **5 минут:** `judge pull d-u04-tue-word-concept`, открой заготовку: `Word` и `storage_bytes` написаны неверно. Найди открытый тест, который она не пройдёт.
2. Сформулируй `Word` через `std::integral` и исключи `bool` вместе с его `const`-вариантами.
3. Посчитай байты, а не элементы.
4. Проверь запрет через `requires`-выражение для `bool`, `const bool` и `double`, затем сдай.

## Готово, когда
- [ ] 16 из 16 тестов — OK.
- [ ] Ты можешь объяснить, почему `static_assert` в теле не делает вызов недоступным для `requires`.

## Сдать
`solution.cpp` без `main`, шаблоны — целиком в файле:
`judge submit d-u04-tue-word-concept d-u04-tue-word-concept/solution.cpp --wait`

## Как проверяется
- 4 открытых и 12 скрытых тестов, 1 балл за тест.
- Стенд проверяет доступность вызова через `requires` для девяти типов: `int8_t`, `int16_t`, `int32_t`, `uint64_t`, `const int32_t`, `bool`, `const bool`, `double`, `std::byte`. Для запрещённого типа `bytes` — `null`.
- Скрытые: допустимые типы при `n = 0`, 1 и 1024; `std::byte`, `bool` и `double` при разных `n`.
- Судья собирает g++ 14 `-std=c++23`: сначала с ASan и UBSan, затем `-O2`. Эталон проверен и на MSVC `/std:c++latest`. Лимит — 1 с и 256 МиБ на тест.

## Читать
- cppreference: [std::integral — definition](https://en.cppreference.com/w/cpp/concepts/integral.html); [Constraints — requires expressions](https://en.cppreference.com/w/cpp/language/requires.html) — справка.
- CiA, прил. A.2–A.3, PDF 381–386: ограничения интерфейса через `delete` и `default` — сравнение с C3, справка.

## После решения
1. Что было самым трудным?
2. Какой тип элемента буфера в твоих ядрах стоит запретить concept-ом и почему?

## Оценка
20–25 мин, дом или ноут
