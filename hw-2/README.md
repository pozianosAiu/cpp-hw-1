# README — ФизКат

## Как собрать и запустить

Сборка (clang++ из VS Code, C++20):

```
clang++ -g -std=c++20 -Wall -Wextra -fcolor-diagnostics main.cpp -o build/main
```

Запуск:

```
./build/main
```

---

## Схема наследования

```
InventoryItem  (abstract, entity: id_ + name_)
├── Transport  (abstract, score_ 1..10, проверка "broken" → InspectionFailed)
│   ├── Bike                              (обычный велосипед)
│   ├── Scooter         + EnergyConsumer  ← множественное наследование
│   ├── ElectricBike    + EnergyConsumer  ← множественное наследование
│   └── SpeedScooter    + EnergyConsumer  ← множественное наследование, score всегда 1
├── Helmet                                (вещь без энергии)
└── DockStation        + EnergyConsumer  ← множественное наследование

EnergyConsumer  (abstract, interface: dailyEnergy())
```

**Множественное наследование:** `Scooter`, `ElectricBike`, `SpeedScooter`, `DockStation` наследуются от двух базисов одновременно. `virtual public` не используется, потому что `Transport` и `EnergyConsumer` — независимые ветки, общего предка нет, ромба нет.

---

## Где `unique_ptr`, `shared_ptr`, `weak_ptr` и почему

| Где | Тип | Почему |
|---|---|---|
| `ServiceCenter::storage_` | `unordered_map<std::string, shared_ptr<InventoryItem>>` | Парк. `shared_ptr`, а не `unique_ptr`, потому что `weak_ptr` (витрина `focus_`, журнал `last_added_`) невозможно создать из `unique_ptr`. Задание разрешает «аналог» — им и пользуемся. |
| `ServiceCenter::focus_` | `weak_ptr<InventoryItem>` | Витрина. Не владеет объектом, не продлевает жизнь. После `remove` корректно становится `expired()`. |
| `ServiceCenter::last_added_` | `Registry<weak_ptr<InventoryItem>, 5>` | Журнал последних добавленных. Слабая ссылка — если объект удалён, он автоматически «expired», и мы это видим при печати. |
| `ServiceCenter::last_deleted_` | `Registry<unique_ptr<InventoryItem>, 5>` | Журнал последних удалённых. Хранит **копию** через `clone()` (полиморфное клонирование), чтобы удалённый объект нельзя было случайно вернуть в парк или сослаться на него. Оригинал удаляется из `storage_` полностью. |
| `InventoryItem::clone()` | возвращает `unique_ptr<InventoryItem>` | Полиморфное копирование. Реализован в каждом конкретном классе как `make_unique<СвойКласс>(*this)`. Используется только для `last_deleted_`. |

Сырой `new`/`delete` нигде не используется. `Rule of Zero`: все поля — RAII-типы, копирующие/перемещающие конструкторы не объявлены, компилятор генерирует их корректно. Единственный объявленный деструктор — виртуальный `= default` в базисах, для полиморфного удаления.

---

## Исключения

```
std::runtime_error
└── FleetError
    ├── InspectionFailed  — техосмотр транспорта не пройден (имя содержит "broken" без учёта регистра)
    └── InvalidArgument   — пустой id/name, score вне 1..10, отрицательная энергия,
                            дубликат id, неверное число аргументов, невалидный формат числа,
                            неизвестный тип, item not found
```

- `throw` происходит внутри доменных операций: в конструкторах (`InventoryItem`, `Transport`, `Scooter`, `ElectricBike`, `SpeedScooter`, `DockStation`) и в `ServiceCenter::add` / `focus` / `remove`.
- `catch` — только в `main`. Один блок `catch (const FleetError& e)` печатает `error: <what()>`.
- После исключения состояние парка не портится: если конструктор бросил, объект не создан, в `storage_` ничего не добавлено.

---

## Какой шаблон добавлен и где используется

`Registry<T, Cap>` — кольцевой буфер фиксированной ёмкости `Cap` на этапе компиляции. Методы: `add`, `size`, `operator[]` (индекс 0 — самый свежий элемент).

Используется в двух инстансах с разными типами (см. таблицу умных указателей), что и оправдывает шаблонность: одна и та же логика кольца применяется к `weak_ptr` и `unique_ptr`, без дублирования кода.

---

## Что сделано в креативной части

1. **`SpeedScooter`** — новый тип транспорта. Фиксированный `score = 1` (не для новичков), не принимает `score` из команды. Команда: `add speedscooter <id> <name> <energy>`.
2. **Журнал последних добавленных** (`last_added`) — 5 записей, `weak_ptr`. Если объект уже удалён — при печати показывается `removed item`.
3. **Журнал последних удалённых** (`last_deleted`) — 5 записей, `unique_ptr` через `clone()`. Позволяет посмотреть, что было удалено, не возвращая объект в парк.

---

## Использование ИИ

- **Сгенерировано ИИ:** текст команды `help` в `main.cpp`, функция divide в ServiceCenter и этот README. Всё остальное написано вручную.

---

## Приложение: команды

| Команда | Смысл |
|---|---|
| `add scooter <id> <name> <energy> <score>` | Электросамокат |
| `add bike <id> <name> <score>` | Велосипед |
| `add ebike <id> <name> <energy> <score>` | Электровелосипед |
| `add speedscooter <id> <name> <energy>` | Скоростной самокат (score = 1) |
| `add helmet <id> <name>` | Шлем |
| `add dock <id> <name> <energy>` | Док-станция |
| `energy` | Суммарное суточное энергопотребление |
| `novice` | Транспорт с `score >= 6` |
| `list` | Все позиции, сортировка по ID |
| `focus <id>` | Установить витрину |
| `show_focus` | Показать витрину или `expired` |
| `remove <id>` | Удалить из парка |
| `last_added` | Последние 5 добавленных |
| `last_deleted` | Последние 5 удалённых |
| `help` | Список команд |
| `quit` | Выход |

---
