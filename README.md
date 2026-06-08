# Saturn — карта психологической обстановки

Прототип десктоп-приложения (Qt 6 / QML + QMapLibre + PostgreSQL) для отображения
и анализа психологической обстановки на карте. Базовая подложка — векторные тайлы,
раздаваемые локальным tileserver-gl; объекты обстановки приходят из PostgreSQL.

> Этот файл — точка входа для нового разработчика. Подробности по подсистемам — в `docs/`.

---

## 1. Что нужно установить

| Компонент | Версия / примечание |
|-----------|---------------------|
| Qt | **6.5.3**, набор `msvc2019_64` (путь по умолчанию `C:\Qt\6.5.3\msvc2019_64`) |
| QMapLibre | поставляется вместе с Qt-китом (`.../lib/cmake/QMapLibre`) |
| Компилятор | Visual Studio 2022 **или** VS 2022 BuildTools (`vcvars64`) |
| CMake | ≥ 3.21 (идёт с VS) |
| PostgreSQL | сервер + клиентские `libpq.dll` (без PostGIS) |
| Node.js | для `tileserver-gl` (раздача тайлов) |

Если Qt стоит не в `C:\Qt\6.5.3\...`, поправь пути в `CMakePresets.json`
(`Qt6_DIR`, `QMapLibre_DIR`).

---

## 2. Сборка

Через CMake-пресеты (см. `CMakePresets.json`):

```powershell
# Visual Studio генератор
cmake --preset windows-qt653-maplibre
cmake --build --preset windows-qt653-maplibre-debug

# либо Ninja из "x64 Native Tools Command Prompt"
cmake --preset windows-buildtools-ninja
cmake --build --preset windows-buildtools-ninja-debug
```

Пресеты собирают в каталог `build/`. В репозитории также присутствует
исторический каталог сборки `build_ncuorf/` (игнорируется git) — это локальная
сборка автора; новому разработчику он не нужен, ориентируйся на `build/`.

После линковки `POST_BUILD`-шаг сам раскладывает рядом с `saturn.exe` Qt-рантайм
(`windeployqt`), плагин `mapview`, `MyMapView.qml`, нужные DLL QMapLibre и
клиент PostgreSQL. Готовый `saturn.exe` лежит в `<build>/Debug/`.

---

## 3. База данных

1. Подними PostgreSQL и создай БД (исторически называется `SATURN`).
2. Применить миграции из `db_saturn/migrations/` (новая геометрия объектов):
   - `20260331_object_geometry.sql`
   - `20260402_object_geometry_integrity.sql`
   - `20260403_object_geometry_change_log.sql`
3. Залить демо-данные обстановки (дамп предоставляется отдельно — **не в репозитории**).
4. Параметры подключения вводятся **в самом приложении** на экране «Подключение к БД»
   и сохраняются в QSettings (`HKCU\Software\vka\saturn*`). Хардкода кредов в коде нет.

Обзор схемы и соответствие типов объектов таблицам — в авто-памяти проекта и в
`docs/backend_map_api_contract.md`.

---

## 4. Тайлы карты (tileserver-gl)

Тайлы (`.mbtiles`, единицы–десятки ГБ) **в репозиторий не входят**.

1. Скачать исходные данные:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\tools\download_map_data.ps1
   ```
   Подробности и источники — `docs/mbtiles_data_sources_and_download.md`.
2. Сгенерировать `.mbtiles` (пайплайн — `docs/tileserver_geojson_pipeline.md`).
3. Положить готовый `.mbtiles` в каталог с данными и указать его в
   `tools/start_tileserver.ps1` (параметр `-MbtilesSource`, по умолчанию
   `E:\saturn_tiles_data` — **это путь автора, его нужно заменить на свой**) и
   в `tileserver-gl/config.json` (`data.openmaptiles.mbtiles`).
4. Запустить раздачу:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\tools\start_tileserver.ps1
   ```
   Сервер слушает `http://localhost:8080`. Скрипт сам создаёт NTFS-junction на
   каталог данных (обход бага `@mapbox/mbtiles` с путём вида `E:/…`) и при первом
   запуске переводит `.mbtiles` в режим WAL для параллельного чтения.

> tileserver-gl читает стили **только при старте** — после правок
> `tileserver-gl/styles/**/style.json` сервер нужно перезапустить.

---

## 5. Запуск

1. Запустить tileserver-gl (см. выше).
2. Запустить PostgreSQL с применёнными миграциями и данными.
3. Запустить `<build>/Debug/saturn.exe`, войти и подключиться к БД.

---

## 6. Структура репозитория (основное)

| Каталог | Назначение |
|---------|------------|
| `mainform/` | точка входа, оболочка QML (`AppShell.qml`), панели интерфейса |
| `mapview/` | карта на QMapLibre (`MyMapView.qml`), плагин `mapview` |
| `dataaccess/` | C++-сервисы и QML-синглтоны (рантайм карты, репозитории, БД) |
| `db_saturn/` | доступ к БД и SQL-миграции |
| `tileserver-gl/` | конфиг и стили локального тайл-сервера |
| `tools/` | скрипты: загрузка данных, запуск tileserver |
| `docs/` | контракты API, пайплайн тайлов, аудит репозитория |
| `coord/`, `event_manager/`, `signs_edit/`, `reports/`, … | legacy-модули |

---

## 7. Машинно-зависимые места (проверить при переносе)

- `CMakePresets.json` — пути к Qt / QMapLibre.
- `tools/start_tileserver.ps1` (`-MbtilesSource`) и `tileserver-gl/config.json` — путь к `.mbtiles`.
- `mapview/MyMapView.qml` → `gadmBasePath` — опциональный overlay админграниц из
  файлов GADM (`gadm41_RUS_<level>.json`). По умолчанию **пусто** = overlay
  выключен, госграницы берутся из стиля tileserver. Чтобы включить — укажи путь к
  каталогу с GADM-файлами.

---

## 8. Известное состояние / WIP

- **Список объектов** в правой панели и **отображение спорных территорий**
  (Крым, Севастополь, ДНР, ЛНР, Запорожская и Херсонская обл.) — в активной
  отладке. Спорные госграницы скрываются фильтром в стилях tileserver
  (`admin_country_*`: `!has claimed_by`, `disputed != 1`); если в конкретном
  `.mbtiles` эти участки не помечены тегом `disputed`, потребуется геометрическая
  маска поверх спорных регионов.
- Тесты в проекте отсутствуют; проверка — ручная, запуском приложения.
